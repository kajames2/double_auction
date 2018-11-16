#include "experiment/experiment.h"

namespace experiment {

void Experiment::SetRound(int n) {
  round_ = n;
  market_ = market::Market(config_.init_holdings[n]);
}

void Experiment::NextRound() { SetRound(round_ + 1); }

void Experiment::AddRoundEarnings() {
  for (auto& [id, player] : players_) {
    auto holdings = market_.GetHoldings(id);
    player.edollars += (config_.utility_funcs.at(id))(holdings);
  }
}

std::vector<market::Transaction> Experiment::GetPeriodTransactions() {
  return market_.GetTransactionHistory();
}

std::vector<RoundResult> Experiment::GetRoundResults() {
  std::vector<RoundResult> results;
  for (const auto& [id, player] : players_) {
    auto holdings = market_.GetHoldings(id);
    int utility = (config_.utility_funcs.at(id))(holdings);
    return results.emplace_back(id, holdings, utility);
  }
}

std::vector<Payout> Experiment::GetPayouts() {
  std::vector<Payout> payouts;
  for (const auto& [id, player] : players_) {
    payouts.push_back(ToPayout(player, config_.eperdollars.at(id)));
  }
  return payouts;
}

}  // namespace experiment
