#include "comms/connection_manager.h"
#include "comms/reconnector.h"
#include "experiment/configuration.h"
#include "experiment/controller.h"
#include "experiment/event_manager.h"
#include "server/client_message_processor.h"
#include "server/logger.h"
#include "server/message_sender.h"
#include "server/outputter.h"

#include <boost/asio.hpp>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace experiment;
using namespace market;

int Utility(Holdings h, double c1, double c2, double c3, double c4) {
  if (h.cash * h.units <= 0) {
    return 0;
  }
  return static_cast<int>(
      std::round(c3 + c4 *
                          (c1 * std::pow(h.units, c2) +
                           (1 - c1) * std::pow(h.cash / 100., c2)) /
                          c2));
}

Configuration CreateConfiguration(int n_rounds, int n_players,
                                  std::vector<int> player_ids) {
  std::vector<std::map<int, Holdings>> init_holdings;

  std::map<int, Holdings> per_holdings;
  std::map<int, std::function<int(Holdings)>> utility_funcs;
  std::map<int, double> eperdollars;
  std::map<int, PlayerType> player_types;
  for (int i = 0; i < n_players; ++i) {
    int id = player_ids[i];
    if (i % 2 == 0) {
      player_types[id] = PlayerType::Buyer;
      utility_funcs[id] = [](Holdings h) {
        return Utility(h, 0.269279, -1, 280.404, 897.97374);
      };
      per_holdings[id] = {1700, 1};
    } else {
      player_types[id] = PlayerType::Seller;
      utility_funcs[id] = [](Holdings h) {
        return Utility(h, 0.752208, -1, 297.872, 1019.97);
      };
      per_holdings[id] = {100, 17};
    }
    eperdollars[id] = 80;      
  }

  for (int i = 0; i < n_rounds; ++i) {
    init_holdings.push_back(per_holdings);
  }

  std::chrono::seconds round_time = std::chrono::minutes(3);
  std::chrono::seconds review_time = std::chrono::seconds(10);
  Configuration config;
  config.init_holdings = init_holdings;
  config.eperdollars = eperdollars;
  config.player_types = player_types;
  config.n_players = n_players;
  config.round_time = round_time;
  config.review_time = review_time;
  config.utility_funcs = utility_funcs;
  config.n_rounds = n_rounds;

  return config;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Usage: server <port>\n";
    return 1;
  }
  int n_players = 2;

  boost::asio::io_context io;
  comms::ConnectionEventManager connection_em;

  server::Logger logger(std::cerr);
  HookupLogger(logger, connection_em);

  comms::ConnectionManager cm(io, std::move(connection_em), std::atoi(argv[1]));
  cm.StartAccepting();
  auto tthread = std::thread([&] { io.run(); });
  comms::Reconnector recon(cm);
  comms::HookupReconnector(recon, cm.GetEventManager());

  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "start" && cm.GetIDs().size() == n_players) {
      break;
    } else if (command == "reset") {
      cm.StopAccepting();
      cm.DisconnectAll();
      cm.StartAccepting();
    } else {
      std::cout << "Current Connections: " << cm.GetIDs().size() << " --- Need "
                << n_players << std::endl;
    }
  }
  cm.FinalizeConnections();

  std::vector<int> player_ids = cm.GetIDs();
  int n_rounds = 10;
  Configuration config = CreateConfiguration(n_rounds, n_players, player_ids);

  EventManager controller_em;
  HookupLogger(logger, controller_em);

  Controller controller(io, config, std::move(controller_em), player_ids);

  std::ofstream out_file("results.csv");
  server::Outputter outputter(out_file, controller);
  server::HookupOutputter(outputter, controller.GetEventManager());

  server::MessageSender mess(controller, cm);
  server::HookupMessageSender(mess, cm.GetEventManager());
  server::HookupMessageSender(mess, controller.GetEventManager());

  server::ClientMessageProcessor proc(controller);
  server::HookupClientMessageProcessor(proc, cm.GetEventManager());

  controller.StartExperiment();

  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "pause") {
      controller.Pause();
    } else if (command == "resume") {
      controller.Resume();
    } else if (command == "end") {
      controller.EndExperiment();
      cm.DisconnectAll();
      cm.StopAccepting();
      break;
    } else {
      std::cout << "Invalid command" << std::endl;
    }
    std::cin.ignore();
  }
  std::ofstream pay_out("payouts.txt");
  for (Payout p : controller.GetPayouts()) {
    pay_out << p << std::endl;
  }
  pay_out.close();
  tthread.join();
  out_file.close();
}
