#include "experiment/controller.h"

namespace experiment {

std::vector<int> Range(int n) {
  std::vector<int> vec(n);
  std::iota(vec.begin(), vec.end(), 0);
  return vec;
}

Controller::Controller(asio::io_context& io, Configuration config,
                       EventManager em)
    : em_(std::move(em)),
      io_(io),
      config_(config),
      experiment_(config, Range(config.n_players)) {}

void Controller::StartExperiment() {
  experiment_.SetRound(1);
  StartRound();
  Resume();
  em_.exp_start(config_);
}

void Controller::StartRound() {
  experiment_.SetStage(Stage::Round);
  market_ = market::Market(config_.init_holdings[1]);
  timer_ = std::make_unique<market::PausableTimer>(
      io_, config_.round_time, [this](const asio::error_code& e) {
        if (!e) EndRound();
      });
  offer_id_ = 0;
  em_.round_start();
}

void Controller::EndRound() {
  experiment_.AddRoundEarnings(market_.GetAllHoldings());
  experiment_.NextRound();
  StartRound();
  em_.round_end();
}

void Controller::EndExperiment() {
  experiment_.SetStage(Stage::End);
  experiment_.SetStatus(Status::Ended);
  timer_->Reset();
  em_.exp_end();
}

void Controller::Pause() {
  experiment_.SetStatus(Status::Paused);
  timer_->Pause();
  em_.paused();
}

void Controller::Resume() {
  experiment_.SetStatus(Status::Running);
  timer_->Resume();
  em_.resumed();
}

void Controller::ResetRound() { StartRound(); }

ControllerState Controller::GetState() {
  return {experiment_.GetState(), timer_->GetTimeRemaining(),
          market_.GetState()};
}

template <typename T>
market::OfferValidity Controller::TakeOffer(int player_id, int price,
                                            int quantity) {
  double timestamp = timer_->GetTimeElapsed().count();
  int unique_id = offer_id_;
  T offer{unique_id, player_id, timestamp, price, quantity};
  auto validity = market_.CheckOffer(offer);
  if (validity == market::OfferValidity::kValid) {
    auto transactions = market_.AcceptOffer(offer);
    ++offer_id_;    
  }
  return validity;
}

market::OfferValidity Controller::TakeBid(int player_id, int price,
                                          int quantity) {
  return TakeOffer<market::Bid>(player_id, price, quantity);
}

market::OfferValidity Controller::TakeAsk(int player_id, int price,
                                          int quantity) {
  return TakeOffer<market::Ask>(player_id, price, quantity);
}

void Controller::RetractOffer(int unique_id) {
  market_.RetractOffer(unique_id);
}

std::vector<Payout> Controller::GetPayouts() {
  em_.payments();
  return experiment_.GetPayouts();
};

}  // namespace experiment
