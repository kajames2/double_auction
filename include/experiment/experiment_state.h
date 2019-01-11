#ifndef _EXPERIMENT_EXPERIMENT_STATE_H_
#define _EXPERIMENT_EXPERIMENT_STATE_H_

#include <iostream>

#include "experiment/experiment_enums.h"

namespace experiment {

struct ExperimentState {
  int round;
  Stage stage;
  Status status;
};

void PrettyPrint(std::ostream& os, ExperimentState state);
std::ostream& operator<<(std::ostream& os, ExperimentState state);
std::istream& operator>>(std::istream& is, ExperimentState& state);
}  // namespace experiment

#endif  // _EXPERIMENT_EXPERIMENT_STATE_H_
