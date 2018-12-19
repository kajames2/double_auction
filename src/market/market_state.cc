#include "market/market_state.h"

#include <ostream>

namespace market {

std::ostream& operator<<(std::ostream& os, const MarketState state) {
  os << "Player Holdings:" << '\n';
  for (auto pair : state.player_holdings) {
    os << pair.first << '\t' << pair.second << std::endl;
  }
  os << std::endl << "Transactions:" << std::endl;
  os << "buyer\tseller\tprice\tquant\ttime\n";
  for (auto t : state.transaction_history) {
    os << t << std::endl;
  }
  os << std::endl << "Ask Queue:" << std::endl;
  os << "uid\tpid\tprice\tquant\ttime\n";
  for (auto a : state.ask_queue) {
    os << a << std::endl;
  }
  os << std::endl << "Bid Queue:" << std::endl;
  os << "uid\tpid\tprice\tquant\ttime\n";

  auto end = state.bid_queue.rend();
  for (auto b = state.bid_queue.rbegin(); b != end; ++b) {
    os << *b << std::endl;
  }
  return os;
}

}  // namespace market
