#ifndef _EXPERIMENT_OUTPUTTER_H_
#define _EXPERIMENT_OUTPUTTER_H_

#include "experiment/configuration.h"
#include "experiment/controller.h"
#include "experiment/event_manager.h"
#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_validity.h"
#include "market/transaction.h"

#include <ostream>
#include <sstream>
#include <string>
#include <vector>

namespace server {

template <typename T>
std::string OptionalToString(T value) {
  return value == -1 ? "" : std::to_string(value);
}

struct OutputLine {
  std::string event = "";
  int round = -1;
  float time_rem = -1;
  int player_id = -1;
  int cash = -1;
  int units = -1;
  int offer_id = -1;
  int price = -1;
  int quantity = -1;
  int buyer_id = -1;
  int seller_id = -1;
  int utility = -1;
  void Reset();
};

std::string GetOutputLineHeader();
std::ostream& operator<<(std::ostream& os, const OutputLine& line);
class Outputter {
 public:
  Outputter(std::ostream& out, const experiment::Controller& con);
  void OnRoundStarted();
  void OnRoundEnded();

  void OnBidReceived(market::Bid bid, market::OfferValidity ov);
  void OnAskReceived(market::Ask ask, market::OfferValidity ov);
  void OnOfferRetract(int id);
  void OnTransactions(const std::vector<market::Transaction>& trans);

 private:
  void Write(std::string event);
  void WriteHoldings(std::string event);

  template <typename T>
  void WriteOffer(T offer, std::string event);

  OutputLine out_line;
  const experiment::Controller& con_;
  std::ostream& writer;
};

void HookupOutputter(Outputter& writer, experiment::EventManager& em);

}  // namespace server

#endif  // _EXPERIMENT_OUTPUTTER_H_
