#ifndef _TOOLS_PAUSABLE_TIMER_H_
#define _TOOLS_PAUSABLE_TIMER_H_

#include <asio.hpp>
#include <chrono>
#include <functional>
#include <iostream>

namespace tools {

class PausableTimer {
 public:
  PausableTimer(asio::io_context& io, std::chrono::seconds duration,
                std::function<void(const std::error_code&)> callback);
  std::chrono::duration<float> GetTimeRemaining() const;
  std::chrono::duration<float> GetTimeElapsed() const;
  void Reset();
  void Pause();
  void Resume();

 private:
  void Tick(const std::error_code& e);

  asio::steady_timer timer_;
  std::chrono::duration<float> time_remaining_;
  std::chrono::duration<float> duration_;
  std::chrono::milliseconds interval_;
  std::function<void(const std::error_code&)> callback_;
  bool is_running_ = true;
  bool done_ = false;
};

}  // namespace tools

#endif  // _TOOLS_PAUSABLE_TIMER_H_
