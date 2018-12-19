#ifndef _EXPERIMENT_LOGGER_H_
#define _EXPERIMENT_LOGGER_H_

#include "experiment/configuration.h"
#include "market/bid.h"
#include "market/offer_validity.h"
#include "market/transaction.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace experiment {

class Logger {
 public:
  Logger(std::string filename) : log(filename) {}
  void OnExperimentStarted(const Configuration&) {
    Log("Experiment Started\n");
  }
  void OnExperimentEnded() { Log("Experiment Ended"); }
  void OnRoundStarted() { Log("Round Started"); }
  void OnRoundEnded() { Log("Round Ended"); }
  void OnRoundResultsStarted() { Log("Round Results Started"); }
  void OnRoundResultsEnded() { Log("Round Results Ended"); }
  void OnBidAccepted(market::Bid bid,
                     const std::vector<market::Transaction>& trans) {
    Log("Accepted bid from player " + std::to_string(bid.player_id) + " for " +
        std::to_string(bid.quantity) + " unit(s) at " +
        std::to_string(bid.price) +
        " each.  ID: " + std::to_string(bid.unique_id));
    LogTransactions(trans);
  }

  void OnBidRejected(market::Bid bid, market::OfferValidity ov) {
    std::stringstream s;
    s << "Rejected bid from player " << std::to_string(bid.player_id)
      << ". Reason: " << ov;
    Log(s.str());
  }

  void OnAskAccepted(market::Ask ask,
                     const std::vector<market::Transaction>& trans) {
    Log("Accepted ask from player " + std::to_string(ask.player_id) + " for " +
        std::to_string(ask.quantity) + " unit(s) at " +
        std::to_string(ask.price) +
        " each.  ID: " + std::to_string(ask.unique_id));
    LogTransactions(trans);
  }

  void OnAskRejected(market::Ask ask, market::OfferValidity ov) {
    std::stringstream s;
    s << "Rejected bid from player " << std::to_string(ask.player_id)
      << ". Reason: " << ov;
    Log(s.str());
  }

  void OnPaused() { Log("Experiment Paused"); }
  void OnResumed() { Log("Experiment Resumed"); }
  void OnPaymentsCalculated() { Log("Final Payments Determined"); }

 private:
  void Log(std::string message) { log << message << "\n"; }
  void LogTransactions(const std::vector<market::Transaction>& trans) {
    for (const auto& tran : trans) {
      Log("Player " + std::to_string(tran.buyer_id) + " bought from " +
          std::to_string(tran.seller_id) + ": " +
          std::to_string(tran.quantity) + "unit at a price of " +
          std::to_string(tran.price) + " each.");
    }
  }
  std::ofstream log;
};

}  // namespace experiment

#endif  // _EXPERIMENT_LOGGER_H_
