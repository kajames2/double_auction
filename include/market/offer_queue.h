#ifndef _MARKET_OFFER_QUEUE_H_
#define _MARKET_OFFER_QUEUE_H_

#include <algorithm>
#include <cassert>
#include <vector>

namespace market {

template <typename T>
class OfferQueue {
 public:
  void push_back(T offer);
  void RetractOffer(int id);
  void pop_back() {
    assert(offers_.size() > 0);
    offers_.pop_back();
  }
  void clear() { offers_.clear(); }

  auto begin() const { return offers_.begin(); }
  auto end() const { return offers_.end(); }
  auto rbegin() const { return offers_.rbegin(); }
  auto rend() const { return offers_.rend(); }

  std::size_t size() const { return offers_.size(); }
  bool empty() const { return offers_.empty(); }
  T GetStandingOffer() const { return offers_.back(); }
  std::vector<T> GetOffers(int player_id) const;

 private:
  std::vector<T> offers_;
};

template <typename T>
void OfferQueue<T>::push_back(T offer) {
  offers_.push_back(offer);
  std::sort(offers_.begin(), offers_.end());
}

template <typename T>
void OfferQueue<T>::RetractOffer(int id) {
  offers_.erase(std::remove_if(offers_.begin(), offers_.end(),
                               [id](T offer) { return offer.unique_id == id; }),
                offers_.end());
}

template <typename T>
std::vector<T> OfferQueue<T>::GetOffers(int player_id) const {
  std::vector<T> offers;
  std::copy_if(offers_.begin(), offers_.end(), std::back_inserter(offers),
               [player_id](const T& b) { return b.player_id == player_id; });
  return offers;
}

}  // namespace market

#endif  // _MARKET_OFFER_QUEUE_H_
