#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <cstring>
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class AdjTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static int equal_length(const char *v1_, const char *v2_);
  static int mult(int initial_, int element_);
};

int AdjTest::mult(int initial_, int element_) {
  return initial_ * element_;
}

int AdjTest::equal_length(const char *v1_, const char *v2_) {
  return std::strlen(v1_) == std::strlen(v2_);
}

TEST_F(AdjTest, adjfind0) {
  int numbers1[5] = {1, 2, 4, 8, 16};
  int numbers2[5] = {5, 3, 2, 1, 1};

  int *location = adjacent_find((int *) numbers1, (int *) numbers1 + 5);
  ASSERT_TRUE(location == numbers1 + 5);// no adj so loc should be _last

  location = adjacent_find((int *) numbers2, (int *) numbers2 + 5);
  ASSERT_TRUE(location != numbers2 + 5);// adj location off should be 3 (first 1)
  ASSERT_TRUE((location - numbers2) == 3);
}
TEST_F(AdjTest, adjfind1) {
  typedef vector<int> IntVector;
  IntVector v(10);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i;
  IntVector::iterator location;
  location = adjacent_find(v.begin(), v.end());
  ASSERT_TRUE(location == v.end());
  v[6] = 7;
  location = adjacent_find(v.begin(), v.end());
  ASSERT_TRUE(location != v.end());
}

TEST_F(AdjTest, adjfind2) {
  typedef vector<const char *> CStrVector;

  const char *names[] = {"Brett", "Graham", "Jack", "Mike", "Todd"};

  const int nameCount = sizeof(names) / sizeof(names[0]);
  CStrVector v(nameCount);
  for (int i = 0; i < nameCount; i++)
    v[i] = names[i];
  CStrVector::iterator location;
  location = adjacent_find(v.begin(), v.end(), equal_length);

  ASSERT_TRUE(location != v.end());
}
