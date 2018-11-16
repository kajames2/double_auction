#include "market/transaction.h"

#include <ostream>

#include "market/bid.h"
#include "market/ask.h"

namespace market {

Transaction MakeTransaction(int time, Bid bid, Ask ask) {
  int price = bid.timestamp < ask.timestamp ? bid.price : ask.price;
  int quantity = std::min(bid.quantity, ask.quantity);
  return Transaction{time, bid.id, ask.id, price, quantity};
}

std::ostream& operator<<(std::ostream& os, Transaction t) {
  return os << t.timestamp << "," << t.buyer_id << "," << t.seller_id << ","
            << t.price << "," << t.quantity;
}

}  // namespace market
