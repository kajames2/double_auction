#include "market/clearing_queue.h"

#include <algorithm>
#include <chrono>
#include <functional>

#include "market/ask.h"
#include "market/bid.h"
#include "market/transaction.h"

namespace market {

ClearingQueue::ClearingQueue(bool improving, bool auto_replace)
    : improving_(improving), auto_replace_(auto_replace) {}

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

template <typename T>
OfferValidity ClearingQueue::CheckOffer(T offer) const {
  if (improving_ && !Improving(offer)) {
    return OfferValidity::kInvalidUnimproving;
  } else if (TradeWithSelf(offer)) {
    return OfferValidity::kInvalidTradeWithSelf;
  } else {
    return OfferValidity::kValid;
  }
}

template OfferValidity ClearingQueue::CheckOffer<Ask>(Ask ask) const;
template OfferValidity ClearingQueue::CheckOffer<Bid>(Bid bid) const;

bool ClearingQueue::TradeWithSelf(Bid bid) const {
  if (asks_.empty()) return false;
  Ask matched_ask = asks_.GetStandingOffer();
  return (bid.player_id == matched_ask.player_id) &&
         (bid.price > matched_ask.price);
}

bool ClearingQueue::TradeWithSelf(Ask ask) const {
  if (bids_.empty()) return false;
  Bid matched_bid = bids_.GetStandingOffer();
  return (ask.player_id == matched_bid.player_id) &&
         (matched_bid.price > ask.price);
}

bool ClearingQueue::Improving(Ask ask) const {
  return asks_.empty() || (asks_.GetStandingOffer() < ask);
}

bool ClearingQueue::Improving(Bid bid) const {
  return bids_.empty() || (bids_.GetStandingOffer() < bid);
}

}  // namespace market
