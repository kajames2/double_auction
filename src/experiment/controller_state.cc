#include "experiment/controller_state.h"

namespace experiment {

std::ostream& operator<<(std::ostream& os, const ControllerState& state) {
  os << state.exp_state << '\n';
  os << state.time_remaining.count() << '\n';
  return os << state.market_state;
}

std::istream& operator>>(std::istream& is, ControllerState& state) {
  is >> state.exp_state; is.ignore();
  float time_remaining;
  is >> time_remaining; is.ignore();
  state.time_remaining = std::chrono::duration<float>(time_remaining);
  return is >> state.market_state;
}

}  // namespace experiment
