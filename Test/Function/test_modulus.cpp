#include "Algorithms/algo/stl_algo.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class ModulusTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(ModulusTest, modulus0) {
  int input1[4] = {6, 8, 10, 2};
  int input2[4] = {4, 2, 11, 3};

  int output[4];

  transform((int *) input1, (int *) input1 + 4, (int *) input2, (int *) output, modulus<int>());
  ASSERT_TRUE(output[0] == 2);
  ASSERT_TRUE(output[1] == 0);
  ASSERT_TRUE(output[2] == 10);
  ASSERT_TRUE(output[3] == 2);
}
