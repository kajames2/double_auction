#ifndef _EXPERIMENT_CONTROLLER_STATE_H_
#define _EXPERIMENT_CONTROLLER_STATE_H_

#include <chrono>
#include <ostream>

#include "experiment/experiment_state.h"
#include "market/market_state.h"

namespace experiment {

struct ControllerState {
  ExperimentState exp_state;
  std::chrono::duration<float> time_remaining;
  market::MarketState market_state;
};

std::ostream& operator<<(std::ostream& os, ControllerState state);

}  // namespace experiment

#endif  // _EXPERIMENT_CONTROLLER_STATE_H_
