#include "market/bid.h"

namespace market {

bool operator==(const Bid& b1, const Bid& b2) {
  return b1.timestamp == b2.timestamp &&
         b1.id == b2.id &&
         b1.price == b2.price &&
         b1.quantity == b2.quantity;
}

}  // namespace market
