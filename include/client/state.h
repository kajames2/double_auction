#ifndef _CLIENT_STATE_H_
#define _CLIENT_STATE_H_

#include <ostream>
#include <chrono>

#include "experiment/experiment_enums.h"
#include "experiment/experiment_state.h"
#include "market/market_state.h"

namespace client {

struct State {
  int id;
  experiment::PlayerType type;
  std::chrono::duration<float> time_remaining;
  experiment::ExperimentState exp_state;
  market::MarketState market_state;
  int cents_earnings;
};

std::ostream& operator<<(std::ostream& os, const State& state);

}  // namespace client

#endif  // _CLIENT_STATE_H_
