#include "comms/connection_manager.h"
#include "comms/reconnector.h"
#include "experiment/configuration.h"
#include "experiment/controller.h"
#include "experiment/event_manager.h"
#include "server/client_message_processor.h"
#include "server/logger.h"
#include "server/message_sender.h"
#include "server/outputter.h"

#include <asio.hpp>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <thread>

using namespace experiment;
using namespace market;

Configuration CreateConfiguration(int n_rounds, int n_players,
                                  std::vector<int> player_ids) {
  std::vector<std::map<int, Holdings>> init_holdings;

  for (int i = 0; i < n_rounds; ++i) {
    std::map<int, Holdings> holdings;
    for (int id : player_ids) {
      holdings[id] = {300 + 10 * i, 5 + i};
    }
    init_holdings.push_back(holdings);
  }

  std::map<int, std::function<int(Holdings)>> utility_funcs;
  std::map<int, double> eperdollars;
  std::map<int, PlayerType> player_types;
  for (int id : player_ids) {
    utility_funcs[id] = [](Holdings h) { return h.cash + h.units * 100; };
    eperdollars[id] = 1;
    player_types[id] = id % 2 == 0 ? PlayerType::Buyer : PlayerType::Seller;
  }

  std::chrono::seconds round_time = std::chrono::minutes(1);
  std::chrono::seconds review_time = std::chrono::seconds(30);
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

int main(int argc, char** argv) {
  int n_players = 2;

  asio::io_context io;
  comms::ConnectionEventManager connection_em;

  server::Logger logger(std::cerr);
  HookupLogger(logger, connection_em);

  comms::ConnectionManager cm(io, std::move(connection_em));
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
  int n_rounds = 2;
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
      cm.DisconnectAll();
      cm.StopAccepting();
      controller.EndExperiment();
      break;
    } else {
      std::cout << "Invalid command" << std::endl;
    }
    std::cin.ignore();
  }
  tthread.join();
  out_file.close();
}
