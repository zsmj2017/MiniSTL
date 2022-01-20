#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class MergeTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(MergeTest, merge0) {
  int numbers1[5] = {1, 6, 13, 25, 101};
  int numbers2[5] = {-5, 26, 36, 46, 99};

  int result[10];
  merge((int *) numbers1, (int *) numbers1 + 5, (int *) numbers2, (int *) numbers2 + 5, (int *) result);

  ASSERT_TRUE(result[0] == -5);
  ASSERT_TRUE(result[1] == 1);
  ASSERT_TRUE(result[2] == 6);
  ASSERT_TRUE(result[3] == 13);
  ASSERT_TRUE(result[4] == 25);
  ASSERT_TRUE(result[5] == 26);
  ASSERT_TRUE(result[6] == 36);
  ASSERT_TRUE(result[7] == 46);
  ASSERT_TRUE(result[8] == 99);
  ASSERT_TRUE(result[9] == 101);
}
TEST_F(MergeTest, merge1) {
  vector<int> v1(5);
  vector<int> v2(v1.size());
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }
  for (int i = 0; i < (int) v1.size(); ++i) {
    v2[i] = i + 3;
  }

  vector<int> result(v1.size() + v2.size());
  merge(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin());

  ASSERT_TRUE(result[0] == 0);
  ASSERT_TRUE(result[1] == 1);
  ASSERT_TRUE(result[2] == 2);
  ASSERT_TRUE(result[3] == 3);
  ASSERT_TRUE(result[4] == 3);
  ASSERT_TRUE(result[5] == 4);
  ASSERT_TRUE(result[6] == 4);
  ASSERT_TRUE(result[7] == 5);
  ASSERT_TRUE(result[8] == 6);
  ASSERT_TRUE(result[9] == 7);
}
TEST_F(MergeTest, merge2) {
  vector<int> v1(5);
  vector<int> v2(v1.size());
  for (int i = 0; (size_t) i < v1.size(); ++i) {
    v1[i] = 10 - i;
    v2[i] = 7 - i;
  }
  vector<int> result(v1.size() + v2.size());
  merge(v1.begin(), v1.end(), v2.begin(), v2.end(), result.begin(), greater<int>());

  ASSERT_TRUE(result[0] == 10);
  ASSERT_TRUE(result[1] == 9);
  ASSERT_TRUE(result[2] == 8);
  ASSERT_TRUE(result[3] == 7);
  ASSERT_TRUE(result[4] == 7);
  ASSERT_TRUE(result[5] == 6);
  ASSERT_TRUE(result[6] == 6);
  ASSERT_TRUE(result[7] == 5);
  ASSERT_TRUE(result[8] == 4);
  ASSERT_TRUE(result[9] == 3);
}