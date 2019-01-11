#ifndef _SERVER_CLIENT_MESSAGE_PROCESSOR_H_
#define _SERVER_CLIENT_MESSAGE_PROCESSOR_H_

#include <sstream>
#include <string>

#include "comms/connection_event_manager.h"
#include "experiment/controller.h"

namespace server {

class ClientMessageProcessor {
 public:
  ClientMessageProcessor(experiment::Controller& con) : con_(con) {}

  void OnReceived(int id, std::string message) {
    std::string type;
    std::stringstream ss(message);
    ss >> type;
    ss.ignore();
    if (type == "BID") {
      int price, quantity;
      ss >> price >> quantity;
      ss.ignore();
      con_.TakeBid(id, price, quantity);
    } else if (type == "ASK") {
      int price, quantity;
      ss >> price >> quantity;
      ss.ignore();
      con_.TakeAsk(id, price, quantity);
    } else if (type == "RETRACT") {
      int uid;
      ss >> uid;
      ss.ignore();
      con_.RetractOffer(uid);
    } else if (type == "NAME") {
      std::string name;
      ss >> name;
      ss.ignore();
      con_.TakeName(id, name);
    }
  }

 private:
  experiment::Controller& con_;
};

void HookupClientMessageProcessor(ClientMessageProcessor& rec,
                                 comms::ConnectionEventManager& em);

}  // namespace server

#endif  // _SERVER_CLIENT_MESSAGE_PROCESSOR_H_
