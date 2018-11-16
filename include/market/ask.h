#ifndef _MARKET_ASK_
#define _MARKET_ASK_

namespace market {

struct Ask {
  int timestamp;
  int id;
  int price;
  int quantity;
};

bool operator==(const Ask& a1, const Ask& a2);

}  // namespace market

#endif // _MARKET_ASK_
