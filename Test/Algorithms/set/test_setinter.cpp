#include "Algorithms/algoset/stl_algoset.h"
#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class SetIntersectionTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(SetIntersectionTest, setintr1) {
  vector<int> v1(10);
  for (int i = 0; i < (int) v1.size(); ++i)
    v1[i] = i;
  vector<int> v2(10);
  for (int i = 0; i < (int) v2.size(); ++i)
    v2[i] = i + 7;

  vector<int> inter;
  set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), back_inserter(inter));
  ASSERT_TRUE(inter.size() == 3);
  ASSERT_TRUE(inter[0] == 7);
  ASSERT_TRUE(inter[1] == 8);
  ASSERT_TRUE(inter[2] == 9);
}

TEST_F(SetIntersectionTest, setintr2) {
  const char *word1 = "ABCDEFGHIJKLMNO";
  const char *word2 = "LMNOPQRSTUVWXYZ";

  string inter;
  MiniSTL::set_intersection(word1, word1 + ::strlen(word1), word2, word2 + ::strlen(word2),
                   MiniSTL::back_inserter(inter), less<char>());
  ASSERT_TRUE(inter.size() == 4);
  ASSERT_TRUE(inter[0] == 'L');
  ASSERT_TRUE(inter[1] == 'M');
  ASSERT_TRUE(inter[2] == 'N');
  ASSERT_TRUE(inter[3] == 'O');
}