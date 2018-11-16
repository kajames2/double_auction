#ifndef _EXPERIMENT_EXPERIMENT_H_
#define _EXPERIMENT_EXPERIMENT_H_

#include <cmath>

#include "experiment/payout.h"
#include "experiment/player.h"
#include "experiment/round_result.h"
#include "experiment/configuration.h"
#include "market/market.h"

namespace experiment {

class Experiment {
 public:
  Experiment(Configuration config_, std::vector<int> player_ids);
  void SetRound(int n);
  void NextRound();
  void AddRoundEarnings();
  std::vector<market::Transaction> GetPeriodTransactions();
  std::vector<RoundResult> GetRoundResults();
  std::vector<Payout> GetPayouts();

 private:
  std::map<int, Player> players_;
  market::Market market_;
  int round_;
  Configuration config_;
};

}  //  namespace experiment

#endif  // _EXPERIMENT_EXPERIMENT_H_
