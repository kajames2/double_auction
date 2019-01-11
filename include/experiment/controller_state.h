#ifndef _EXPERIMENT_CONTROLLER_STATE_H_
#define _EXPERIMENT_CONTROLLER_STATE_H_

#include <chrono>
#include <iostream>

#include "experiment/experiment_state.h"
#include "market/market_state.h"

namespace experiment {

struct ControllerState {
  ExperimentState exp_state;
  std::chrono::duration<float> time_remaining;
  market::MarketState market_state;
};

std::ostream& operator<<(std::ostream& os, const ControllerState& state);
std::istream& operator>>(std::istream& is, ControllerState& state);

}  // namespace experiment

#endif  // _EXPERIMENT_CONTROLLER_STATE_H_
