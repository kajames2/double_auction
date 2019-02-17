#include "market/offer_validity.h"

#include <iostream>

namespace market {

template <typename T>
bool InPriceRange(T offer) {
  return offer.price < 1000;
}

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
