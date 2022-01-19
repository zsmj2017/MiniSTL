#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class FillTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool values_squared(int a, int b) {
    return (a * a == b);
  }
};

TEST_F(FillTest, fill1) {
  vector<int> v(10);
  fill(v.begin(), v.end(), 42);
  ASSERT_TRUE(v[0] == 42);
  ASSERT_TRUE(v[1] == 42);
  ASSERT_TRUE(v[2] == 42);
  ASSERT_TRUE(v[3] == 42);
  ASSERT_TRUE(v[4] == 42);
  ASSERT_TRUE(v[5] == 42);
  ASSERT_TRUE(v[6] == 42);
  ASSERT_TRUE(v[7] == 42);
  ASSERT_TRUE(v[8] == 42);
  ASSERT_TRUE(v[9] == 42);
}
TEST_F(FillTest, fill_n1)

{
  vector<int> v(10);
  fill_n(v.begin(), v.size(), 42);
  ASSERT_TRUE(v[0] == 42);
  ASSERT_TRUE(v[1] == 42);
  ASSERT_TRUE(v[2] == 42);
  ASSERT_TRUE(v[3] == 42);
  ASSERT_TRUE(v[4] == 42);
  ASSERT_TRUE(v[5] == 42);
  ASSERT_TRUE(v[6] == 42);
  ASSERT_TRUE(v[7] == 42);
  ASSERT_TRUE(v[8] == 42);
  ASSERT_TRUE(v[9] == 42);
}
