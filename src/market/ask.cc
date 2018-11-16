#include "market/ask.h"

namespace market {

bool operator==(const Ask& a1, const Ask& a2) {
  return a1.timestamp == a2.timestamp &&
         a1.id == a2.id &&
         a1.price == a2.price &&
         a1.quantity == a2.quantity;
}

}  // namespace market
