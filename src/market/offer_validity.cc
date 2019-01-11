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
  Holdings net{0,0};
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
    return OfferValidity::kPriceOutOfRange;
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

std::string ToString(OfferValidity ov) {
  switch (ov) {
    case OfferValidity::kValid:
      return "Valid";
    case OfferValidity::kInvalidNoTradeoff:
      return "InvalidNoTradeoff";
    case OfferValidity::kInsufficientUnits:
      return "InsufficientUnits";
    case OfferValidity::kInsufficientCash:
      return "InsufficientCash";
    case OfferValidity::kInvalidUnimproving:
      return "InvalidUnimproving";
    case OfferValidity::kInvalidTradeWithSelf:
      return "InvalidTradeWithSelf";
    case OfferValidity::kPriceOutOfRange:
      return "PriceOutOfRange";
    case OfferValidity::kMarketClosed:
      return "MarketClosed";
    case OfferValidity::kInvalidPlayerTypeOffer:
      return "InvalidPlayerTypeOffer";
  }
}

std::ostream& operator<<(std::ostream& os, OfferValidity ov) {
  return os << ToString(ov);
}

std::istream& operator>>(std::istream& is, OfferValidity& ov) {
  static std::string ValidString = ToString(OfferValidity::kValid);
  static std::string NoTradeoffString =
      ToString(OfferValidity::kInvalidNoTradeoff);
  static std::string InsufficientUnitsString =
      ToString(OfferValidity::kInsufficientUnits);
  static std::string InsufficientCashString =
      ToString(OfferValidity::kInsufficientCash);
  static std::string UnimprovingString =
      ToString(OfferValidity::kInvalidUnimproving);
  static std::string TradeWithSelfString =
      ToString(OfferValidity::kInvalidTradeWithSelf);
  static std::string PriceOutOfRangeString =
      ToString(OfferValidity::kPriceOutOfRange);
  static std::string MarketClosedString =
      ToString(OfferValidity::kMarketClosed);
  static std::string PlayerTypeOfferString =
      ToString(OfferValidity::kInvalidPlayerTypeOffer);

  std::string type;
  is >> type;
  is.ignore();
  if (type == ValidString) {
    ov = OfferValidity::kValid;
  } else if (type == NoTradeoffString) {
    ov = OfferValidity::kInvalidNoTradeoff;
  } else if (type == InsufficientUnitsString) {
    ov = OfferValidity::kInsufficientUnits;
  } else if (type == InsufficientCashString) {
    ov = OfferValidity::kInsufficientCash;
  } else if (type == UnimprovingString) {
    ov = OfferValidity::kInvalidUnimproving;
  } else if (type == TradeWithSelfString) {
    ov = OfferValidity::kInvalidTradeWithSelf;
  } else if (type == PriceOutOfRangeString) {
    ov = OfferValidity::kPriceOutOfRange;
  } else if (type == MarketClosedString) {
    ov = OfferValidity::kMarketClosed;
  } else if (type == PlayerTypeOfferString) {
    ov = OfferValidity::kInvalidPlayerTypeOffer;
  }
  return is;
}

}  // namespace market
