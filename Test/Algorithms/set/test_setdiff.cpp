#include "Algorithms/algoset/stl_algoset.h"
#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class SetDifferenceTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(SetDifferenceTest, setsymd0) {
  int v1[3] = {13, 18, 23};
  int v2[4] = {10, 13, 17, 23};
  int result[4] = {0, 0, 0, 0};

  set_symmetric_difference((int *) v1, (int *) v1 + 3, (int *) v2, (int *) v2 + 4, (int *) result);
  ASSERT_TRUE(result[0] == 10);
  ASSERT_TRUE(result[1] == 17);
  ASSERT_TRUE(result[2] == 18);
  ASSERT_TRUE(result[3] == 0);
}

TEST_F(SetDifferenceTest, setsymd1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i)
    v1[i] = i;
  vector<int> v2(10);
  for (int i = 0; i < (int) v2.size(); ++i)
    v2[i] = i + 7;

  vector<int> diff;
  set_symmetric_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(diff));
  ASSERT_TRUE(diff.size() == 14);
  int int_res[] = {0, 1, 2, 3, 4, 5, 6, 10, 11, 12, 13, 14, 15, 16};
  for (int i = 0; i < 14; ++i) {
    ASSERT_TRUE(diff[i] == int_res[i]);
  }
}

TEST_F(SetDifferenceTest, setsymd2) {
  const char *word1 = "ABCDEFGHIJKLMNO";
  const char *word2 = "LMNOPQRSTUVWXYZ";

  string diff;
  set_symmetric_difference(word1, word1 + ::strlen(word1), word2, word2 + ::strlen(word2),
                           MiniSTL::back_inserter(diff), less<char>());
  ASSERT_TRUE(diff.size() == 22);
  char char_res[] = "ABCDEFGHIJKPQRSTUVWXYZ";
  for (int i = 0; i < 22; ++i) {
    ASSERT_TRUE(diff[i] == char_res[i]);
  }
}

TEST_F(SetDifferenceTest, setdiff0) {
  int v1[3] = {13, 18, 23};
  int v2[4] = {10, 13, 17, 23};
  int result[4] = {0, 0, 0, 0};
  //18 0 0 0
  //10 17 23 0

  set_difference((int *) v1, (int *) v1 + 3, (int *) v2, (int *) v2 + 4, (int *) result);
  ASSERT_TRUE(result[0] == 18);
  ASSERT_TRUE(result[1] == 0);
  ASSERT_TRUE(result[2] == 0);
  ASSERT_TRUE(result[3] == 0);

  set_difference((int *) v2, (int *) v2 + 4, (int *) v1, (int *) v1 + 2, (int *) result);
  ASSERT_TRUE(result[0] == 10);
  ASSERT_TRUE(result[1] == 17);
  ASSERT_TRUE(result[2] == 23);
  ASSERT_TRUE(result[3] == 0);
}

TEST_F(SetDifferenceTest, setdiff1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i)
    v1[i] = i;
  vector<int> v2(10);
  for (int i = 0; i < (int) v2.size(); ++i)
    v2[i] = i + 7;

  vector<int> diff;
  MiniSTL::set_difference(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(diff));
  ASSERT_TRUE(diff.size() == 7);
  for (int i = 0; i < 7; ++i) {
    ASSERT_TRUE(diff[i] == i);
  }
}

TEST_F(SetDifferenceTest, setdiff2) {
  const char *word1 = "ABCDEFGHIJKLMNO";
  const char *word2 = "LMNOPQRSTUVWXYZ";

  string diff;
  MiniSTL::set_difference(word1, word1 + strlen(word1), word2, word2 + ::strlen(word2), MiniSTL::back_inserter(diff), less<char>());
  ASSERT_TRUE(diff.size() == 11);
  for (int i = 0; i < 11; ++i) {
    ASSERT_TRUE(diff[i] == ('A' + i));
  }
}