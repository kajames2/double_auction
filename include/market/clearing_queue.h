#ifndef _MARKET_CLEARING_QUEUE_
#define _MARKET_CLEARING_QUEUE_

#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_queue.h"
#include "market/offer_validity.h"
#include "market/transaction.h"

#include <cassert>
#include <chrono>
#include <vector>

namespace market {

class ClearingQueue {
 public:
  // Booleans for whether bids must be improving, and whether new bids replace
  // old ones.
  ClearingQueue(bool improving, bool auto_replace);
  std::vector<Transaction> Process(double time);
  std::vector<Transaction> GetHistory() const { return history_; }

  template <typename T>
  OfferValidity CheckOffer(T offer) const;

  void RetractOffer(int unique_id) {
    bids_.RetractOffer(unique_id);
    asks_.RetractOffer(unique_id);
  }

  void AddOffer(Bid bid) {
    if (auto_replace_) {
      auto cur_bids = GetBids(bid.player_id);
      assert(cur_bids.size() <= 1);
      for (auto b : cur_bids) {
        RetractOffer(b.unique_id);
      }
    }
    bids_.push_back(bid);
  }

  void AddOffer(Ask ask) {
    if (auto_replace_) {
      auto cur_asks = GetAsks(ask.player_id);
      assert(cur_asks.size() <= 1);
      for (auto a : cur_asks) {
        RetractOffer(a.unique_id);
      }
    }
    asks_.push_back(ask);
  }
  
  std::vector<Bid> GetBids(int player_id) const {
    return bids_.GetOffers(player_id);
  }
  std::vector<Ask> GetAsks(int player_id) const {
    return asks_.GetOffers(player_id);
  }

  OfferQueue<Bid> GetBidQueue() const { return bids_; }
  OfferQueue<Ask> GetAskQueue() const { return asks_; }
  Bid GetStandingBid() const { return bids_.GetStandingOffer(); }
  Ask GetStandingAsk() const { return asks_.GetStandingOffer(); }
  bool BidQueueEmpty() const { return bids_.empty(); }
  bool AskQueueEmpty() const { return asks_.empty(); }

 private:
  bool TradeWithSelf(Bid bid) const;
  bool TradeWithSelf(Ask ask) const;
  bool Improving(Ask ask) const;
  bool Improving(Bid bid) const;

  void SortQueues();
  bool AreTrades() const;

  bool improving_;
  bool auto_replace_;
  OfferQueue<Bid> bids_;
  OfferQueue<Ask> asks_;
  std::vector<Transaction> history_;
};

}  // namespace market
#endif  // _MARKET_CLEARING_QUEUE_
