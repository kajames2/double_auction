#ifndef _MARKET_OFFER_VALIDITY_H_
#define _MARKET_OFFER_VALIDITY_H_

#include <iostream>

#include "market/market_state.h"

namespace market {

enum class OfferValidity {
  kValid,
  kInvalidNoTradeoff,
  kInsufficientUnits,
  kInsufficientCash,
  kInvalidUnimproving,
  kInvalidTradeWithSelf,
  kPriceOutOfRange,
  kMarketClosed,
  kInvalidPlayerTypeOffer
};

std::ostream& operator<<(std::ostream& os, OfferValidity ov);
std::istream& operator>>(std::istream& is, OfferValidity& ov);

}  // namespace market

#endif  // _MARKET_OFFER_VALIDITY_H_
