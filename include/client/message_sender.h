#ifndef _CLIENT_MESSAGE_SENDER_H_
#define _CLIENT_MESSAGE_SENDER_H_

#include <sstream>
#include <string>

#include "client/state.h"
#include "comms/connection.h"
#include "comms/message.h"
#include "market/offer_validity.h"

namespace client {

class MessageSender {
 public:
  MessageSender(const State& state, std::shared_ptr<comms::Connection> conn)
      : state_(state), conn_(conn) {}
  market::OfferValidity SubmitBid(int price, int quantity) {
    market::Bid temp{0, state_.id, 0, price, quantity};
    market::OfferValidity val;
    if (state_.exp_state.stage != experiment::Stage::Round ||
        state_.exp_state.status != experiment::Status::Running) {
      val = market::OfferValidity::kMarketClosed;
    } else if (state_.type == experiment::PlayerType::Seller) {
      val = market::OfferValidity::kInvalidPlayerTypeOffer;
    } else {
      val = market::CheckOffer(temp, state_.market_state);
    }
    if (val == market::OfferValidity::kValid) {
      std::stringstream ss;
      ss << "BID" << '\n';
      ss << price << ' ' << quantity;
      conn_->deliver(comms::ConstructMessage(ss.str()));
    }
    return val;
  }

  market::OfferValidity SubmitAsk(int price, int quantity) {
    market::Ask temp{0, state_.id, 0, price, quantity};
    market::OfferValidity val;
    if (state_.exp_state.stage != experiment::Stage::Round ||
        state_.exp_state.status != experiment::Status::Running) {
      val = market::OfferValidity::kMarketClosed;
    } else if (state_.type == experiment::PlayerType::Buyer) {
      val = market::OfferValidity::kInvalidPlayerTypeOffer;
    } else {
      val = market::CheckOffer(temp, state_.market_state);
    }
    if (val == market::OfferValidity::kValid) {
      std::stringstream ss;
      ss << "ASK" << '\n';
      ss << price << ' ' << quantity;
      conn_->deliver(comms::ConstructMessage(ss.str()));
    }
    return val;
  }

  bool SubmitRetract(int uid) {
    bool is_my_offer = false;
    auto my_bids = state_.market_state.bid_queue.GetOffers(state_.id);
    for (const auto& bid : my_bids) {
      if (bid.unique_id == state_.id) is_my_offer = true;
    }
    auto my_asks = state_.market_state.ask_queue.GetOffers(state_.id);
    for (const auto& ask : my_asks) {
      if (ask.unique_id == state_.id) is_my_offer = true;
    }
    if (is_my_offer) {
      std::stringstream ss;
      ss << "RETRACT" << '\n';
      ss << uid;
      conn_->deliver(comms::ConstructMessage(ss.str()));
    }
    return is_my_offer;
  }

  void SubmitName(std::string name) {
    conn_->deliver(comms::ConstructMessage("NAME\n" + name));
  }

 private:
  const State& state_;
  std::shared_ptr<comms::Connection> conn_;
};

}  // namespace client

#endif  // _CLIENT_MESSAGE_SENDER_H_
