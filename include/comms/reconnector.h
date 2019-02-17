#ifndef _COMMS_RECONNECTOR_H_
#define _COMMS_RECONNECTOR_H_

#include "comms/connection_manager.h"
#include "comms/connection_event_manager.h"

#include <boost/asio/error.hpp>

#include <string>
#include <vector>

namespace comms {

class Reconnector {
 public:
  Reconnector(ConnectionManager& cm) : cm_(cm) {}
  void OnError(int id, boost::system::error_code);
  void OnFinalize(std::vector<int> ids);
  
 private:
  bool finalized = false;
  ConnectionManager& cm_;
};

void HookupReconnector(Reconnector& recon, ConnectionEventManager& em);

}  // namespace comms

#endif  // _COMMS_RECONNECTOR_H_
