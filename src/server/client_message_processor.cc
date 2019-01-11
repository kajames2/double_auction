#include "server/client_message_processor.h"

namespace server {

void HookupClientMessageProcessor(ClientMessageProcessor& rec,
                                  comms::ConnectionEventManager& em) {
  em.received.connect(
      [&rec](int id, std::string mess) { rec.OnReceived(id, mess); });
}

}  // namespace server
