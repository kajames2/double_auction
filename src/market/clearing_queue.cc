#include "market/clearing_queue.h"

#include <algorithm>
#include <chrono>
#include <functional>

#include "market/ask.h"
#include "market/bid.h"
#include "market/transaction.h"

namespace market {

ClearingQueue::ClearingQueue() {}

std::vector<Transaction> ClearingQueue::Process(double time) {
  std::vector<Transaction> processed;
  while (AreTrades()) {
    Bid standing_bid = bids_.GetStandingOffer();
    Ask standing_ask = asks_.GetStandingOffer();
    bids_.pop_back();
    asks_.pop_back();
    auto transaction = MakeTransaction(time, standing_bid, standing_ask);
    processed.push_back(transaction);
    history_.push_back(transaction);

    int residual_units = standing_bid.quantity - standing_ask.quantity;
    if (residual_units > 0) {
      standing_bid.quantity = residual_units;
      bids_.push_back(standing_bid);
    } else if (residual_units < 0) {
      standing_ask.quantity = -residual_units;
      asks_.push_back(standing_ask);
    }
  }
  return processed;
}

bool ClearingQueue::AreTrades() const {
  if (BidQueueEmpty() || AskQueueEmpty()) {
    return false;
  }
  return GetStandingAsk().price <= GetStandingBid().price;
}

}  // namespace market
