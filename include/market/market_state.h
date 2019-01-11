#ifndef _MARKET_MARKET_STATE_
#define _MARKET_MARKET_STATE_

#include <map>
#include <iostream>
#include <vector>

#include "market/ask.h"
#include "market/bid.h"
#include "market/holdings.h"
#include "market/offer_queue.h"
#include "market/transaction.h"

namespace market {

struct MarketState {
  std::map<int, Holdings> player_holdings;
  std::vector<Transaction> transaction_history;
  OfferQueue<Bid> bid_queue;
  OfferQueue<Ask> ask_queue;
};

void PrettyPrint(std::ostream& os, const MarketState& state);
std::ostream& operator<<(std::ostream& os, const MarketState& state);
std::istream& operator>>(std::istream& is, MarketState& state);
}  // namespace market

#endif  // _MARKET_MARKET_STATE_
