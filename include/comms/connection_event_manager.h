#ifndef _COMMS_CONNECTION_EVENT_MANAGER_H_
#define _COMMS_CONNECTION_EVENT_MANAGER_H_

#include <boost/signals2.hpp>
#include <vector>

namespace comms {

struct ConnectionEventManager {
 public:
  boost::signals2::signal<void(int id)> connected;
  boost::signals2::signal<void(int id)> disconnected;
  boost::signals2::signal<void(int id, std::error_code)> error;
  boost::signals2::signal<void(int id, std::string)> received;
  boost::signals2::signal<void(std::vector<int> ids)> finalized;
};

}  // namespace comms

#endif  // _COMMS_CONNECTION_EVENT_MANAGER_H_
