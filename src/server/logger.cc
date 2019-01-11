#include "server/logger.h"

namespace server {

void Logger::OnBidReceived(market::Bid bid, market::OfferValidity ov) {
  std::stringstream s;
  s << "Received bid.  Player ID: " << std::to_string(bid.player_id)
    << " Price: " << std::to_string(bid.price)
    << " Quantity: " << std::to_string(bid.quantity) << ".  Evaluation: " << ov;
  Log(s.str());
}

void Logger::OnAskReceived(market::Ask bid, market::OfferValidity ov) {
  std::stringstream s;
  s << "Received ask.  Player ID: " << bid.player_id << " Price: " << bid.price
    << " Quantity: " << bid.quantity << ".  Evaluation: " << ov;
  Log(s.str());
}

void Logger::OnOfferRetract(int id) {
  std::stringstream s;
  s << "Offer with id: " << id << " was retracted.";
  Log(s.str());
}

void Logger::OnTransactions(const std::vector<market::Transaction>& trans) {
  for (const auto& tran : trans) {
    Log("Player " + std::to_string(tran.buyer_id) + " bought from " +
        std::to_string(tran.seller_id) + ": " + std::to_string(tran.quantity) +
        "unit at a price of " + std::to_string(tran.price) + " each.");
  }
}

void HookupLogger(Logger& log, experiment::EventManager& em) {
  em.exp_start.connect([&log](const experiment::Configuration& c) {
    log.OnExperimentStarted(c);
  });
  em.exp_end.connect([&log]() { log.OnExperimentEnded(); });
  em.round_start.connect([&log]() { log.OnRoundStarted(); });
  em.round_end.connect([&log]() { log.OnRoundEnded(); });
  em.review_start.connect([&log]() { log.OnRoundResultsStarted(); });
  em.review_end.connect([&log]() { log.OnRoundResultsEnded(); });
  em.bid_receive.connect([&log](market::Bid bid, market::OfferValidity ov) {
    log.OnBidReceived(bid, ov);
  });
  em.ask_receive.connect([&log](market::Ask ask, market::OfferValidity ov) {
    log.OnAskReceived(ask, ov);
  });
  em.offer_retract.connect([&log](int id) { log.OnOfferRetract(id); });
  em.transaction.connect([&log](const std::vector<market::Transaction>& trans) {
    log.OnTransactions(trans);
  });

  em.paused.connect([&log]() { log.OnPaused(); });
  em.resumed.connect([&log]() { log.OnResumed(); });
  em.payments.connect([&log](const std::vector<experiment::Payout>& payouts) {
    log.OnPaymentsCalculated(payouts);
  });
}

void HookupLogger(Logger& log, comms::ConnectionEventManager& em) {
  em.connected.connect([&log](int id) { log.OnConnection(id); });
  em.disconnected.connect([&log](int id) { log.OnDisconnection(id); });
}

}  // namespace server
