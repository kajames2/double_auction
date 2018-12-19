#include "experiment/experiment_state.h"

namespace experiment {

std::ostream& operator<<(std::ostream& os, ExperimentState state) {
  os << "Status: " << state.status << '\n';
  os << "Stage: " << state.stage << '\n';
  return os << "Round " << state.round;
}

}  // namespace experiment
