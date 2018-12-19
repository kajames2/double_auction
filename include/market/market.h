#ifndef _MARKET_MARKET_H_
#define _MARKET_MARKET_H_

#include <map>
#include <ostream>

#include "market/clearing_queue.h"
#include "market/holdings.h"
#include "market/market_state.h"
#include "market/offer_validity.h"

namespace market {

class Market {
 public:
  Market() {}
  Market(std::map<int, Holdings> init) : auction_(), player_holdings_(init) {}

  template <typename T>
  std::vector<Transaction> AcceptOffer(T offer) {
    auction_.AddOffer(offer);
    return Update(offer.timestamp);
  }

  template <typename T>
  OfferValidity CheckOffer(T offer) const {
    return market::CheckOffer(offer, GetState());
  }

  void RetractOffer(int unique_id) { auction_.RetractOffer(unique_id); }

  Holdings GetHoldings(int id) const { return player_holdings_.at(id); }
  std::map<int, Holdings> GetAllHoldings() const { return player_holdings_; }
  std::vector<Bid> GetBids(int id) const { return auction_.GetBids(id); }
  std::vector<Ask> GetAsks(int id) const { return auction_.GetAsks(id); }
  std::vector<Transaction> GetTransactionHistory() const {
    return auction_.GetHistory();
  }
  MarketState GetState() const {
    return MarketState{player_holdings_, GetTransactionHistory(),
                       auction_.GetBidQueue(), auction_.GetAskQueue()};
  }

 private:
  std::vector<Transaction> Update(double time);

  market::ClearingQueue auction_;
  std::map<int, Holdings> player_holdings_;
};

}  // namespace market

#endif  // _MARKET_MARKET_H_
