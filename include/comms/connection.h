#ifndef _COMMS_CONNECTION_H_
#define _COMMS_CONNECTION_H_

#include <asio.hpp>
#include <deque>
#include <functional>
#include <string>
#include <system_error>

#include "comms/message.h"

namespace comms {

class Connection {
 public:
  Connection(asio::ip::tcp::socket socket,
             std::function<void(std::string)> do_handle_message,
             std::function<void(std::error_code)> do_handle_error);
  void deliver(const Message& msg);

 private:
  void do_read_header();
  void do_read_body();
  void do_write();

  asio::ip::tcp::socket socket_;
  std::function<void(std::string)> do_handle_message_;
  std::function<void(std::error_code)> do_handle_error_;
  Message read_msg_;
  std::deque<Message> write_msgs_;
};

}  // namespace comms

#endif  // _COMMS_CONNECTION_H_
