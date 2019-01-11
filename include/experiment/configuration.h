#ifndef _EXPERIMENT_CONFIGURATION_H_
#define _EXPERIMENT_CONFIGURATION_H_

#include <chrono>
#include <functional>
#include <map>
#include <ostream>
#include <vector>

#include "market/holdings.h"
#include "experiment/experiment_enums.h"

namespace experiment {

struct Configuration {
  std::vector<std::map<int, market::Holdings>> init_holdings;
  std::map<int, std::function<int(market::Holdings)>> utility_funcs;
  std::map<int, double> eperdollars;
  std::map<int, PlayerType> player_types;
  std::chrono::seconds round_time;
  std::chrono::seconds review_time;
  int n_players;
  int n_rounds;
};

}  // namespace experiment

#endif  // _EXPERIMENT_CONFIGURATION_H_
