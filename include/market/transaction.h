#ifndef _MARKET_TRANSACTION_H_
#define _MARKET_TRANSACTION_H_

#include <ostream>
#include <cmath>

struct Bid;
struct Ask;

namespace market {

struct Transaction {
  int timestamp;
  int buyer_id;
  int seller_id;
  int price;
  int quantity;
};

Transaction MakeTransaction(int time, Bid bid, Ask ask);
std::ostream& operator<<(std::ostream& os, Transaction t);

}  // namespace market

#endif  // _MARKET_TRANSACTION_H_
