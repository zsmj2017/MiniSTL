#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class RndShuffleTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  class MyRandomGenerator {
   public:
    unsigned long operator()(unsigned long n) {
      return rand() % n;
    }
  };
};

TEST_F(RndShuffleTest, rndshuf0) {
  int numbers[6] = {1, 2, 3, 4, 5, 6};

  random_shuffle(numbers, numbers + 6);

  ASSERT_TRUE(count(numbers, numbers + 6, 1) == 1);
  ASSERT_TRUE(count(numbers, numbers + 6, 2) == 1);
  ASSERT_TRUE(count(numbers, numbers + 6, 3) == 1);
  ASSERT_TRUE(count(numbers, numbers + 6, 4) == 1);
  ASSERT_TRUE(count(numbers, numbers + 6, 5) == 1);
  ASSERT_TRUE(count(numbers, numbers + 6, 6) == 1);
}
TEST_F(RndShuffleTest, rndshuf1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }

  MyRandomGenerator r;
  for (int i = 0; i < 3; i++) {
    random_shuffle(v1.begin(), v1.end(), r);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 0) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 1) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 2) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 3) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 4) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 5) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 6) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 7) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 8) == 1);
    ASSERT_TRUE(count(v1.begin(), v1.end(), 9) == 1);
  }
}
