#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class PermTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(PermTest, prevprm0) {
  int v1[3] = {0, 1, 2};
  prev_permutation(v1, v1 + 3);
  EXPECT_EQ(v1[0], 2);
  EXPECT_EQ(v1[1], 1);
  EXPECT_EQ(v1[2], 0);
}
TEST_F(PermTest, prevprm1) {
  vector<int> v1(3);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }

  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 0);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 1);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 0);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 2);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 1);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 0);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 1);
  prev_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 0);
}

TEST_F(PermTest, prevprm2) {
  vector<int> v1(3);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }

  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 1);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 2);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 0);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 1);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 0);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 1);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 2);
  prev_permutation(v1.begin(), v1.end(), greater<int>());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 0);
}

TEST_F(PermTest, nextprm0) {
  int v1[3] = {0, 1, 2};
  next_permutation(v1, v1 + 3);

  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 1);
}
TEST_F(PermTest, nextprm1) {
  vector<int> v1(3);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }

  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 1);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 2);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 0);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 1);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 0);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 0);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 1);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 0);
  ASSERT_TRUE(v1[2] == 2);
  next_permutation(v1.begin(), v1.end());
  ASSERT_TRUE(v1[0] == 1);
  ASSERT_TRUE(v1[1] == 2);
  ASSERT_TRUE(v1[2] == 0);
}

TEST_F(PermTest, nextprm2) {
  vector<char> v1(3);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i + 'A';
  }

  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'A');
  ASSERT_TRUE(v1[1] == 'C');
  ASSERT_TRUE(v1[2] == 'B');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'B');
  ASSERT_TRUE(v1[1] == 'A');
  ASSERT_TRUE(v1[2] == 'C');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'B');
  ASSERT_TRUE(v1[1] == 'C');
  ASSERT_TRUE(v1[2] == 'A');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'C');
  ASSERT_TRUE(v1[1] == 'A');
  ASSERT_TRUE(v1[2] == 'B');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'C');
  ASSERT_TRUE(v1[1] == 'B');
  ASSERT_TRUE(v1[2] == 'A');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'A');
  ASSERT_TRUE(v1[1] == 'B');
  ASSERT_TRUE(v1[2] == 'C');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'A');
  ASSERT_TRUE(v1[1] == 'C');
  ASSERT_TRUE(v1[2] == 'B');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'B');
  ASSERT_TRUE(v1[1] == 'A');
  ASSERT_TRUE(v1[2] == 'C');
  next_permutation(v1.begin(), v1.end(), less<char>());
  ASSERT_TRUE(v1[0] == 'B');
  ASSERT_TRUE(v1[1] == 'C');
  ASSERT_TRUE(v1[2] == 'A');
}