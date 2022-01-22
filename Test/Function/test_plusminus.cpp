#include "Algorithms/algo/stl_algo.h"
#include "Algorithms/numeric/stl_numeric.h"
#include "Function/stl_function.h"
#include "string"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
using std::string;

class PlusMinusTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(PlusMinusTest, plus0) {
  int input1[4] = {1, 6, 11, 8};
  int input2[4] = {1, 5, 2, 3};

  int total = inner_product(input1, input1 + 4, input2, 0, plus<int>(), multiplies<int>());

  ASSERT_TRUE(total == 77);
}

TEST_F(PlusMinusTest, minus0) {
  int input1[4] = {1, 5, 7, 8};
  int input2[4] = {1, 4, 8, 3};

  int output[4];

  transform((int *) input1, (int *) input1 + 4, (int *) input2, (int *) output, minus<int>());
  ASSERT_TRUE(output[0] == 0);
  ASSERT_TRUE(output[1] == 1);
  ASSERT_TRUE(output[2] == -1);
  ASSERT_TRUE(output[3] == 5);
}