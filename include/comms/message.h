// Mostly copied from the chat example from asio documentation

#ifndef _COMMS_MESSAGE_H_
#define _COMMS_MESSAGE_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <algorithm>

namespace comms {

class Message {
 public:
  enum { header_length = 4 };
  enum { max_body_length = 1024 };

  Message() : body_length_(0) {}
  const char* data() const { return data_; }
  char* data() { return data_; }
  size_t length() const { return header_length + body_length_; }
  const char* body() const { return data_ + header_length; }
  char* body() { return data_ + header_length; }
  size_t body_length() const { return body_length_; }

  void body_length(size_t new_length) {
    body_length_ = new_length;
    if (body_length_ > max_body_length) body_length_ = max_body_length;
  }

  bool decode_header() {
    char header[header_length + 1] = "";
    std::strncat(header, data_, header_length);
    body_length_ = std::atoi(header);
    if (body_length_ > max_body_length) {
      body_length_ = 0;
      return false;
    }
    return true;
  }

  void encode_header() {
    char header[header_length + 1] = "";
    std::sprintf(header, "%4d", static_cast<int>(body_length_));
    std::memcpy(data_, header, header_length);
  }

  std::string ToString() {
    return std::string(body(), body() + body_length());
  }

 private:
  char data_[header_length + max_body_length];
  size_t body_length_;
};

Message ConstructMessage(std::string msg);
void ConstructMessage(Message& m, std::string msg);

}  // namespace comms

#endif  // _COMMS_MESSAGE_H_
