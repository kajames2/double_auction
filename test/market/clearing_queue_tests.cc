#include <gtest/gtest.h>

#include "market/clearing_queue.h"
#include "market/pausable_timer.h"

class ClearingQueueTest : public ::testing::Test {
 public:
  ClearingQueueTest() {}

 protected:
  virtual void SetUp() {}
  market::ClearingQueue cq;
};

TEST_F(ClearingQueueTest, AddBidTest) {
  cq.AddBid({5, 1, 1, 100});
  EXPECT_EQ(1, cq.GetBids(1).size());
  EXPECT_EQ(0, cq.GetBids(0).size());
}
