#ifndef _MARKET_OFFER_VALIDITY_H_
#define _MARKET_OFFER_VALIDITY_H_

#include "market/market_state.h"

namespace market {

enum class OfferValidity {
  kValid,
  kInvalidNoTradeoff,
  kInsufficientUnits,
  kInsufficientCash,
  kInvalidUnimproving,
  kInvalidTradeWithSelf,
  kCashChangeOutOfRange
};

template <typename T>
OfferValidity CheckOffer(T offer, const MarketState& market);
std::ostream& operator<<(std::ostream& os, const OfferValidity ov);

}  // namespace market

#endif  // _MARKET_OFFER_VALIDITY_H_
