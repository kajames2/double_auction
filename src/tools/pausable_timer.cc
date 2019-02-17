#include "tools/pausable_timer.h"

namespace tools {

PausableTimer::PausableTimer(
    boost::asio::io_context& io, std::chrono::seconds duration,
    std::function<void(const boost::system::error_code&)> callback)
    : timer_(io, std::chrono::milliseconds(50)),
      callback_(callback),
      duration_(duration),
      time_remaining_(duration),
      interval_(std::chrono::milliseconds(50)) {
  timer_.async_wait([this](const boost::system::error_code& e) { Tick(e); });
}

std::chrono::duration<float> PausableTimer::GetTimeRemaining() const {
  return time_remaining_;
}

std::chrono::duration<float> PausableTimer::GetTimeElapsed() const {
  return std::chrono::duration<float>(duration_ - time_remaining_);
}

void PausableTimer::Reset() {
  time_remaining_ = duration_;
  timer_.async_wait(callback_);
  timer_.cancel();
  is_running_ = false;
  done_ = false;
}

void PausableTimer::Pause() { is_running_ = false; }

void PausableTimer::Resume() { is_running_ = true; }

void PausableTimer::Tick(const boost::system::error_code& e) {
  if (e == boost::asio::error::operation_aborted) {
    return;
  }
  if (is_running_) {
    time_remaining_ -= interval_;
  }
  if (time_remaining_ <= std::chrono::microseconds(0)) {
    timer_.async_wait(callback_);
    is_running_ = false;
    done_ = true;
  } else {
    timer_.expires_after(interval_);
    timer_.async_wait([this](const boost::system::error_code& e) { Tick(e); });
  }
}

}  // namespace tools
