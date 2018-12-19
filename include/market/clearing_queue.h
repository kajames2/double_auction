#ifndef _MARKET_CLEARING_QUEUE_
#define _MARKET_CLEARING_QUEUE_

#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_queue.h"
#include "market/transaction.h"

#include <chrono>
#include <vector>

namespace market {

class ClearingQueue {
 public:
  ClearingQueue();
  std::vector<Transaction> Process(double time);
  std::vector<Transaction> GetHistory() const { return history_; }

  void RetractOffer(int unique_id) {
    bids_.RetractOffer(unique_id);
    asks_.RetractOffer(unique_id);
  }
  void AddOffer(Bid bid) { bids_.push_back(bid); }
  void AddOffer(Ask ask) { asks_.push_back(ask); }
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
  void SortQueues();
  bool AreTrades() const;

  OfferQueue<Bid> bids_;
  OfferQueue<Ask> asks_;
  std::vector<Transaction> history_;
};

}  // namespace market
#endif  // _MARKET_CLEARING_QUEUE_
