#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class NegateTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(NegateTest, negate0) {
  int input[3] = {1, 2, 3};

  int output[3];
  transform((int *) input, (int *) input + 3, (int *) output, negate<int>());

  ASSERT_TRUE(output[0] == -1);
  ASSERT_TRUE(output[1] == -2);
  ASSERT_TRUE(output[2] == -3);
}
