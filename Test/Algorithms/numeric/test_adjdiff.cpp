#include "Algorithms/numeric/stl_numeric.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class AdjdiffTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static int mult(int initial_, int element_);
};

int AdjdiffTest::mult(int initial_, int element_) {
  return initial_ * element_;
}

TEST_F(AdjdiffTest, accum1) {
  vector<int> v(5);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i + 1;
  int sum = accumulate(v.begin(), v.end(), 0);
  ASSERT_TRUE(sum == 15);
}

TEST_F(AdjdiffTest, accum2) {
  vector<int> v(5);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i + 1;
  int prod = accumulate(v.begin(), v.end(), 1, mult);
  ASSERT_TRUE(prod == 120);
}

TEST_F(AdjdiffTest, adjdiff0) {
  int numbers[5] = {1, 2, 4, 8, 16};
  int difference[5];
  adjacent_difference(numbers, numbers + 5, (int *) difference);
  ASSERT_TRUE(difference[0] == 1);
  ASSERT_TRUE(difference[1] == 1);
  ASSERT_TRUE(difference[2] == 2);
  ASSERT_TRUE(difference[3] == 4);
  ASSERT_TRUE(difference[4] == 8);
}
TEST_F(AdjdiffTest, adjdiff1) {
  vector<int> v(10);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i * i;
  vector<int> result(v.size());
  adjacent_difference(v.begin(), v.end(), result.begin());
  ASSERT_TRUE(result[0] == 0);
  ASSERT_TRUE(result[1] == 1);
  ASSERT_TRUE(result[2] == 3);
  ASSERT_TRUE(result[3] == 5);
  ASSERT_TRUE(result[4] == 7);
  ASSERT_TRUE(result[5] == 9);
  ASSERT_TRUE(result[6] == 11);
  ASSERT_TRUE(result[7] == 13);
  ASSERT_TRUE(result[8] == 15);
  ASSERT_TRUE(result[9] == 17);
}
TEST_F(AdjdiffTest, adjdiff2) {
  vector<int> v(10);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i + 1;
  vector<int> result(v.size());
  adjacent_difference(v.begin(), v.end(), result.begin(), mult);
  ASSERT_TRUE(result[0] == 1);
  ASSERT_TRUE(result[1] == 2);
  ASSERT_TRUE(result[2] == 6);
  ASSERT_TRUE(result[3] == 12);
  ASSERT_TRUE(result[4] == 20);
  ASSERT_TRUE(result[5] == 30);
  ASSERT_TRUE(result[6] == 42);
  ASSERT_TRUE(result[7] == 56);
  ASSERT_TRUE(result[8] == 72);
  ASSERT_TRUE(result[9] == 90);
}
