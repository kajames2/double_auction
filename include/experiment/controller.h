#ifndef _EXPERIMENT_CONTROLLER_H_
#define _EXPERIMENT_CONTROLLER_H_

#include <memory>
#include <vector>

#include <asio.hpp>

#include "experiment.h"
#include "experiment/configuration.h"
#include "experiment/controller_state.h"
#include "experiment/event_manager.h"
#include "experiment/player_round_result.h"
#include "market/market.h"
#include "market/offer_validity.h"
#include "tools/pausable_timer.h"

namespace experiment {

class Controller {
 public:
  Controller(asio::io_context& io, Configuration config, EventManager em,
             std::vector<int> player_ids);
  void StartExperiment();
  void EndExperiment();
  void StartRound();
  void EndRound();
  void StartReview();
  void EndReview();
  void StartPayouts();
  void Pause();
  void Resume();
  void ResetRound();

  std::chrono::duration<float> GetTimeRemaining() const {
    return timer_->GetTimeRemaining();
  }
  std::chrono::duration<float> GetTimeElapsed() const {
    return timer_->GetTimeElapsed();
  }
  experiment::ExperimentState GetExperimentState() const {
    return experiment_.GetState();
  }
  market::MarketState GetMarketState() const { return market_.GetState(); }
  ControllerState GetState() const;
  std::vector<experiment::PlayerRoundResult> GetCurrentResults() const {
    return experiment_.GetRoundResults(market_.GetAllHoldings());
  }
  market::OfferValidity TakeBid(int player_id, int price, int quantity);
  market::OfferValidity TakeAsk(int player_id, int price, int quantity);
  void RetractOffer(int unique_id);
  void TakeName(int player_id, std::string name);
  std::vector<Payout> GetPayouts();
  EventManager& GetEventManager() { return em_; }
  const Configuration& GetConfiguration() const { return config_; }

 private:
  EventManager em_;
  int offer_id_;
  market::Market market_;
  Experiment experiment_;
  Configuration config_;
  std::unique_ptr<tools::PausableTimer> timer_ = nullptr;
  asio::io_context& io_;

  template <typename T>
  std::vector<market::Transaction> TakeOffer(T offer);
};

}  // namespace experiment

#endif  // _EXPERIMENT_CONTROLLER_H_
