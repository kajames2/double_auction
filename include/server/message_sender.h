#ifndef _SERVER_MESSAGE_SENDER_H_
#define _SERVER_MESSAGE_SENDER_H_

#include <sstream>
#include <string>

#include "comms/connection_manager.h"
#include "experiment/configuration.h"
#include "experiment/controller.h"
#include "experiment/payout.h"
#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_validity.h"

namespace server {

class MessageSender {
 public:
  MessageSender(const experiment::Controller& con, comms::ConnectionManager& cm);
  void OnExperimentStarted(const experiment::Configuration& c);
  void OnExperimentEnded();
  void OnRoundStarted();
  void OnRoundEnded();
  void OnReviewStarted();
  void OnReviewEnded();
  void OnBidReceived(market::Bid bid, market::OfferValidity ov);
  void OnAskReceived(market::Ask ask, market::OfferValidity ov);
  void OnOfferRetract(int id);
  void OnPaused();
  void OnResumed();
  void OnPaymentsCalculated(const std::vector<experiment::Payout>& payouts);
  void OnConnection(int id);
  
 private:
  void SendState();
  void SendExpStart(int id, const experiment::Configuration& c);

  const experiment::Controller& con_;
  comms::ConnectionManager& cm_;
  bool is_started_ = false;
};

void HookupMessageSender(MessageSender& mess, experiment::EventManager& em);
void HookupMessageSender(MessageSender& mess,
                         comms::ConnectionEventManager& em);

}  // namespace server

#endif  // _SERVER_MESSAGE_SENDER_H_
