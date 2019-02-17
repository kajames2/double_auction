#include "comms/connection.h"

#include <boost/asio.hpp>
#include <functional>
#include <string>
#include <system_error>

namespace comms {

using namespace boost::asio;

Connection::Connection(
    ip::tcp::socket socket, std::function<void(std::string)> do_handle_message,
    std::function<void(boost::system::error_code)> do_handle_error)
    : socket_(std::move(socket)),
      do_handle_message_(do_handle_message),
      do_handle_error_(do_handle_error) {
  do_read_header();
}

void Connection::deliver(const Message& msg) {
  bool write_in_progress = !write_msgs_.empty();
  write_msgs_.push_back(msg);
  if (!write_in_progress) {
    do_write();
  }
}

void Connection::do_read_header() {
  async_read(socket_, buffer(read_msg_.data(), Message::header_length),
             [this](boost::system::error_code ec, std::size_t /*length*/) {
               if (!ec && read_msg_.decode_header()) {
                 do_read_body();
               } else {
                 do_handle_error_(ec);
               }
             });
}

void Connection::do_read_body() {
  async_read(socket_, buffer(read_msg_.body(), read_msg_.body_length()),
             [this](boost::system::error_code ec, std::size_t /*length*/) {
               if (!ec) {
                 do_handle_message_(read_msg_.ToString());
                 do_read_header();
               } else {
                 do_handle_error_(ec);
               }
             });
}

void Connection::do_write() {
  async_write(socket_,
              buffer(write_msgs_.front().data(), write_msgs_.front().length()),
              [this](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                  write_msgs_.pop_front();
                  if (!write_msgs_.empty()) {
                    do_write();
                  }
                } else {
                  do_handle_error_(ec);
                }
              });
}

}  // namespace comms
