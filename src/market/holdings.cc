#include "market/holdings.h"

namespace market {

Holdings operator+(Holdings h1, Holdings h2) {
  return {h1.items + h2.items, h1.cash + h2.cash};
}

Holdings& operator+=(Holdings& h1, Holdings h2) {
  h1.items += h2.items;
  h1.cash += h2.cash;
  return h1;
}

Holdings operator-(Holdings h1, Holdings h2) {
  return {h1.items - h2.items, h1.cash - h2.cash};
}

Holdings& operator-=(Holdings& h1, Holdings h2) {
  h1.items -= h2.items;
  h1.cash -= h2.cash;
  return h1;
}

bool IsValid(Holdings h) { return h.items >= 0 && h.cash >= 0; }
Holdings ToHoldings(Transaction t) { return {t.quantity, -t.price}; }
Holdings ToHoldings(Bid b) { return {b.quantity, -b.price}; }
Holdings ToHoldings(Ask a) { return {-a.quantity, a.price}; }

}  // namespace market
