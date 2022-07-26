#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class LogicTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(LogicTest, logicand) {
  bool input1[4] = {true, true, false, true};
  bool input2[4] = {false, true, false, false};

  bool output[4];
  transform((bool *) input1, (bool *) input1 + 4, (bool *) input2, (bool *) output, logical_and<bool>());

  ASSERT_TRUE(output[0] == false);
  ASSERT_TRUE(output[1] == true);
  ASSERT_TRUE(output[2] == false);
  ASSERT_TRUE(output[3] == false);
}
TEST_F(LogicTest, logicnot) {
  bool input[7] = {true, false, false, true, true, true, true};

  int n = count_if(input, input + 7, logical_not<bool>());
  ASSERT_TRUE(n == 2);
}
TEST_F(LogicTest, logicor) {
  bool input1[4] = {true, true, false, true};
  bool input2[4] = {false, true, false, false};

  bool output[4];
  transform((bool *) input1, (bool *) input1 + 4, (bool *) input2, (bool *) output, logical_or<bool>());

  ASSERT_TRUE(output[0] == true);
  ASSERT_TRUE(output[1] == true);
  ASSERT_TRUE(output[2] == false);
  ASSERT_TRUE(output[3] == true);
}