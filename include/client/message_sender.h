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
    std::stringstream ss;
    ss << "BID" << '\n';
    ss << price << ' ' << quantity;
    conn_->deliver(comms::ConstructMessage(ss.str()));
    return market::OfferValidity::kValid;
  }

  market::OfferValidity SubmitAsk(int price, int quantity) {
    std::stringstream ss;
    ss << "ASK" << '\n';
    ss << price << ' ' << quantity;
    conn_->deliver(comms::ConstructMessage(ss.str()));
    return market::OfferValidity::kValid;
  }

  bool SubmitRetract(int uid) {
    std::stringstream ss;
    ss << "RETRACT" << '\n';
    ss << uid;
    conn_->deliver(comms::ConstructMessage(ss.str()));
    return true;
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
