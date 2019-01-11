#include "server/outputter.h"

namespace server {

std::string GetOutputLineHeader() {
  return "Event,Round,TimeRemaining,PlayerID,Cash,Units,OfferID,Price,Quantity,"
         "BuyerID,SellerID,Utility";
}

void OutputLine::Reset() {
  event = "";
  round = -1;
  time_rem = -1;
  player_id = -1;
  cash = -1;
  units = -1;
  offer_id = -1;
  price = -1;
  quantity = -1;
  buyer_id = -1;
  seller_id = -1;
  utility = -1;
}

std::ostream& operator<<(std::ostream& os, const OutputLine& line) {
  return os << line.event << "," << OptionalToString(line.round) << ","
            << OptionalToString(line.time_rem) << ","
            << OptionalToString(line.player_id) << ","
            << OptionalToString(line.cash) << ","
            << OptionalToString(line.units) << ","
            << OptionalToString(line.offer_id) << ","
            << OptionalToString(line.price) << ","
            << OptionalToString(line.quantity) << ","
            << OptionalToString(line.buyer_id) << ","
            << OptionalToString(line.seller_id) << ","
            << OptionalToString(line.utility);
}

Outputter::Outputter(std::ostream& out, const experiment::Controller& con)
    : writer(out), con_(con) {
  out << GetOutputLineHeader() << std::endl;
}
void Outputter::OnRoundStarted() { WriteHoldings("Start Holdings"); }
void Outputter::OnRoundEnded() { WriteHoldings("End Holdings"); }

void Outputter::OnBidReceived(market::Bid bid, market::OfferValidity ov) {
  if (ov == market::OfferValidity::kValid) {
    WriteOffer(bid, "Bid");
  }
}
void Outputter::OnAskReceived(market::Ask ask, market::OfferValidity ov) {
  if (ov == market::OfferValidity::kValid) {
    WriteOffer(ask, "Ask");
  }
}
void Outputter::OnOfferRetract(int id) {
  out_line.offer_id = id;
  Write("Retract");
}
void Outputter::OnTransactions(const std::vector<market::Transaction>& trans) {
  for (const auto& t : trans) {
    out_line.price = t.price;
    out_line.quantity = t.quantity;
    out_line.buyer_id = t.buyer_id;
    out_line.seller_id = t.seller_id;
    Write("Transaction");
  }
}

void Outputter::Write(std::string event) {
  out_line.event = event;
  out_line.round = con_.GetExperimentState().round;
  out_line.time_rem = con_.GetTimeRemaining().count();
  writer << out_line << std::endl;
  out_line.Reset();
}

void Outputter::WriteHoldings(std::string event) {
  auto round_results = con_.GetCurrentResults();
  for (const auto& res : round_results) {
    out_line.player_id = res.id;
    out_line.cash = res.holdings.cash;
    out_line.units = res.holdings.units;
    out_line.utility = res.utility;
    Write(event);
  }
}
template <typename T>
void Outputter::WriteOffer(T offer, std::string event) {
  out_line.player_id = offer.player_id;
  out_line.offer_id = offer.unique_id;
  out_line.price = offer.price;
  out_line.quantity = offer.quantity;
  Write(event);
}

template void Outputter::WriteOffer<market::Ask>(market::Ask ask,
                                                 std::string event);
template void Outputter::WriteOffer<market::Bid>(market::Bid bid,
                                                 std::string event);

void HookupOutputter(Outputter& writer, experiment::EventManager& em) {
  em.round_start.connect([&writer]() { writer.OnRoundStarted(); });
  em.round_end.connect([&writer]() { writer.OnRoundEnded(); });
  em.bid_receive.connect([&writer](market::Bid b, market::OfferValidity ov) {
    writer.OnBidReceived(b, ov);
  });
  em.ask_receive.connect([&writer](market::Ask a, market::OfferValidity ov) {
    writer.OnAskReceived(a, ov);
  });
  em.offer_retract.connect([&writer](int id) { writer.OnOfferRetract(id); });
  em.transaction.connect([&writer](const std::vector<market::Transaction>& ts) {
    writer.OnTransactions(ts);
  });
}

}  // namespace server
