#ifndef _MARKET_BID_
#define _MARKET_BID_

namespace market {

struct Bid {
  int timestamp;
  int id;
  int price;
  int quantity;
};

bool operator==(const Bid& b1, const Bid& b2);

}  // namespace market

#endif // _MARKET_BID_
