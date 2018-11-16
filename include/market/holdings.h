#ifndef _MARKET_HOLDINGS_H_
#define _MARKET_HOLDINGS_H_

#include "market/transaction.h"
#include "market/bid.h"
#include "market/ask.h"

namespace market {

struct Holdings {
  int items;
  int cash;
};

Holdings operator+(Holdings h1, Holdings h2);
Holdings& operator+=(Holdings& h1, Holdings h2);
Holdings operator-(Holdings h1, Holdings h2);
Holdings& operator-=(Holdings& h1, Holdings h2);

bool IsValid(Holdings h);
Holdings ToHoldings(Transaction t);
Holdings ToHoldings(Bid b);
Holdings ToHoldings(Ask a);
}  // namespace market

#endif  // _MARKET_HOLDINGS_H_
