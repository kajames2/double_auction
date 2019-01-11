#include "experiment/experiment_state.h"

namespace experiment {

void PrettyPrint(std::ostream& os, ExperimentState state) {
  os << "Status: " << state.status << '\n';
  os << "Stage: " << state.stage << '\n';
  os << "Round " << state.round;
}

std::ostream& operator<<(std::ostream& os, ExperimentState state) {
  return os << state.round << ' ' << state.status << ' ' << state.stage << ' ';
}

std::istream& operator>>(std::istream& is, ExperimentState& state) {
  return is >> state.round >> state.status >> state.stage;
}
}  // namespace experiment
