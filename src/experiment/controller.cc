#include "experiment/controller.h"

#include <vector>
#include <numeric>

#include "market/ask.h"
#include "market/bid.h"

namespace experiment {

std::vector<int> Range(int n) {
  std::vector<int> vec(n);
  std::iota(vec.begin(), vec.end(), 0);
  return vec;
}

bool IsAcceptingOffers(const Experiment& exp) {
  return exp.GetStage() == Stage::Round && exp.GetStatus() == Status::Running;
}

Controller::Controller(asio::io_context& io, Configuration config,
                       EventManager em, std::vector<int> player_ids)
    : em_(std::move(em)),
      io_(io),
      config_(config),
      experiment_(config, player_ids) {}

void Controller::StartExperiment() {
  experiment_.SetRound(0);
  em_.exp_start(config_);
  StartRound();
}

void Controller::StartRound() {
  experiment_.SetStage(Stage::Round);
  experiment_.SetStatus(Status::Running);
  market_ = market::Market(config_.init_holdings[experiment_.GetRound()]);
  timer_ = std::make_unique<tools::PausableTimer>(
      io_, config_.round_time, [this](const std::error_code& e) {
        if (!e) EndRound();
      });
  offer_id_ = 0;
  em_.round_start();
}

void Controller::EndRound() {
  experiment_.AddRoundEarnings(market_.GetAllHoldings());
  em_.round_end();
  if (experiment_.GetRound() == config_.n_rounds - 1) {
    StartPayouts();
  } else {
    StartReview();
  }
}

void Controller::StartReview() {
  timer_ = std::make_unique<tools::PausableTimer>(
      io_, config_.review_time, [this](const std::error_code& e) {
        if (!e) EndReview();
      });
  experiment_.SetStage(Stage::Review);
  em_.review_start();
}

void Controller::EndReview() {
  em_.review_end();
  experiment_.NextRound();
  StartRound();
}

void Controller::StartPayouts() {
  experiment_.SetStage(Stage::Payouts);
  em_.payments(GetPayouts());
};

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

ControllerState Controller::GetState() const {
  return {experiment_.GetState(), timer_->GetTimeRemaining(),
          market_.GetState()};
}

template <typename T>
std::vector<market::Transaction> Controller::TakeOffer(T offer) {
  auto transactions = market_.AcceptOffer(offer);
  ++offer_id_;
  return transactions;
}

market::OfferValidity Controller::TakeBid(int player_id, int price,
                                          int quantity) {
  double timestamp = timer_->GetTimeElapsed().count();
  int unique_id = offer_id_;
  market::Bid offer{unique_id, player_id, timestamp, price, quantity};
  market::OfferValidity validity;
  if (!IsAcceptingOffers(experiment_)) {
    validity = market::OfferValidity::kMarketClosed;
  } else if (config_.player_types.at(player_id) == PlayerType::Seller) {
    validity = market::OfferValidity::kInvalidPlayerTypeOffer;
  } else {
    validity = market_.CheckOffer(offer);
  }
  std::vector<market::Transaction> transactions;
  if (validity == market::OfferValidity::kValid) {
    transactions = TakeOffer(offer);
  }
  em_.bid_receive(offer, validity);
  if (!transactions.empty()) {
    em_.transaction(transactions);
  }
  return validity;
}

market::OfferValidity Controller::TakeAsk(int player_id, int price,
                                          int quantity) {
  double timestamp = timer_->GetTimeElapsed().count();
  int unique_id = offer_id_;
  market::Ask offer{unique_id, player_id, timestamp, price, quantity};
  market::OfferValidity validity;
  if (!IsAcceptingOffers(experiment_)) {
    validity = market::OfferValidity::kMarketClosed;
  } else if (config_.player_types.at(player_id) == PlayerType::Buyer) {
    validity = market::OfferValidity::kInvalidPlayerTypeOffer;
  } else {
    validity = market_.CheckOffer(offer);
  }
  std::vector<market::Transaction> transactions;
  if (validity == market::OfferValidity::kValid) {
    transactions = TakeOffer(offer);
  }
  em_.ask_receive(offer, validity);
  if (!transactions.empty()) {
    em_.transaction(transactions);
  }
  return validity;
}

void Controller::TakeName(int player_id, std::string name) {
  experiment_.SetName(player_id, name);
  em_.name_taken(player_id, name);
}


void Controller::RetractOffer(int unique_id) {
  market_.RetractOffer(unique_id);
  em_.offer_retract(unique_id);
}

std::vector<Payout> Controller::GetPayouts() {
  return experiment_.GetPayouts();
}

}  // namespace experiment
