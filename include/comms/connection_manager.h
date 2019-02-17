#ifndef _COMMS_CONNECTION_MANAGER_H_
#define _COMMS_CONNECTION_MANAGER_H_

#include <boost/asio.hpp>
#include <deque>
#include <map>
#include <memory>
#include "comms/connection.h"
#include "comms/connection_event_manager.h"
#include "comms/message.h"

namespace comms {

class ConnectionManager {
 public:
  ConnectionManager(boost::asio::io_context& io_context, ConnectionEventManager em,
                    int port = 12345);

  void Disconnect(int id);
  void DisconnectAll();
  void Accept(int id);
  void StartAccepting();
  void StopAccepting();
  void FinalizeConnections();

  void Deliver(int id, const std::string& str);
  void DeliverAll(const std::string& str);

  int ConnectionCount() { return participants_.size(); }
  std::vector<int> GetIDs() const;
  ConnectionEventManager& GetEventManager() { return em_; }

 private:
  void do_accept();
  void do_accept(int id);
  void AddConnection(int id);
  int GetNextID();
  void OpenAcceptor();
  
  Message message_;
  std::deque<int> to_accept_ids;
  std::map<int, std::unique_ptr<Connection>> participants_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::endpoint endpoint_;
  boost::asio::ip::tcp::socket socket_;
  ConnectionEventManager em_;
  bool is_accepting_ = false;
  bool is_accepting_new_connections_ = false;
};

}  // namespace comms

#endif  // _COMMS_CONNECTION_MANAGER_H_
