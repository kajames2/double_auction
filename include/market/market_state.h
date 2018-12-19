#ifndef _MARKET_MARKET_STATE_
#define _MARKET_MARKET_STATE_

#include <map>
#include <ostream>
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

std::ostream& operator<<(std::ostream& os, const MarketState state);
}  // namespace market

#endif  // _MARKET_MARKET_STATE_
