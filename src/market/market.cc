#include "market/market.h"

#include <ostream>
#include <vector>

#include "market/transaction.h"

namespace market {

std::vector<Transaction> Market::Update(double time) {
  auto transactions = auction_.Process(time);
  for (const auto& t : transactions) {
    player_holdings_.at(t.buyer_id) += ToBuyHoldings(t);
    player_holdings_.at(t.seller_id) += ToSellHoldings(t);
  }
  return transactions;
}

template <typename T>
Holdings NetHoldings(const std::vector<T>& offers) {
  Holdings net{0, 0};
  for (const auto& offer : offers) {
    net += ToHoldings(offer);
  }
  return net;
}

bool Market::SufficientCash(Bid bid) const {
  auto holdings = player_holdings_.at(bid.player_id);
  holdings += NetHoldings(auction_.GetBids(bid.player_id));
  holdings += ToHoldings(bid);
  return holdings.cash > 0;
}

bool Market::SufficientUnits(Ask ask) const {
  auto holdings = player_holdings_.at(ask.player_id);
  holdings += NetHoldings(auction_.GetAsks(ask.player_id));
  holdings += ToHoldings(ask);
  return holdings.units > 0;
}

template <typename T>
OfferValidity Market::CheckOffer(T offer) const {
  if (!SufficientCash(offer)) {
    return OfferValidity::kInsufficientCash;
  } else if (!SufficientUnits(offer)) {
    return OfferValidity::kInsufficientUnits;
  } else {
    return auction_.CheckOffer(offer);
  }
}
template OfferValidity Market::CheckOffer<Ask>(Ask ask) const;
template OfferValidity Market::CheckOffer<Bid>(Bid bid) const;

}  // namespace market
