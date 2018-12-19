#ifndef _MARKET_PAUSABLE_TIMER_H_
#define _MARKET_PAUSABLE_TIMER_H_

#include <asio.hpp>
#include <chrono>
#include <functional>
#include <iostream>

namespace market {

class PausableTimer {
 public:
  PausableTimer(asio::io_context& io, std::chrono::seconds duration,
                std::function<void(const asio::error_code&)> callback);
  std::chrono::duration<float> GetTimeRemaining() const;
  std::chrono::duration<float> GetTimeElapsed() const;
  void Reset();
  void Pause();
  void Resume();

 private:
  void Tick(const asio::error_code& e);

  asio::steady_timer timer_;
  std::chrono::duration<float> time_remaining_;
  std::chrono::duration<float> duration_;
  std::chrono::milliseconds interval_;
  std::function<void(const asio::error_code&)> callback_;
  bool is_running_ = true;
  bool done_ = false;
};

}  // namespace market

#endif  // _MARKET_PAUSABLE_TIMER_H_
