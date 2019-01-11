#ifndef _EXPERIMENT_EXPERIMENT_H_
#define _EXPERIMENT_EXPERIMENT_H_

#include <map>
#include <vector>

#include "experiment/configuration.h"
#include "experiment/experiment_enums.h"
#include "experiment/experiment_state.h"
#include "experiment/payout.h"
#include "experiment/player.h"
#include "experiment/player_round_result.h"

namespace experiment {

class Experiment {
 public:
  Experiment(Configuration config_, std::vector<int> player_ids);
  void SetRound(int n);
  void NextRound();
  void AddRoundEarnings(std::map<int, market::Holdings>);
  std::vector<PlayerRoundResult> GetRoundResults(
      std::map<int, market::Holdings> holdings) const;
  std::vector<Payout> GetPayouts();
  ExperimentState GetState() const;
  void SetStatus(Status status) { status_ = status; }
  void SetStage(Stage stage) { stage_ = stage; }
  Status GetStatus() const { return status_; }
  Stage GetStage() const { return stage_; }
  int GetRound() const { return round_; }
  void SetName(int id, std::string name) { players_.at(id).name = name; }

 private:
  int round_;
  Status status_;
  Stage stage_;
  std::map<int, Player> players_;
  Configuration config_;
};

}  //  namespace experiment

#endif  // _EXPERIMENT_EXPERIMENT_H_
