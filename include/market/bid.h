#ifndef _MARKET_BID_
#define _MARKET_BID_

#include <iostream>

namespace market {

struct Bid {
  int unique_id;
  int player_id;
  double timestamp;
  int price;
  int quantity;
};

bool operator==(const Bid& b1, const Bid& b2);
bool operator<(const Bid& b1, const Bid& b2);
bool operator!=(const Bid& b1, const Bid& b2);
bool operator>(const Bid& b1, const Bid& b2);
bool operator<=(const Bid& b1, const Bid& b2);
bool operator>=(const Bid& b1, const Bid& b2);

std::ostream& operator<<(std::ostream& os, const Bid& b);
std::istream& operator>>(std::istream& is, Bid& b);
}  // namespace market

#endif  // _MARKET_BID_
