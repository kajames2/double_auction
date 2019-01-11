#include "comms/message.h"

#include <algorithm>

namespace comms {

Message ConstructMessage(std::string msg) {
  Message m;
  ConstructMessage(m, msg);
  return m;
}

void ConstructMessage(Message& m, std::string msg) {
  std::copy_n(msg.begin(), msg.size(), m.body());
  m.body_length(msg.size());
  m.encode_header();
}

}  // namespace comms
