#ifndef _CLIENT_COMMAND_LINE_OUTPUTTER_H_
#define _CLIENT_COMMAND_LINE_OUTPUTTER_H_

#include <iostream>

#include "client/event_manager.h"
#include "client/state.h"
#include "market/offer_validity.h"

namespace client {

class CommandLineOutputter {
 public:
  CommandLineOutputter(const State& state) : state_(state) {}
  void OnExperimentStart() {
    std::cout << "The experiment has begun." << std::endl;
  }
  void OnExperimentEnd() {
    std::cout << "\n\nThe experiment has ended." << std::endl;
  }
  void OnRoundStart() { std::cout << "The round has begun." << std::endl; }
  void OnRoundEnd() { std::cout << "\nThe round has ended." << std::endl; }
  void OnReviewStart() { std::cout << "Review Period has begin." << std::endl; }
  void OnReviewEnd() { std::cout << "\nReview Period has ended." << std::endl; }
  void OnBidReceived(market::OfferValidity ov) { PrintOffer(ov); }
  void OnAskReceived(market::OfferValidity ov) { PrintOffer(ov); }
  void OnPaused() { std::cout << "\nExperiment Paused" << std::endl; }
  void OnResumed() { std::cout << "\nExperiment Resumed" << std::endl; }
  void OnPayments() {
    std::cout << "You earned: " << (state_.cents_earnings / 100.0) << std::endl;
    std::cout << "Please enter your name." << std::endl;
  }
  void OnUpdate() { std::cout << state_; }

 private:
  void PrintOffer(market::OfferValidity ov) {
    if (ov == market::OfferValidity::kValid) {
      std::cout << "Offer Accepted" << std::endl;
    } else {
      std::cout << "Offer Rejected.  Reason: " << ov << std::endl;
    }
  }
  const State& state_;
};

void HookupCommandLineOutputter(CommandLineOutputter& clo, EventManager& em);

}  // namespace client

#endif  // _CLIENT_COMMAND_LINE_OUTPUTTER_H_
