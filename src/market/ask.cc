#include "market/ask.h"

#include <tuple>

namespace market {

bool operator==(const Ask& a1, const Ask& a2) {
  return a1.unique_id == a2.unique_id;
}

bool operator<(const Ask& a1, const Ask& a2) {
  if (a1.price != a2.price) return a1.price > a2.price;
  return a1.unique_id > a2.unique_id;
}

bool operator!=(const Ask& a1, const Ask& a2) { return !(a1 == a2); }
bool operator>(const Ask& a1, const Ask& a2) { return a2 < a1; }
bool operator<=(const Ask& a1, const Ask& a2) { return !(a2 < a1); }
bool operator>=(const Ask& a1, const Ask& a2) { return !(a1 < a2); }

std::ostream& operator<<(std::ostream& os, const Ask& a) {
  return os << a.unique_id << '\t' << a.player_id << '\t' << a.price << '\t'
            << a.quantity << '\t' << a.timestamp;
}

std::istream& operator>>(std::istream& is, Ask& a) {
  return is >> a.unique_id >> a.player_id >> a.price >> a.quantity >>
         a.timestamp;
}
}  // namespace market
