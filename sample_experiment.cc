#include "experiment/configuration.h"
#include "experiment/controller.h"
#include "experiment/event_manager.h"
#include "experiment/logger.h"

#include <asio.hpp>
#include <iostream>
#include <string>
#include <thread>

using namespace experiment;
using namespace market;

void SetLoggerToListen(Logger&, EventManager&);

int main(int argc, char** argv) {
  int n_players = 2;
  std::vector<std::map<int, Holdings>> init_holdings;

  for (int i = 0; i < 2; ++i) {
    std::map<int, Holdings> holdings;
    for (int j = 0; j < n_players; ++j) {
      holdings[j] = {300 + 10 * i, 5 + i};
    }
    init_holdings.push_back(holdings);
  }

  std::map<int, std::function<int(Holdings)>> utility_funcs;
  std::map<int, double> eperdollars;
  for (int i = 0; i < n_players; ++i) {
    utility_funcs[i] = [](Holdings h) { return h.cash + h.units * 100; };
    eperdollars[i] = 1;
  }

  std::chrono::seconds round_time = std::chrono::minutes(1);
  Configuration config;
  config.init_holdings = init_holdings;
  config.eperdollars = eperdollars;
  config.n_players = n_players;
  config.round_time = round_time;
  config.utility_funcs = utility_funcs;

  EventManager controller_em;
  Logger logger("log.txt");
  SetLoggerToListen(logger, controller_em);

  asio::io_context io;
  std::thread tthread;
  Controller controller(io, config, std::move(controller_em));
  while (true) {
    std::string command;
    std::cin >> command;
    if (command == "status") {
      std::cout << controller.GetState() << std::endl;
    } else if (command == "start") {
      controller.StartExperiment();
      tthread = std::thread([&] { io.run(); });
    } else if (command == "pause") {
      controller.Pause();
    } else if (command == "resume") {
      controller.Resume();
    } else if (command == "bid") {
      int player_id;
      int price;
      int quant;
      std::cin >> player_id >> price >> quant;
      auto val = controller.TakeBid(player_id, price, quant);
      if (val != market::OfferValidity::kValid) std::cout << val << std::endl;
    } else if (command == "ask") {
      int player_id;
      int price;
      int quant;
      std::cin >> player_id >> price >> quant;
      auto val = controller.TakeAsk(player_id, price, quant);
      if (val != market::OfferValidity::kValid) std::cout << val << std::endl;
    } else if (command == "retract") {
      int offer_id;
      std::cin >> offer_id;
      controller.RetractOffer(offer_id);
    } else if (command == "end") {
      controller.EndExperiment();
      break;
    } else {
      std::cout << "Invalid command" << std::endl;
    }
    std::cin.ignore();
  }
  tthread.join();
}

void SetLoggerToListen(Logger& log, EventManager& em) {
  em.exp_start.connect(
      [&log](const Configuration& c) { log.OnExperimentStarted(c); });
  em.exp_end.connect([&log]() { log.OnExperimentEnded(); });
  em.round_start.connect([&log]() { log.OnRoundStarted(); });
  em.round_end.connect([&log]() { log.OnRoundEnded(); });
  em.res_start.connect([&log]() { log.OnRoundResultsStarted(); });
  em.res_end.connect([&log]() { log.OnRoundResultsEnded(); });
  em.bid_accept.connect(
      [&log](market::Bid bid, const std::vector<Transaction>& trans) {
        log.OnBidAccepted(bid, trans);
      });
  em.bid_reject.connect([&log](market::Bid bid, OfferValidity ov) {
    log.OnBidRejected(bid, ov);
  });
  em.ask_accept.connect(
      [&log](market::Ask ask, const std::vector<Transaction>& trans) {
        log.OnAskAccepted(ask, trans);
      });
  em.ask_reject.connect([&log](market::Ask ask, OfferValidity ov) {
    log.OnAskRejected(ask, ov);
  });
  em.paused.connect([&log]() { log.OnPaused(); });
  em.resumed.connect([&log]() { log.OnResumed(); });
  em.payments.connect([&log]() { log.OnPaymentsCalculated(); });
}
