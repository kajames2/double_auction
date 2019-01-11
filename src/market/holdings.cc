#include "market/holdings.h"

namespace market {

Holdings operator+(Holdings h1, Holdings h2) { return h1 += h2; }

Holdings& operator+=(Holdings& h1, Holdings h2) {
  h1.cash += h2.cash;
  h1.units += h2.units;
  return h1;
}

Holdings operator-(Holdings h1, Holdings h2) { return h1 -= h2; }

Holdings& operator-=(Holdings& h1, Holdings h2) {
  h1.cash -= h2.cash;
  h1.units -= h2.units;
  return h1;
}

bool IsValid(Holdings h) { return h.units >= 0 && h.cash >= 0; }
Holdings ToBuyHoldings(Transaction t) {
  return {-t.price * t.quantity, t.quantity};
}
Holdings ToSellHoldings(Transaction t) {
  return {t.price * t.quantity, -t.quantity};
}
Holdings ToHoldings(Bid bid) {
  return {-bid.price * bid.quantity, bid.quantity};
}
Holdings ToHoldings(Ask ask) {
  return {ask.price * ask.quantity, -ask.quantity};
}

std::ostream& operator<<(std::ostream& os, const Holdings& h) {
  return os << h.cash << ' ' << h.units;
}

std::istream& operator>>(std::istream& is, Holdings& h) {
  return is >> h.cash >> h.units;
}

}  // namespace market
