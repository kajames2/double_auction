#include "market/clearing_queue.h"

#include <algorithm>
#include <chrono>
#include <functional>

#include "market/transaction.h"

namespace market {

ClearingQueue::ClearingQueue()
    : market_start(std::chrono::system_clock::now()) {}

ClearingQueue::ClearingQueue(std::chrono::time_point<std::chrono::system_clock> time) : market_start(time) {}

void ClearingQueue::RetractBid(Bid bid) {
  bids_.erase(std::remove(bids_.begin(), bids_.end(), bid), bids_.end());
}

void ClearingQueue::RetractAsk(Ask ask) {
  asks_.erase(std::remove(asks_.begin(), asks_.end(), ask), asks_.end());
}

std::vector<Transaction> ClearingQueue::Process() {
  return Process(std::chrono::system_clock::now());
}

std::vector<Transaction> ClearingQueue::Process(std::chrono::time_point<std::chrono::system_clock> now) {
  SortQueues();
  std::vector<Transaction> processed;
  auto time = std::chrono::duration<double>(market_start - now);
  while (AreTrades()) {
    auto transaction =
        MakeTransaction(time.count(), bids_.back(), asks_.back());
    processed.push_back(transaction);
    history_.push_back(transaction);
    bids_.pop_back();
    asks_.pop_back();
  }
  return processed;
}

void ClearingQueue::SortQueues() {
  if (!std::is_sorted(bids_.begin(), bids_.end())) {
    std::sort(bids_.begin(), bids_.end(), std::greater<Bid>());
  }
  if (!std::is_sorted(asks_.begin(), asks_.end())) {
    std::sort(asks_.begin(), asks_.end(), std::greater<Ask>());
  }
}

bool ClearingQueue::AreTrades() const {
  if (bids_.empty() || asks_.empty()) {
    return false;
  }
  return bids_.back().price >= asks_.back().price;
}

std::vector<Bid> ClearingQueue::GetBids(int id) const {
  std::vector<Bid> player_bids;
  std::copy_if(bids_.begin(), bids_.end(), std::back_inserter(player_bids),
               [id](const Bid& b) { return b.id == id; });
  return player_bids;
}

std::vector<Ask> ClearingQueue::GetAsks(int id) const {
  std::vector<Ask> player_asks;
  std::copy_if(asks_.begin(), asks_.end(), std::back_inserter(player_asks),
               [id](const Ask& b) { return b.id == id; });
  return player_asks;
}

}  // namespace market
