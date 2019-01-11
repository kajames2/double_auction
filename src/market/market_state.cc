#include "market/market_state.h"

#include <iostream>

namespace market {

void PrettyPrint(std::ostream& os, const MarketState& state) {
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
}

std::ostream& operator<<(std::ostream& os, const MarketState& ms) {
  os << ms.player_holdings.size() << '\n';
  for (auto pair : ms.player_holdings) {
    os << pair.first << ' ' << pair.second << '\n';
  }
  
  os << ms.transaction_history.size() << '\n';
  for (auto t : ms.transaction_history) {
    os << t << '\n';
  }
  
  os << ms.ask_queue.size() << '\n';
  for (auto a : ms.ask_queue) {
    os << a << '\n';
  }

  os << ms.bid_queue.size() << '\n';
  for (auto b : ms.bid_queue) {
    os << b << '\n';
  }
  return os;
}

std::istream& operator>>(std::istream& is, MarketState& ms) {
  int n_holdings, n_transactions, n_asks, n_bids;
  ms.player_holdings.clear();
  ms.transaction_history.clear();
  ms.ask_queue.clear();
  ms.bid_queue.clear();
  
  is >> n_holdings; is.ignore();
  for (int i = 0; i < n_holdings; ++i) {
    int id;
    Holdings h;
    is >> id >> h; is.ignore();
    ms.player_holdings[id] = h;
  }
  
  is >> n_transactions; is.ignore();
  for (int i = 0; i < n_transactions; ++i) {
    Transaction t;
    is >> t; is.ignore();
    ms.transaction_history.push_back(t);
  }

  is >> n_asks; is.ignore();
  for (int i = 0; i < n_asks; ++i) {
    Ask a;
    is >> a; is.ignore();
    ms.ask_queue.push_back(a);
  }
  
  is >> n_bids; is.ignore();
  for (int i = 0; i < n_bids; ++i) {
    Bid b;
    is >> b; is.ignore();
    ms.bid_queue.push_back(b);
  }
  return is;
}

}  // namespace market
