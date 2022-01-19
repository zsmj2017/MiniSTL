#include "Algorithms/numeric/stl_numeric.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class DivideTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(DivideTest, div) {
  int input[3] = {2, 3, 4};
  int result = accumulate(input, input + 3, 48, divides<int>());
  ASSERT_TRUE(result == 2);
}
