#include "market/offer_validity.h"

#include <iostream>

namespace market {

template <typename T>
bool InPriceRange(T offer) {
  return offer.price < 1000;
}

bool Improving(Ask ask, const MarketState& state) {
  return state.ask_queue.empty() || (state.ask_queue.GetStandingOffer() < ask);
}

bool Improving(Bid bid, const MarketState& state) {
  return state.bid_queue.empty() || (state.bid_queue.GetStandingOffer() < bid);
}

template <typename T>
Holdings NetHoldings(const std::vector<T>& offers) {
  Holdings net;
  for (const auto& offer : offers) {
    net += ToHoldings(offer);
  }
  return net;
}

bool SufficientCash(Ask ask, const MarketState& state) { return true; }
bool SufficientUnits(Bid bid, const MarketState& state) { return true; }

bool SufficientCash(Bid bid, const MarketState& state) {
  auto holdings = state.player_holdings.at(bid.player_id);
  holdings += NetHoldings(state.bid_queue.GetOffers(bid.player_id));
  holdings += ToHoldings(bid);
  return holdings.cash > 0;
}

bool SufficientUnits(Ask ask, const MarketState& state) {
  auto holdings = state.player_holdings.at(ask.player_id);
  holdings += NetHoldings(state.ask_queue.GetOffers(ask.player_id));
  holdings += ToHoldings(ask);
  return holdings.units > 0;
}

bool TradeWithSelf(Bid bid, const MarketState& state) {
  if (state.ask_queue.empty()) return false;
  Ask matched_ask = state.ask_queue.GetStandingOffer();
  return (bid.player_id == matched_ask.player_id) &&
      (bid.price > matched_ask.price);
}

bool TradeWithSelf(Ask ask, const MarketState& state) {
  if (state.bid_queue.empty()) return false;
  Bid matched_bid = state.bid_queue.GetStandingOffer();
  return (ask.player_id == matched_bid.player_id) &&
      (matched_bid.price > ask.price);
}

template <typename T>
OfferValidity CheckOffer(T offer, const MarketState& state) {
  if (!InPriceRange(offer)) {
    return OfferValidity::kCashChangeOutOfRange;
  } else if (!Improving(offer, state)) {
    return OfferValidity::kInvalidUnimproving;
  } else if (!SufficientCash(offer, state)) {
    return OfferValidity::kInsufficientCash;
  } else if (!SufficientUnits(offer, state)) {
    return OfferValidity::kInsufficientUnits;
  } else if (TradeWithSelf(offer, state)) {
    return OfferValidity::kInvalidTradeWithSelf;
  } else {
    return OfferValidity::kValid;
  }
}
template OfferValidity CheckOffer<Ask>(Ask ask, const MarketState& state);
template OfferValidity CheckOffer<Bid>(Bid bid, const MarketState& state);


std::ostream& operator<<(std::ostream& os, const OfferValidity av) {
  switch (av) {
    case OfferValidity::kValid:
      return os;
    case OfferValidity::kInvalidNoTradeoff:
      return os << "Must be a either a bid or an ask.";
    case OfferValidity::kInsufficientUnits:
      return os << "You do not have enough units.";
    case OfferValidity::kInsufficientCash:
      return os << "You do not have enough Cash.";
    case OfferValidity::kInvalidUnimproving:
      return os << "Does not beat the current standing offer.";
    case OfferValidity::kInvalidTradeWithSelf:
      return os << "Offer would result in a trade with yourself.";
    case OfferValidity::kCashChangeOutOfRange:
      return os << "Offer is outside acceptable range (0 to 999).";
  }
}

}  // namespace market
