#ifndef _EXPERIMENT_PLAYER_H_
#define _EXPERIMENT_PLAYER_H_

#include <string>
#include <cmath>

#include "experiment/payout.h"

namespace experiment {

struct Player {
  int id;
  std::string name;
  int edollars;
};

}  //  namespace experiment

#endif  // _EXPERIMENT_PLAYER_H_
