#ifndef _EXPERIMENT_EVENT_MANAGER_H_
#define _EXPERIMENT_EVENT_MANAGER_H_

#include "experiment/configuration.h"
#include "experiment/payout.h"
#include "market/ask.h"
#include "market/bid.h"
#include "market/offer_validity.h"
#include "market/transaction.h"

#include <boost/signals2.hpp>
#include <vector>

namespace experiment {

class EventManager {
 public:
  boost::signals2::signal<void(const Configuration&)> exp_start;
  boost::signals2::signal<void()> exp_end;
  boost::signals2::signal<void()> round_start;
  boost::signals2::signal<void()> round_end;
  boost::signals2::signal<void()> review_start;
  boost::signals2::signal<void()> review_end;
  boost::signals2::signal<void(market::Bid, market::OfferValidity)> bid_receive;
  boost::signals2::signal<void(market::Ask, market::OfferValidity)> ask_receive;
  boost::signals2::signal<void(int id)> offer_retract;
  boost::signals2::signal<void(const std::vector<market::Transaction>&)>
      transaction;
  boost::signals2::signal<void(int id, std::string name)> name_taken;
  boost::signals2::signal<void()> paused;
  boost::signals2::signal<void()> resumed;
  boost::signals2::signal<void(const std::vector<experiment::Payout>&)> payments;
};

}  // namespace experiment

#endif  // _EXPERIMENT_EVENT_MANAGER_H_
