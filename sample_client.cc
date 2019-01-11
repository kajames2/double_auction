#include <asio.hpp>
#include <cstdlib>
#include <deque>
#include <iostream>
#include <thread>
#include "comms/connection.h"

#include "client/command_line_outputter.h"
#include "client/event_manager.h"
#include "client/message_sender.h"
#include "client/server_message_processor.h"
#include "client/state.h"

using asio::ip::tcp;
using namespace client;

int main(int argc, char* argv[]) {
  try {
    if (argc != 2) {
      std::cerr << "Usage: chat_client <host>\n";
      return 1;
    }

    asio::io_context io_context;

    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(argv[1], "12345");
    tcp::socket socket(io_context);
    asio::connect(socket, endpoints);

    EventManager em;
    State state;
    ServerMessageProcessor processor(state, std::move(em));

    bool connected_to_server = true;
    auto conn = std::make_shared<comms::Connection>(
        std::move(socket),
        [&processor](std::string message) { processor.OnReceived(message); },
        [&connected_to_server](std::error_code) {
          std::cout << "Connection to server lost." << std::endl;
          connected_to_server = false;
        });
    
    MessageSender sender(state, conn);
    CommandLineOutputter clo(state);
    HookupCommandLineOutputter(clo, processor.GetEventManager());

    std::thread t([&io_context]() { io_context.run(); });
    while (connected_to_server) {
      std::string command;
      std::cin >> command;
      if (command == "bid") {
        int price;
        std::cin >> price;
        std::cin.ignore();
        auto ov = sender.SubmitBid(price, 1);
        if (ov != market::OfferValidity::kValid) {
          std::cout << "Bid Rejected.  Reason: " << ov << std::endl;
        }
      } else if (command == "ask") {
        int price;
        std::cin >> price;
        std::cin.ignore();
        auto ov = sender.SubmitAsk(price, 1);
        if (ov != market::OfferValidity::kValid) {
          std::cout << "Ask Rejected.  Reason: " << ov << std::endl;
        }
      } else if (command == "retract") {
        int uid;
        std::cin >> uid;
        std::cin.ignore();
        bool retracted = sender.SubmitRetract(uid);
        if (!retracted) {
          std::cout << "You do not have a bid with that id." << std::endl;
        }
      } else if (command == "name") {
        std::string name;
        std::cin >> name;
        std::cin.ignore();
        sender.SubmitName(name);
      }
    }
    t.join();
  } catch (std::exception& e) {
    std::cerr << "Exception: " << e.what() << "\n";
  }

  return 0;
}
