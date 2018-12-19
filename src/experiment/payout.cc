#include "experiment/payout.h"

namespace experiment {

Payout ToPayout(const Player& p, double eperdollar) {
  int ncents = static_cast<int>(std::round(p.edollars * 100 / eperdollar));
  return Payout{p.id, p.name, ncents};
}

}  // namespace experiment
