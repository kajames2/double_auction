#include "market/transaction.h"

#include <iostream>

#include "market/ask.h"
#include "market/bid.h"

namespace market {

Transaction MakeTransaction(double time, Bid bid, Ask ask) {
  int price;
  if (bid.timestamp != ask.timestamp) {
    price = bid.timestamp < ask.timestamp ? bid.price : ask.price;
  } else {
    price = bid.unique_id < ask.unique_id ? bid.price : ask.price;
  }
  int quantity = std::min(bid.quantity, ask.quantity);
  return Transaction{time, bid.player_id, ask.player_id, price, quantity};
}

std::ostream& operator<<(std::ostream& os, const Transaction& t) {
  return os << t.buyer_id << " " << t.seller_id << " " << t.price << " "
            << t.quantity << " " << t.timestamp;
}

std::istream& operator>>(std::istream& is, Transaction& t) {
  return is >> t.buyer_id >> t.seller_id >> t.price >> t.quantity >>
         t.timestamp;
}
}  // namespace market
