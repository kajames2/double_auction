#include "comms/connection.h"

#include <asio.hpp>
#include <functional>
#include <string>
#include <system_error>

namespace comms {

using asio::ip::tcp;

Connection::Connection(tcp::socket socket,
                       std::function<void(std::string)> do_handle_message,
                       std::function<void(std::error_code)> do_handle_error)
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
  asio::async_read(socket_,
                   asio::buffer(read_msg_.data(), Message::header_length),
                   [this](std::error_code ec, std::size_t /*length*/) {
                     if (!ec && read_msg_.decode_header()) {
                       do_read_body();
                     } else {
                       do_handle_error_(ec);
                     }
                   });
}

void Connection::do_read_body() {
  asio::async_read(socket_,
                   asio::buffer(read_msg_.body(), read_msg_.body_length()),
                   [this](std::error_code ec, std::size_t /*length*/) {
                     if (!ec) {
                       do_handle_message_(read_msg_.ToString());
                       do_read_header();
                     } else {
                       do_handle_error_(ec);
                     }
                   });
}

void Connection::do_write() {
  asio::async_write(
      socket_,
      asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
      [this](std::error_code ec, std::size_t /*length*/) {
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
