#include "client/command_line_outputter.h"

namespace client {

void HookupCommandLineOutputter(CommandLineOutputter& clo, EventManager& em) {
  em.exp_start.connect([&clo]() { clo.OnExperimentStart(); });
  em.exp_end.connect([&clo]() { clo.OnExperimentEnd(); });
  em.round_start.connect([&clo]() { clo.OnRoundStart(); });
  em.round_end.connect([&clo]() { clo.OnRoundEnd(); });
  em.review_start.connect([&clo]() { clo.OnReviewStart(); });
  em.review_end.connect([&clo]() { clo.OnReviewEnd(); });
  em.bid_received.connect(
      [&clo](market::OfferValidity ov) { clo.OnBidReceived(ov); });
  em.ask_received.connect(
      [&clo](market::OfferValidity ov) { clo.OnAskReceived(ov); });
  em.paused.connect([&clo]() { clo.OnPaused(); });
  em.resumed.connect([&clo]() { clo.OnResumed(); });
  em.payments.connect([&clo]() { clo.OnPayments(); });
  em.updated.connect([&clo]() { clo.OnUpdate(); });
}

}  // namespace client
