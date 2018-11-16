#ifndef _MARKET_CLEARING_QUEUE_
#define _MARKET_CLEARING_QUEUE_

#include "market/ask.h"
#include "market/bid.h"
#include "market/transaction.h"

#include <chrono>
#include <vector>

namespace market {

class ClearingQueue {
 public:
  ClearingQueue();
  ClearingQueue(std::chrono::time_point<std::chrono::system_clock> time);
  void AddBid(Bid bid) { bids_.push_back(bid); }
  void AddAsk(Ask ask) { asks_.push_back(ask); }
  void RetractBid(Bid bid);
  void RetractAsk(Ask ask);
  std::vector<Transaction> Process(std::chrono::time_point<std::chrono::system_clock> time);
  std::vector<Transaction> Process();
  std::vector<Bid> GetBids(int id) const;
  std::vector<Ask> GetAsks(int id) const;
  std::vector<Transaction> GetHistory() const { return history_; }

 private:
  void SortQueues();
  bool AreTrades() const;

  std::vector<Bid> bids_;
  std::vector<Ask> asks_;
  std::vector<Transaction> history_;
  std::chrono::time_point<std::chrono::system_clock> market_start;
};

}  // namespace market
#endif  // _MARKET_CLEARING_QUEUE_
