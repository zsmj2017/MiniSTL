#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class NeqTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(NeqTest, neq0) {
  int input1[4] = {1, 7, 2, 2};
  int input2[4] = {1, 6, 2, 3};

  int output[4];
  transform((int *) input1, (int *) input1 + 4, (int *) input2, (int *) output, not_equal_to<int>());

  ASSERT_TRUE(output[0] == 0);
  ASSERT_TRUE(output[1] == 1);
  ASSERT_TRUE(output[2] == 0);
  ASSERT_TRUE(output[3] == 1);
}
