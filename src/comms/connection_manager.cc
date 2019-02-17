#include "comms/connection_manager.h"

#include <boost/asio.hpp>
#include <functional>
#include <string>
#include <system_error>

#include <iostream>

#include "comms/connection.h"
#include "comms/message.h"

using namespace boost::asio;
using ip::tcp;

namespace comms {

ConnectionManager::ConnectionManager(io_context& io_context,
                                     ConnectionEventManager em, int port)
    : acceptor_(io_context),
      endpoint_(tcp::endpoint(tcp::v4(), port)),
      socket_(io_context),
      em_(std::move(em)) {
  OpenAcceptor();
}

void ConnectionManager::Disconnect(int id) {
  participants_.erase(id);
  em_.disconnected(id);
}

void ConnectionManager::DisconnectAll() {
  for (int id : GetIDs()) Disconnect(id);
}

void ConnectionManager::Accept(int id) {
  to_accept_ids.push_front(id);
  if (!is_accepting_) {
    OpenAcceptor();
    do_accept();
  }
}

void ConnectionManager::StartAccepting() {
  is_accepting_new_connections_ = true;
  if (!is_accepting_) {
    OpenAcceptor();
    do_accept();
  }
}

void ConnectionManager::StopAccepting() {
  is_accepting_new_connections_ = false;
  is_accepting_ = false;
  if (acceptor_.is_open()) {
    acceptor_.close();
  }
}

void ConnectionManager::FinalizeConnections() {
  StopAccepting();
  em_.finalized(GetIDs());
}

void ConnectionManager::do_accept() { do_accept(GetNextID()); }

void ConnectionManager::do_accept(int id) {
  is_accepting_ = true;
  acceptor_.async_accept(socket_, [this, id](boost::system::error_code ec) {
    if (!ec) {
      AddConnection(id);
      int next_id = GetNextID();
      if (next_id != -1) {
        do_accept(next_id);
      } else {
        StopAccepting();
      }
    }
  });
}

void ConnectionManager::AddConnection(int id) {
  auto c = std::make_unique<Connection>(
      std::move(socket_),
      [this, id](std::string message) { em_.received(id, message); },
      [this, id](boost::system::error_code ec) { em_.error(id, ec); });
  participants_.emplace(id, std::move(c));
  em_.connected(id);
}

void ConnectionManager::Deliver(int id, const std::string& str) {
  ConstructMessage(message_, str);
  participants_.at(id)->deliver(message_);
}

void ConnectionManager::DeliverAll(const std::string& str) {
  ConstructMessage(message_, str);
  for (auto& participant : participants_) {
    participant.second->deliver(message_);
  }
}

void ConnectionManager::OpenAcceptor() {
  if (!acceptor_.is_open()) {
    acceptor_.open(endpoint_.protocol());
    acceptor_.set_option(socket_base::reuse_address(true));
    acceptor_.bind(endpoint_);
    acceptor_.listen();
  }
}

int ConnectionManager::GetNextID() {
  if (!to_accept_ids.empty()) {
    int id = to_accept_ids.back();
    to_accept_ids.pop_back();
    return id;
  } else if (is_accepting_new_connections_) {
    int id = -1;
    while (participants_.count(++id) > 0) {
    }
    return id;
  } else {
    return -1;
  }
}

std::vector<int> ConnectionManager::GetIDs() const {
  std::vector<int> taken_keys;
  for (const auto& key_val : participants_) {
    taken_keys.push_back(key_val.first);
  }
  std::sort(taken_keys.begin(), taken_keys.end());
  return taken_keys;
}

}  // namespace comms
