#ifndef _EXPERIMENT_CONFIGURATION_H_
#define _EXPERIMENT_CONFIGURATION_H_

#include <vector>
#include <map>
#include <functional>

#include "market/holdings.h"

namespace experiment {

struct Configuration {
  std::vector<market::Holdings> init_holdings;
  std::map<int, std::function<int(market::Holdings)>> utility_funcs;
  std::map<int, double> eperdollars;
};

}  // namespace experiment

#endif  // _EXPERIMENT_CONFIGURATION_H_
