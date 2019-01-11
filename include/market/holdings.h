#ifndef _MARKET_HOLDINGS_H_
#define _MARKET_HOLDINGS_H_

#include <iostream>

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

std::ostream& operator<<(std::ostream& os, const Holdings& h);
std::istream& operator>>(std::istream& is, Holdings& h);
}  // namespace market

#endif  // _MARKET_HOLDINGS_H_
