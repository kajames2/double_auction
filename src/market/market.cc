#include "market/market.h"

#include <vector>

#include "market/ask.h"
#include "market/bid.h"
#include "market/transaction.h"

namespace market {

std::vector<market::Transaction> Market::Update() {
  auto transactions = auction_.Process();
  for (const auto& t : transactions) {
    Holdings change = ToHoldings(t);
    player_holdings_.at(t.buyer_id) += change;
    player_holdings_.at(t.seller_id) -= change;
  }
  return transactions;
}

std::vector<Transaction> Market::AcceptBid(market::Bid bid) {
  auction_.AddBid(bid);
  return Update();
}
std::vector<Transaction> Market::AcceptAsk(market::Ask ask) {
  auction_.AddAsk(ask);
  return Update();
}

bool IsValid(const Market& m, Bid bid) {
  auto other_bids = m.GetBids(bid.id);
  return IsValid(m, other_bids, bid);
}

bool IsValid(const Market& m, Ask ask) {
  auto other_asks = m.GetAsks(ask.id);
  return IsValid(m, other_asks, ask);
}

}  // namespace market
