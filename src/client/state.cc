#include "client/state.h"

namespace client {

std::ostream& operator<<(std::ostream& os, const State& s) {
  os << "My ID: " << s.id << '\n';
  os << "Type: " << s.type << '\n';
  os << "Round: " << s.exp_state.round << '\n';
  os << s.exp_state.stage << ' ' << s.exp_state.status << '\n';
  os << "Seconds remaining: " << s.time_remaining.count() << "\n";

  os << "\nMy Holdings\n";
  os << "Cash: " << s.market_state.player_holdings.at(s.id).cash << '\n';
  os << "Units: " << s.market_state.player_holdings.at(s.id).units << '\n';

  os << "\nTransactions:" << '\n';
  os << "buyer\tseller\tprice\tquant\ttime\n";
  for (const auto& t : s.market_state.transaction_history) {
    os << t.buyer_id << '\t' << t.seller_id << '\t' << t.price << '\t'
       << t.quantity << '\t' << t.timestamp << '\n';
  }

  os << "\nAsk Queue:" << '\n';
  os << "uid\tpid\tprice\tquant\ttime\n";
  for (const auto& a : s.market_state.ask_queue) {
    os << a.unique_id << '\t' << a.player_id << '\t' << a.price << '\t'
       << a.quantity << '\t' << a.timestamp << '\n';
  }

  os << "\nBid Queue:" << '\n';
  os << "uid\tpid\tprice\tquant\ttime\n";
  for (auto b = s.market_state.bid_queue.rbegin();
       b != s.market_state.bid_queue.rend(); ++b) {
    os << b->unique_id << '\t' << b->player_id << '\t' << b->price << '\t'
       << b->quantity << '\t' << b->timestamp << '\n';
  }
  os << std::flush;
  return os;
}

}  // namespace client
