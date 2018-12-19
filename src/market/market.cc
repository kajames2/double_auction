#include "market/market.h"

#include <ostream>
#include <vector>

#include "market/offer.h"
#include "market/transaction.h"

namespace market {

std::vector<Transaction> Market::Update(double time) {
  auto transactions = auction_.Process(time);
  for (const auto& t : transactions) {
    player_holdings_.at(t.buyer_id) += ToBuyHoldings(t);
    player_holdings_.at(t.seller_id) += ToSellHoldings(t);
  }
  return transactions;
}

}  // namespace market
