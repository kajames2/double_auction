#ifndef _MARKET_MARKET_H_
#define _MARKET_MARKET_H_

#include <map>

#include "market/ask.h"
#include "market/bid.h"
#include "market/clearing_queue.h"
#include "market/holdings.h"

namespace market {

class Market {
 public:
  Market(std::map<int, Holdings> init) : auction_(), player_holdings_(init) {}

  std::vector<Transaction> AcceptBid(market::Bid bid);
  std::vector<Transaction> AcceptAsk(market::Ask ask);
  Holdings GetHoldings(int id) const { return player_holdings_.at(id); }
  std::map<int, Holdings> GetAllHoldings() const { return player_holdings_; }
  std::vector<Bid> GetBids(int id) const { return auction_.GetBids(id); }
  std::vector<Ask> GetAsks(int id) const { return auction_.GetAsks(id); }
  std::vector<Transaction> GetTransactionHistory() const {
    return auction_.GetHistory();
  }

 private:
  std::vector<market::Transaction> Update();
  market::ClearingQueue auction_;
  std::map<int, Holdings> player_holdings_;
};

template <typename T> bool IsValid(const Market& m, std::vector<T> current_subs, T sub) {
  auto holdings = m.GetHoldings(sub.id);
  for (auto b : current_subs) {
    holdings += ToHoldings(b);
  }
  holdings += ToHoldings(sub);
  return IsValid(holdings);
}

bool IsValid(const Market& m, Bid bid);
bool IsValid(const Market& m, Ask ask);

}  // namespace market

#endif  // _MARKET_MARKET_H_
