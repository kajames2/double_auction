#ifndef _EXPERIMENT_EXPERIMENT_STATE_H_
#define _EXPERIMENT_EXPERIMENT_STATE_H_

#include <ostream>

#include "experiment/experiment_enums.h"

namespace experiment {

struct ExperimentState {
  int round;
  Stage stage;
  Status status;
};

std::ostream& operator<<(std::ostream& os, ExperimentState state);

}  // namespace experiment

#endif  // _EXPERIMENT_EXPERIMENT_STATE_H_
