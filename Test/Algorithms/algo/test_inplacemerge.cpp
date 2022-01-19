#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class InplaceTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(InplaceTest, inplmrg1) {
  int numbers[6] = {1, 10, 42, 3, 16, 32};
  inplace_merge(numbers, numbers + 3, numbers + 6);

  ASSERT_TRUE(numbers[0] == 1);
  ASSERT_TRUE(numbers[1] == 3);
  ASSERT_TRUE(numbers[2] == 10);
  ASSERT_TRUE(numbers[3] == 16);
  ASSERT_TRUE(numbers[4] == 32);
  ASSERT_TRUE(numbers[5] == 42);
}

// TODO::inplace_merge() need pred
#if 0
TEST_F(InplaceTest, inplmrg2) {
  vector<size_t> v1(10);
  for (size_t i = 0; i < v1.size(); ++i)
    v1[i] = (v1.size() - i - 1) % 5;

  inplace_merge(v1.begin(), v1.begin() + 5, v1.end(), greater<size_t>());

  ASSERT_TRUE(v1[0] == 4);
  ASSERT_TRUE(v1[1] == 4);
  ASSERT_TRUE(v1[2] == 3);
  ASSERT_TRUE(v1[3] == 3);
  ASSERT_TRUE(v1[4] == 2);
  ASSERT_TRUE(v1[5] == 2);
  ASSERT_TRUE(v1[6] == 1);
  ASSERT_TRUE(v1[7] == 1);
  ASSERT_TRUE(v1[8] == 0);
  ASSERT_TRUE(v1[9] == 0);
}
#endif