#ifndef _EXPERIMENT_PAYOUT_H_
#define _EXPERIMENT_PAYOUT_H_

#include <cmath>
#include <string>

#include "experiment/player.h"

namespace experiment {

struct Payout {
  int id;
  std::string name;
  int cents;
};

Payout ToPayout(const Player& p, double eperdollar) {
  int ncents = static_cast<int>(std::round(p.edollars * 100 / eperdollar));
  return Payout{p.id, p.name, ncents};
}

}  //  namespace experiment

#endif  // _EXPERIMENT_PAYOUT_H_
