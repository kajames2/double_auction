#include "server/message_sender.h"

namespace server {

MessageSender::MessageSender(const experiment::Controller& con,
                             comms::ConnectionManager& cm)
    : con_(con), cm_(cm) {}

void MessageSender::OnExperimentStarted(const experiment::Configuration& c) {
  for (int id : cm_.GetIDs()) {
    SendExpStart(id, c);
  }
  is_started_ = true;
}

void MessageSender::OnExperimentEnded() { cm_.DeliverAll("EXPENDED"); }
void MessageSender::OnRoundStarted() {
  cm_.DeliverAll("ROUNDSTARTED");
  SendState();
}
void MessageSender::OnRoundEnded() { cm_.DeliverAll("ROUNDENDED"); }
void MessageSender::OnReviewStarted() { cm_.DeliverAll("REVIEWSTARTED"); }
void MessageSender::OnReviewEnded() { cm_.DeliverAll("REVIEWENDED"); }

void MessageSender::OnBidReceived(market::Bid bid, market::OfferValidity ov) {
  std::stringstream outstr;
  outstr << "BIDRECEIVED" << '\n';
  outstr << ov;
  cm_.Deliver(bid.player_id, outstr.str());
  if (ov == market::OfferValidity::kValid) {
    SendState();
  }
}

void MessageSender::OnAskReceived(market::Ask ask, market::OfferValidity ov) {
  std::stringstream outstr;
  outstr << "ASKRECEIVED" << '\n';
  outstr << ov;
  cm_.Deliver(ask.player_id, outstr.str());
  if (ov == market::OfferValidity::kValid) {
    SendState();
  }
}

void MessageSender::OnOfferRetract(int id) { SendState(); }

void MessageSender::OnPaused() { cm_.DeliverAll("PAUSED"); }
void MessageSender::OnResumed() { cm_.DeliverAll("RESUMED"); }
void MessageSender::OnPaymentsCalculated(
    const std::vector<experiment::Payout>& payouts) {
  for (const auto& payout : payouts) {
    std::stringstream outstr;
    outstr << "PAYMENT" << '\n';
    outstr << payout.cents;
    cm_.Deliver(payout.id, outstr.str());
  }
}

void MessageSender::OnConnection(int id) {
  std::stringstream outstr;
  outstr << "CONNECTED" << '\n';
  outstr << id;
  cm_.Deliver(id, outstr.str());
  if (is_started_) {
    SendExpStart(id, con_.GetConfiguration());
    SendState();
  }
}

void MessageSender::SendState() {
  std::stringstream ss;
  ss << "STATE" << '\n';
  ss << con_.GetState();
  cm_.DeliverAll(ss.str());
}

void MessageSender::SendExpStart(int id, const experiment::Configuration& c) {
  std::stringstream outstr;
  outstr << "EXPSTART" << '\n';
  outstr << id << ' ' << c.player_types.at(id);
  cm_.Deliver(id, outstr.str());
}

void HookupMessageSender(MessageSender& mess, experiment::EventManager& em) {
  em.exp_start.connect([&mess](const experiment::Configuration& c) {
    mess.OnExperimentStarted(c);
  });
  em.exp_end.connect([&mess]() { mess.OnExperimentEnded(); });
  em.round_start.connect([&mess]() { mess.OnRoundStarted(); });
  em.round_end.connect([&mess]() { mess.OnRoundEnded(); });
  em.review_start.connect([&mess]() { mess.OnReviewStarted(); });
  em.review_end.connect([&mess]() { mess.OnReviewEnded(); });
  em.bid_receive.connect([&mess](market::Bid bid, market::OfferValidity ov) {
    mess.OnBidReceived(bid, ov);
  });
  em.ask_receive.connect([&mess](market::Ask ask, market::OfferValidity ov) {
    mess.OnAskReceived(ask, ov);
  });
  em.offer_retract.connect([&mess](int id) { mess.OnOfferRetract(id); });
  em.paused.connect([&mess]() { mess.OnPaused(); });
  em.resumed.connect([&mess]() { mess.OnResumed(); });
  em.payments.connect([&mess](const std::vector<experiment::Payout>& payouts) {
    mess.OnPaymentsCalculated(payouts);
  });
}

void HookupMessageSender(MessageSender& mess,
                         comms::ConnectionEventManager& em) {
  em.connected.connect([&mess](int id) { mess.OnConnection(id); });
}

}  // namespace server
