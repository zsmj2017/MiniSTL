#include "Algorithms/algoset/stl_algoset.h"
#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class SetUnionTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(SetUnionTest, setunon0) {
  int v1[3] = {13, 18, 23};
  int v2[4] = {10, 13, 17, 23};
  int result[7] = {0, 0, 0, 0, 0, 0, 0};

  set_union((int *) v1, (int *) v1 + 3, (int *) v2, (int *) v2 + 4, (int *) result);

  ASSERT_TRUE(result[0] == 10);
  ASSERT_TRUE(result[1] == 13);
  ASSERT_TRUE(result[2] == 17);
  ASSERT_TRUE(result[3] == 18);
  ASSERT_TRUE(result[4] == 23);
  ASSERT_TRUE(result[5] == 0);
  ASSERT_TRUE(result[6] == 0);
}

TEST_F(SetUnionTest, setunon1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i)
    v1[i] = i;
  vector<int> v2(10);
  for (int i = 0; i < (int) v2.size(); ++i)
    v2[i] = i + 7;

  vector<int> diff;
  set_union(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(diff));
  ASSERT_TRUE(diff.size() == 17);
  for (int i = 0; i < 17; ++i) {
    ASSERT_TRUE(diff[i] == i);
  }
}

TEST_F(SetUnionTest, setunon2) {
  const char *word1 = "ABCDEFGHIJKLMNO";
  const char *word2 = "LMNOPQRSTUVWXYZ";

  string diff;
  MiniSTL::set_union(word1, word1 + ::strlen(word1), word2, word2 + ::strlen(word2),
                     MiniSTL::back_inserter(diff), less<char>());
  ASSERT_TRUE(diff.size() == 26);
  for (int i = 0; i < 26; ++i) {
    ASSERT_TRUE(diff[i] == ('A' + i));
  }
}