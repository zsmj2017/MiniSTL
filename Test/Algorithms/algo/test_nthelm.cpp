#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class NthElemTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(NthElemTest, nthelem0) {
  int numbers[7] = {5, 2, 4, 1, 0, 3, 77};
  nth_element(numbers, numbers + 3, numbers + 6);

  ASSERT_TRUE(numbers[0] == 1);
  ASSERT_TRUE(numbers[1] == 0);
  ASSERT_TRUE(numbers[2] == 2);
  ASSERT_TRUE(numbers[3] == 3);
  ASSERT_TRUE(numbers[4] == 4);
  ASSERT_TRUE(numbers[5] == 5);
}
TEST_F(NthElemTest, nthelem1) {
  //6 8 5 1 7 4 1 5 2 6
  //1 1 4 2 5 5 6 7 8 6
  int numbers[10] = {6, 8, 5, 1, 7, 4, 1, 5, 2, 6};

  vector<int> v1(numbers, numbers + 10);
  nth_element(v1.begin(), v1.begin() + v1.size() / 2, v1.end());

  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 4);
  ASSERT_TRUE(v1[3] == 2);
  ASSERT_TRUE(v1[4] == 5);
  ASSERT_TRUE(v1[5] == 5);
  ASSERT_TRUE(v1[6] == 6);
  ASSERT_TRUE(v1[7] == 7);
  ASSERT_TRUE(v1[8] == 8);
  ASSERT_TRUE(v1[9] == 6);
}

// TODO::nth_elemnt() need pred
#if 0
TEST_F(NthElemTest, nthelem2) {
  //4 5 4 2 1 7 4 3 1 6
  //6 7 4 4 5 4 3 2 1 1

  int numbers[10] = {4, 5, 4, 2, 1, 7, 4, 3, 1, 6};
  vector<int> v1(numbers, numbers + 10);
  nth_element(v1.begin(), v1.begin() + v1.size() / 2, v1.end(), greater<int>());

  ASSERT_TRUE(v1[0] == 6);
  ASSERT_TRUE(v1[1] == 7);
  ASSERT_TRUE(v1[2] == 4);
  ASSERT_TRUE(v1[3] == 4);
  ASSERT_TRUE(v1[4] == 5);
  ASSERT_TRUE(v1[5] == 4);
  ASSERT_TRUE(v1[6] == 3);
  ASSERT_TRUE(v1[7] == 2);
  ASSERT_TRUE(v1[8] == 1);
  ASSERT_TRUE(v1[9] == 1);
}
#endif
