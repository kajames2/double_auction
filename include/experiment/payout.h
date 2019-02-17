#ifndef _EXPERIMENT_PAYOUT_H_
#define _EXPERIMENT_PAYOUT_H_

#include <cmath>
#include <string>
#include <ostream>

#include "experiment/player.h"

namespace experiment {

struct Payout {
  int id;
  std::string name;
  int cents;
};

Payout ToPayout(const Player& p, double eperdollar);
std::ostream& operator<<(std::ostream& os, const Payout& p);

}  //  namespace experiment

#endif  // _EXPERIMENT_PAYOUT_H_
