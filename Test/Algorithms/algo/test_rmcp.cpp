#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class RmCpTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool odd(int a) {
    return (a % 2) != 0;
  }
};

TEST_F(RmCpTest, reverse1) {
  int numbers[6] = {0, 1, 2, 3, 4, 5};

  reverse(numbers, numbers + 6);
  ASSERT_TRUE(numbers[0] == 5);
  ASSERT_TRUE(numbers[1] == 4);
  ASSERT_TRUE(numbers[2] == 3);
  ASSERT_TRUE(numbers[3] == 2);
  ASSERT_TRUE(numbers[4] == 1);
  ASSERT_TRUE(numbers[5] == 0);
}

TEST_F(RmCpTest, revcopy1) {
  int numbers[6] = {0, 1, 2, 3, 4, 5};

  int result[6];
  reverse_copy((int *) numbers, (int *) numbers + 6, (int *) result);
  // 5 4 3 2 1 0
  ASSERT_TRUE(result[0] == 5);
  ASSERT_TRUE(result[1] == 4);
  ASSERT_TRUE(result[2] == 3);
  ASSERT_TRUE(result[3] == 2);
  ASSERT_TRUE(result[4] == 1);
  ASSERT_TRUE(result[5] == 0);
}

TEST_F(RmCpTest, replif1) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i % 5;

  replace_if(v1.begin(), v1.end(), odd, 42);

  // 0 42 2 42 4 0 42 2 42 4
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 42);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 42);
  ASSERT_TRUE(v1[4] == 4);
  ASSERT_TRUE(v1[5] == 0);
  ASSERT_TRUE(v1[6] == 42);
  ASSERT_TRUE(v1[7] == 2);
  ASSERT_TRUE(v1[8] == 42);
  ASSERT_TRUE(v1[9] == 4);
}

TEST_F(RmCpTest, replcpy1) {
  int numbers[6] = {0, 1, 2, 0, 1, 2};
  int result[6] = {0, 0, 0, 0, 0, 0};

  replace_copy((int *) numbers, (int *) numbers + 6, (int *) result, 2, 42);
  ASSERT_TRUE(result[0] == 0);
  ASSERT_TRUE(result[1] == 1);
  ASSERT_TRUE(result[2] == 42);
  ASSERT_TRUE(result[3] == 0);
  ASSERT_TRUE(result[4] == 1);
  ASSERT_TRUE(result[5] == 42);
}

TEST_F(RmCpTest, replace0) {
  int numbers[6] = {0, 1, 2, 0, 1, 2};

  replace(numbers, numbers + 6, 2, 42);

  // 0 1 42 0 1 42
  ASSERT_TRUE(numbers[0] == 0);
  ASSERT_TRUE(numbers[1] == 1);
  ASSERT_TRUE(numbers[2] == 42);
  ASSERT_TRUE(numbers[3] == 0);
  ASSERT_TRUE(numbers[4] == 1);
  ASSERT_TRUE(numbers[5] == 42);
}

TEST_F(RmCpTest, replace1) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i % 5;
  replace(v1.begin(), v1.end(), 2, 42);

  // 0 1 2 3 4 0 1 2 3 4
  // 0 1 42 3 4 0 1 42 3 4
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 42);
  ASSERT_TRUE(v1[3] == 3);
  ASSERT_TRUE(v1[4] == 4);
  ASSERT_TRUE(v1[5] == 0);
  ASSERT_TRUE(v1[6] == 1);
  ASSERT_TRUE(v1[7] == 42);
  ASSERT_TRUE(v1[8] == 3);
  ASSERT_TRUE(v1[9] == 4);
}

TEST_F(RmCpTest, repcpif1) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i % 5;
  vector<int> v2(v1.size());

  // 0 1 2 3 4 0 1 2 3 4
  // 0 1 2 3 4 0 1 2 3 4
  // 0 42 2 42 4 0 42 2 42 4
  replace_copy_if(v1.begin(), v1.end(), v2.begin(), odd, 42);
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 3);
  ASSERT_TRUE(v1[4] == 4);
  ASSERT_TRUE(v1[5] == 0);
  ASSERT_TRUE(v1[6] == 1);
  ASSERT_TRUE(v1[7] == 2);
  ASSERT_TRUE(v1[8] == 3);
  ASSERT_TRUE(v1[9] == 4);

  ASSERT_TRUE(v2[0] == 0);
  ASSERT_TRUE(v2[1] == 42);
  ASSERT_TRUE(v2[2] == 2);
  ASSERT_TRUE(v2[3] == 42);
  ASSERT_TRUE(v2[4] == 4);
  ASSERT_TRUE(v2[5] == 0);
  ASSERT_TRUE(v2[6] == 42);
  ASSERT_TRUE(v2[7] == 2);
  ASSERT_TRUE(v2[8] == 42);
  ASSERT_TRUE(v2[9] == 4);
}

TEST_F(RmCpTest, remove1) {
  int numbers[6] = {1, 2, 3, 1, 2, 3};
  remove((int *) numbers, (int *) numbers + 6, 1);

  // 2 3 2 3 2 3
  ASSERT_TRUE(numbers[0] == 2);
  ASSERT_TRUE(numbers[1] == 3);
  ASSERT_TRUE(numbers[2] == 2);
  ASSERT_TRUE(numbers[3] == 3);
  ASSERT_TRUE(numbers[4] == 2);
  ASSERT_TRUE(numbers[5] == 3);
}

TEST_F(RmCpTest, remif1) {
  int numbers[6] = {0, 0, 1, 1, 2, 2};

  remove_if((int *) numbers, (int *) numbers + 6, odd);

  // 0 0 2 2 2 2
  ASSERT_TRUE(numbers[0] == 0);
  ASSERT_TRUE(numbers[1] == 0);
  ASSERT_TRUE(numbers[2] == 2);
  ASSERT_TRUE(numbers[3] == 2);
  ASSERT_TRUE(numbers[4] == 2);
  ASSERT_TRUE(numbers[5] == 2);
}

TEST_F(RmCpTest, remcopy1) {
  int numbers[6] = {1, 2, 3, 1, 2, 3};
  int result[6] = {0, 0, 0, 0, 0, 0};

  remove_copy((int *) numbers, (int *) numbers + 6, (int *) result, 2);

  ASSERT_TRUE(result[0] == 1);
  ASSERT_TRUE(result[1] == 3);
  ASSERT_TRUE(result[2] == 1);
  ASSERT_TRUE(result[3] == 3);
  ASSERT_TRUE(result[4] == 0);
  ASSERT_TRUE(result[5] == 0);
}

TEST_F(RmCpTest, remcpif1) {
  int numbers[6] = {1, 2, 3, 1, 2, 3};
  int result[6] = {0, 0, 0, 0, 0, 0};

  remove_copy_if((int *) numbers, (int *) numbers + 6, (int *) result, odd);

  // 2 2 0 0 0 0
  ASSERT_TRUE(result[0] == 2);
  ASSERT_TRUE(result[1] == 2);
  ASSERT_TRUE(result[2] == 0);
  ASSERT_TRUE(result[3] == 0);
  ASSERT_TRUE(result[4] == 0);
  ASSERT_TRUE(result[5] == 0);
}