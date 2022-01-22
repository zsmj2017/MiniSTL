#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class RotateTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(RotateTest, rotate0) {
  int numbers[6] = {0, 1, 2, 3, 4, 5};
  // 3 4 5 0 1 2
  rotate((int *) numbers, numbers + 3, numbers + 6);
  ASSERT_TRUE(numbers[0] == 3);
  ASSERT_TRUE(numbers[1] == 4);
  ASSERT_TRUE(numbers[2] == 5);
  ASSERT_TRUE(numbers[3] == 0);
  ASSERT_TRUE(numbers[4] == 1);
  ASSERT_TRUE(numbers[5] == 2);
}
TEST_F(RotateTest, rotate1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }

  rotate(v1.begin(), v1.begin() + 1, v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 3);
  ASSERT_TRUE(v1[3] == 4);
  ASSERT_TRUE(v1[4] == 5);
  ASSERT_TRUE(v1[5] == 6);
  ASSERT_TRUE(v1[6] == 7);
  ASSERT_TRUE(v1[7] == 8);
  ASSERT_TRUE(v1[8] == 9);
  ASSERT_TRUE(v1[9] == 0);

  rotate(v1.begin(), v1.begin() + 2, v1.end());
  ASSERT_TRUE(v1[0] == 3);
  ASSERT_TRUE(v1[1] == 4);
  ASSERT_TRUE(v1[2] == 5);
  ASSERT_TRUE(v1[3] == 6);
  ASSERT_TRUE(v1[4] == 7);
  ASSERT_TRUE(v1[5] == 8);
  ASSERT_TRUE(v1[6] == 9);
  ASSERT_TRUE(v1[7] == 0);
  ASSERT_TRUE(v1[8] == 1);
  ASSERT_TRUE(v1[9] == 2);

  rotate(v1.begin(), v1.begin() + 7, v1.end());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 3);
  ASSERT_TRUE(v1[4] == 4);
  ASSERT_TRUE(v1[5] == 5);
  ASSERT_TRUE(v1[6] == 6);
  ASSERT_TRUE(v1[7] == 7);
  ASSERT_TRUE(v1[8] == 8);
  ASSERT_TRUE(v1[9] == 9);
}
TEST_F(RotateTest, rotcopy0) {
  int numbers[6] = {0, 1, 2, 3, 4, 5};

  int result[6];
  rotate_copy((int *) numbers, (int *) numbers + 3, (int *) numbers + 6, (int *) result);
  // 3 4 5 0 1 2
  ASSERT_TRUE(result[0] == 3);
  ASSERT_TRUE(result[1] == 4);
  ASSERT_TRUE(result[2] == 5);
  ASSERT_TRUE(result[3] == 0);
  ASSERT_TRUE(result[4] == 1);
  ASSERT_TRUE(result[5] == 2);
}
TEST_F(RotateTest, rotcopy1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }
  vector<int> v2(v1.size());

  rotate_copy(v1.begin(), v1.begin() + 1, v1.end(), v2.begin());
  ASSERT_TRUE(v2[0] == 1);
  ASSERT_TRUE(v2[1] == 2);
  ASSERT_TRUE(v2[2] == 3);
  ASSERT_TRUE(v2[3] == 4);
  ASSERT_TRUE(v2[4] == 5);
  ASSERT_TRUE(v2[5] == 6);
  ASSERT_TRUE(v2[6] == 7);
  ASSERT_TRUE(v2[7] == 8);
  ASSERT_TRUE(v2[8] == 9);
  ASSERT_TRUE(v2[9] == 0);

  rotate_copy(v1.begin(), v1.begin() + 3, v1.end(), v2.begin());
  ASSERT_TRUE(v2[0] == 3);
  ASSERT_TRUE(v2[1] == 4);
  ASSERT_TRUE(v2[2] == 5);
  ASSERT_TRUE(v2[3] == 6);
  ASSERT_TRUE(v2[4] == 7);
  ASSERT_TRUE(v2[5] == 8);
  ASSERT_TRUE(v2[6] == 9);
  ASSERT_TRUE(v2[7] == 0);
  ASSERT_TRUE(v2[8] == 1);
  ASSERT_TRUE(v2[9] == 2);
}
