#ifndef _CLIENT_SERVER_MESSAGE_PROCESSOR_H_
#define _CLIENT_SERVER_MESSAGE_PROCESSOR_H_

#include <sstream>
#include <string>

#include "market/offer_validity.h"
#include "client/event_manager.h"
#include "client/state.h"
#include "experiment/experiment_enums.h"

namespace client {

class ServerMessageProcessor {
 public:
  ServerMessageProcessor(State& state, EventManager em)
      : state_(state), em_(std::move(em)) {}

  EventManager& GetEventManager() { return em_; }

  void OnReceived(std::string message) {
    std::string type;
    std::stringstream ss(message);
    ss >> type;
    ss.ignore();
    if (type == "EXPSTART") {
      ss >> state_.id >> state_.type;
      ss.ignore();
      em_.exp_start();
    } else if (type == "EXPENDED") {
      em_.exp_end();
    } else if (type == "ROUNDSTARTED") {
      em_.round_start();
    } else if (type == "ROUNDENDED") {
      em_.round_end();
    } else if (type == "REVIEWSTARTED") {
      em_.review_start();
    } else if (type == "REVIEWENDED") {
      em_.review_end();
    } else if (type == "BIDRECEIVED") {
      market::OfferValidity ov;
      ss >> ov; ss.ignore();
      em_.bid_received(ov);
    } else if (type == "ASKRECEIVED") {
      market::OfferValidity ov;
      ss >> ov; ss.ignore();
      em_.ask_received(ov);
    } else if (type == "PAUSED") {
      em_.paused();
    } else if (type == "RESUMED") {
      em_.resumed();
    } else if (type == "PAYMENT") {
      ss >> state_.cents_earnings; ss.ignore();
      em_.payments();
    } else if (type == "STATE") {
      ss >> state_.exp_state; ss.ignore();
      double time_rem;
      ss >> time_rem; ss.ignore();
      state_.time_remaining = std::chrono::duration<float>(time_rem);
      ss >> state_.market_state;
      em_.updated();
    } else if (type == "CONNECTED") {
      ss >> state_.id; ss.ignore();
    }
  }

 private:
  State& state_;
  EventManager em_;
};

}  // namespace client

#endif  // _CLIENT_SERVER_MESSAGE_PROCESSOR_H_
