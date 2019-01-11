#include "comms/reconnector.h"

#include <iostream>

namespace comms {

void Reconnector::OnError(int id, std::error_code e) {
  if (e == asio::error::eof) {
    cm_.Disconnect(id);
    if (finalized) {
      cm_.Accept(id);
    }
  } 
}

void Reconnector::OnFinalize(std::vector<int> ids) { finalized = true; }

void HookupReconnector(Reconnector& recon, ConnectionEventManager& em) {
  em.error.connect(
      [&recon](int id, std::error_code e) { recon.OnError(id, e); });
  em.finalized.connect(
      [&recon](std::vector<int> ids) { recon.OnFinalize(ids); });
}

}  // namespace comms
