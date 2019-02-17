#include "comms/reconnector.h"

#include <boost/asio/error.hpp>
#include <iostream>

namespace comms {

void Reconnector::OnError(int id, boost::system::error_code e) {
  if (e == boost::asio::error::eof ||
      e == boost::asio::error::connection_reset) {
    cm_.Disconnect(id);
    if (finalized) {
      cm_.Accept(id);
    }
  }
}

void Reconnector::OnFinalize(std::vector<int> ids) { finalized = true; }

void HookupReconnector(Reconnector& recon, ConnectionEventManager& em) {
  em.error.connect(
      [&recon](int id, boost::system::error_code e) { recon.OnError(id, e); });
  em.finalized.connect(
      [&recon](std::vector<int> ids) { recon.OnFinalize(ids); });
}

}  // namespace comms
