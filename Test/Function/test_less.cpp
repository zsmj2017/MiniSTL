#include "Algorithms/algo/stl_algo.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class LessTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(LessTest, lesst) {
  int array[4] = {3, 1, 4, 2};
  sort(array, array + 4, less<int>());

  ASSERT_TRUE(array[0] == 1);
  ASSERT_TRUE(array[1] == 2);
  ASSERT_TRUE(array[2] == 3);
  ASSERT_TRUE(array[3] == 4);
}
TEST_F(LessTest, lesseqt) {
  int array[4] = {3, 1, 4, 2};
  sort(array, array + 4, less_equal<int>());

  ASSERT_TRUE(array[0] == 1);
  ASSERT_TRUE(array[1] == 2);
  ASSERT_TRUE(array[2] == 3);
  ASSERT_TRUE(array[3] == 4);
}