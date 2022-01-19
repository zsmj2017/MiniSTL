#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class IncludeTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(IncludeTest, incl0) {
  int numbers1[5] = {1, 2, 3, 4, 5};
  //int numbers2[5] = { 1, 2, 4, 8, 16 };
  int numbers3[2] = {4, 8};
  bool r1 = include(numbers1, numbers1 + 5, numbers3, numbers3 + 2);
  ASSERT_TRUE(!r1);
}
TEST_F(IncludeTest, incl1) {
  vector<int> v1(10);
  vector<int> v2(3);
  int i;
  for (i = 0; (size_t) i < v1.size(); ++i) {
    v1[i] = i;
  }

  bool r1 = include(v1.begin(), v1.end(), v2.begin(), v2.end());
  ASSERT_TRUE(!r1);

  for (i = 0; (size_t) i < v2.size(); ++i)
    v2[i] = i + 3;

  bool r2 = include(v1.begin(), v1.end(), v2.begin(), v2.end());
  ASSERT_TRUE(r2);
}