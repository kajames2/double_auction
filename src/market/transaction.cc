#include "market/transaction.h"

#include <ostream>

#include "market/bid.h"
#include "market/ask.h"

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

std::ostream& operator<<(std::ostream& os, const Transaction t) {
  return os << t.buyer_id << "\t" << t.seller_id << "\t" << t.price << "\t"
            << t.quantity << "\t" << t.timestamp;
}

}  // namespace market
