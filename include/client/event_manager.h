#ifndef _CLIENT_EVENT_MANAGER_H_
#define _CLIENT_EVENT_MANAGER_H_

#include "market/offer_validity.h"

#include <boost/signals2.hpp>

namespace client {

struct EventManager {
 public:
  boost::signals2::signal<void()> exp_start;
  boost::signals2::signal<void()> exp_end;
  boost::signals2::signal<void()> round_start;
  boost::signals2::signal<void()> round_end;
  boost::signals2::signal<void()> review_start;
  boost::signals2::signal<void()> review_end;
  boost::signals2::signal<void(market::OfferValidity)> bid_received;
  boost::signals2::signal<void(market::OfferValidity)> ask_received;
  boost::signals2::signal<void()> paused;
  boost::signals2::signal<void()> resumed;
  boost::signals2::signal<void()> payments;
  boost::signals2::signal<void()> updated;
};

}  // namespace client

#endif  // _CLIENT_EVENT_MANAGER_H_
