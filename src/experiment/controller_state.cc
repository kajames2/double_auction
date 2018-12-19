#include "experiment/controller_state.h"

namespace experiment {

std::ostream& operator<<(std::ostream& os, ControllerState state) {
  os << state.exp_state << '\n';
  os << "Time Remaining: " << state.time_remaining.count() << " seconds\n\n";
  return os << state.market_state;
}

}  // namespace experiment
