#include "experiment/experiment.h"

namespace experiment {

Experiment::Experiment(Configuration config, std::vector<int> player_ids)
    : config_(config),
      round_(0),
      status_(Status::NotStarted),
      stage_(Stage::PreExperiment) {
  for (auto id : player_ids) {
    players_[id] = Player{id, "", 0};
  }
}

void Experiment::SetRound(int n) { round_ = n; }

void Experiment::NextRound() { round_ += 1; }

void Experiment::AddRoundEarnings(std::map<int, market::Holdings> holdings) {
  for (auto& [id, player] : players_) {
    auto h = holdings.at(id);
    player.edollars += (config_.utility_funcs.at(id))(h);
  }
}

std::vector<PlayerRoundResult> Experiment::GetRoundResults(
    std::map<int, market::Holdings> holdings) const {
  std::vector<PlayerRoundResult> results;
  for (const auto& [id, player] : players_) {
    auto h = holdings.at(id);
    int utility = (config_.utility_funcs.at(id))(h);
    results.push_back({id, h, utility});
  }
  return results;
}

std::vector<Payout> Experiment::GetPayouts() {
  std::vector<Payout> payouts;
  for (const auto& [id, player] : players_) {
    payouts.push_back(ToPayout(player, config_.eperdollars.at(id)));
  }
  return payouts;
}

ExperimentState Experiment::GetState() const {
  return ExperimentState{round_, stage_, status_};
}

}  // namespace experiment
