#ifndef _SERVER_LOGGER_H_
#define _SERVER_LOGGER_H_

#include "comms/connection_event_manager.h"
#include "experiment/configuration.h"
#include "experiment/event_manager.h"
#include "experiment/payout.h"
#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_validity.h"
#include "market/transaction.h"

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace server {

class Logger {
 public:
  Logger(std::ostream& out) : log(out) {}
  void OnExperimentStarted(const experiment::Configuration&) {
    Log("Experiment Started");
  }
  void OnExperimentEnded() { Log("Experiment Ended"); }
  void OnRoundStarted() { Log("Round Started"); }
  void OnRoundEnded() { Log("Round Ended"); }
  void OnRoundResultsStarted() { Log("Round Results Started"); }
  void OnRoundResultsEnded() { Log("Round Results Ended"); }

  void OnBidReceived(market::Bid bid, market::OfferValidity ov);
  void OnAskReceived(market::Ask bid, market::OfferValidity ov);
  void OnOfferRetract(int id);
  void OnTransactions(const std::vector<market::Transaction>& trans);

  void OnPaused() { Log("Experiment Paused"); }
  void OnResumed() { Log("Experiment Resumed"); }
  void OnPaymentsCalculated(const std::vector<experiment::Payout>& payouts) {
    Log("Final Payments Determined");
  }

  void OnConnection(int id) { Log("Connected: " + std::to_string(id)); }
  void OnDisconnection(int id) { Log("Disconnected: " + std::to_string(id)); }

 private:
  void Log(std::string message) { log << message << std::endl; }
  std::ostream& log;
};

void HookupLogger(Logger& log, experiment::EventManager& em);
void HookupLogger(Logger& log, comms::ConnectionEventManager& em);

}  // namespace server

#endif  // _SERVER_LOGGER_H_
