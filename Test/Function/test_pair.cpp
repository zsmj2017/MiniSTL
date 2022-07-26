#include "Function/function_adapter.h"
#include "string"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
using std::string;

class PairTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(PairTest, pair0) {
  pair<int, int> p = make_pair(1, 10);

  ASSERT_TRUE(p.first == 1);
  ASSERT_TRUE(p.second == 10);
}

TEST_F(PairTest, init) {
  pair<int, string> PAIR_ARRAY[] = {pair<int, string>(0, "0")};

  int PAIR_ARRAY_SIZE = sizeof(PAIR_ARRAY) > 0 ? sizeof(PAIR_ARRAY) / sizeof(PAIR_ARRAY[0]) : 0;

  for (int i = 0; i < PAIR_ARRAY_SIZE; i++) {
    ASSERT_TRUE(PAIR_ARRAY[i].first == 0);
    ASSERT_TRUE(PAIR_ARRAY[i].second == "0");
    PAIR_ARRAY[i].second = "1";
  }
}