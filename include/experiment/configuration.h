#ifndef _EXPERIMENT_CONFIGURATION_H_
#define _EXPERIMENT_CONFIGURATION_H_

#include <functional>
#include <map>
#include <vector>
#include <chrono>

#include "market/holdings.h"

namespace experiment {

struct Configuration {
  std::vector<std::map<int, market::Holdings>> init_holdings;
  std::map<int, std::function<int(market::Holdings)>> utility_funcs;
  std::map<int, double> eperdollars;
  std::chrono::seconds round_time;
  int n_players;
};

}  // namespace experiment

#endif  // _EXPERIMENT_CONFIGURATION_H_
