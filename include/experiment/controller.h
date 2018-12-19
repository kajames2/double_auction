#ifndef _EXPERIMENT_CONTROLLER_H_
#define _EXPERIMENT_CONTROLLER_H_

#include <memory>
#include <numeric>

#include <asio.hpp>

#include "experiment.h"
#include "experiment/configuration.h"
#include "experiment/controller_state.h"
#include "experiment/event_manager.h"
#include "market/ask.h"
#include "market/bid.h"
#include "market/market.h"
#include "market/offer_validity.h"
#include "market/pausable_timer.h"

namespace experiment {

class Controller {
 public:
  Controller(asio::io_context& io, Configuration config, EventManager em);
  void StartExperiment();
  void StartRound();
  void EndRound();
  void EndExperiment();
  void Pause();
  void Resume();
  void ResetRound();
  ControllerState GetState();
  market::OfferValidity TakeBid(int player_id, int price, int quantity);
  market::OfferValidity TakeAsk(int player_id, int price, int quantity);
  void RetractOffer(int unique_id);
  std::vector<Payout> GetPayouts();

 private:
  EventManager em_;
  int offer_id_;
  market::Market market_;
  Experiment experiment_;
  Configuration config_;
  std::unique_ptr<market::PausableTimer> timer_ = nullptr;
  asio::io_context& io_;

  template <typename T>
  market::OfferValidity TakeOffer(int player_id, int price, int quantity);
};

}  // namespace experiment

#endif  // _EXPERIMENT_CONTROLLER_H_
