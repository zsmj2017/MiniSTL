#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class ForeachTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static void increase(int &a) {
    a += 1;
  }

  static void sqr(int &a) {
    a = a * a;
  }
};

TEST_F(ForeachTest, foreach0) {
  int numbers[10] = {1, 1, 2, 3, 5, 8, 13, 21, 34, 55};

  for_each(numbers, numbers + 10, ptr_fun(increase));

  ASSERT_TRUE(numbers[0] == 2);
  ASSERT_TRUE(numbers[1] == 2);
  ASSERT_TRUE(numbers[2] == 3);
  ASSERT_TRUE(numbers[3] == 4);
  ASSERT_TRUE(numbers[4] == 6);
  ASSERT_TRUE(numbers[5] == 9);
  ASSERT_TRUE(numbers[6] == 14);
  ASSERT_TRUE(numbers[7] == 22);
  ASSERT_TRUE(numbers[8] == 35);
  ASSERT_TRUE(numbers[9] == 56);
}

TEST_F(ForeachTest, foreach1) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i;
  for_each(v1.begin(), v1.end(), ptr_fun(sqr));

  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 4);
  ASSERT_TRUE(v1[3] == 9);
  ASSERT_TRUE(v1[4] == 16);
  ASSERT_TRUE(v1[5] == 25);
  ASSERT_TRUE(v1[6] == 36);
  ASSERT_TRUE(v1[7] == 49);
  ASSERT_TRUE(v1[8] == 64);
  ASSERT_TRUE(v1[9] == 81);
}
