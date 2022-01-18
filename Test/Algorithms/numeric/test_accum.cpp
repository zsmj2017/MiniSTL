#include "Algorithms/numeric/stl_numeric.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class AccumTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static int mult(int initial_, int element_);
};

int AccumTest::mult(int initial_, int element_) {
  return initial_ * element_;
}

TEST_F(AccumTest, accum1) {
  vector<int> v(5);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i + 1;
  int sum = accumulate(v.begin(), v.end(), 0);
  ASSERT_TRUE(sum == 15);
}

TEST_F(AccumTest, accum2) {
  vector<int> v(5);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i + 1;
  int prod = accumulate(v.begin(), v.end(), 1, mult);
  ASSERT_TRUE(prod == 120);
}