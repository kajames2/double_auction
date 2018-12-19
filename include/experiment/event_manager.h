#ifndef _EXPERIMENT_EVENT_MANAGER_H_
#define _EXPERIMENT_EVENT_MANAGER_H_

#include "experiment/configuration.h"
#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_validity.h"
#include "market/transaction.h"

#include <signals2.hpp>
#include <vector>

namespace experiment {

class EventManager {
 public:
  boost::signals2::signal<void(const Configuration&)> exp_start;
  boost::signals2::signal<void()> exp_end;
  boost::signals2::signal<void()> round_start;
  boost::signals2::signal<void()> round_end;
  boost::signals2::signal<void()> res_start;
  boost::signals2::signal<void()> res_end;
  boost::signals2::signal<void(market::Bid,
                               const std::vector<market::Transaction>&)>
      bid_accept;
  boost::signals2::signal<void(market::Bid, market::OfferValidity)> bid_reject;
  boost::signals2::signal<void(market::Ask,
                               const std::vector<market::Transaction>&)>
      ask_accept;
  boost::signals2::signal<void(market::Ask, market::OfferValidity)> ask_reject;
  boost::signals2::signal<void()> paused;
  boost::signals2::signal<void()> resumed;
  boost::signals2::signal<void()> payments;
};

}  // namespace experiment

#endif  // _EXPERIMENT_EVENT_MANAGER_H_
