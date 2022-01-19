#include "Algorithms/algo/stl_algo.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class GreaterTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

// TODO::sort() need pred
#if 0
TEST_F(GreaterTest, greatert) {
  int array[4] = {3, 1, 4, 2};
  sort(array, array + 4, greater<int>());

  ASSERT_TRUE(array[0] == 4);
  ASSERT_TRUE(array[1] == 3);
  ASSERT_TRUE(array[2] == 2);
  ASSERT_TRUE(array[3] == 1);
}
TEST_F(GreaterTest, greatereq) {
  int array[4] = {3, 1, 4, 2};
  sort(array, array + 4, greater_equal<int>());
  ASSERT_TRUE(array[0] == 4);
  ASSERT_TRUE(array[1] == 3);
  ASSERT_TRUE(array[2] == 2);
  ASSERT_TRUE(array[3] == 1);
}
#endif