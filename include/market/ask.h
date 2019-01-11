#ifndef _MARKET_ASK_
#define _MARKET_ASK_

#include <iostream>

namespace market {

struct Ask {
  int unique_id;
  int player_id;
  double timestamp;
  int price;
  int quantity;
};

bool operator==(const Ask& a1, const Ask& a2);
bool operator<(const Ask& a1, const Ask& a2);
bool operator!=(const Ask& a1, const Ask& a2);
bool operator>(const Ask& a1, const Ask& a2);
bool operator<=(const Ask& a1, const Ask& a2);
bool operator>=(const Ask& a1, const Ask& a2);

std::ostream& operator<<(std::ostream& os, const Ask& a);
std::istream& operator>>(std::istream& is, Ask& a);
}  // namespace market

#endif  // _MARKET_ASK_
