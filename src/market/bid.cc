#include "market/bid.h"

#include <tuple>

namespace market {

bool operator==(const Bid& b1, const Bid& b2) {
  return b1.unique_id == b2.unique_id;
}

bool operator<(const Bid& b1, const Bid& b2) {
  if (b1.price != b2.price) return b1.price < b2.price;
  return b1.unique_id > b2.unique_id;
}

bool operator!=(const Bid& b1, const Bid& b2) { return !(b1 == b2); }
bool operator>(const Bid& b1, const Bid& b2) { return b2 < b1; }
bool operator<=(const Bid& b1, const Bid& b2) { return !(b2 < b1); }
bool operator>=(const Bid& b1, const Bid& b2) { return !(b1 < b2); }

std::ostream& operator<<(std::ostream& os, const Bid& b) {
  return os << b.unique_id << ' ' << b.player_id << ' ' << b.price << ' '
            << b.quantity << ' ' << b.timestamp;
}

std::istream& operator>>(std::istream& is, Bid& b) {
  return is >> b.unique_id >> b.player_id >> b.price >> b.quantity >>
         b.timestamp;
}

}  // namespace market
