#ifndef _MARKET_HOLDINGS_H_
#define _MARKET_HOLDINGS_H_

#include <ostream>

#include "market/transaction.h"
#include "market/bid.h"
#include "market/ask.h"

namespace market {

struct Holdings {
  int cash;
  int units;
};

Holdings operator+(Holdings h1, Holdings h2);
Holdings& operator+=(Holdings& h1, Holdings h2);
Holdings operator-(Holdings h1, Holdings h2);
Holdings& operator-=(Holdings& h1, Holdings h2);

bool IsValid(Holdings h);
Holdings ToBuyHoldings(Transaction t);
Holdings ToSellHoldings(Transaction t);
Holdings ToHoldings(Bid bid);
Holdings ToHoldings(Ask ask);

std::ostream& operator<<(std::ostream& os, const Holdings h);
}  // namespace market

#endif  // _MARKET_HOLDINGS_H_
