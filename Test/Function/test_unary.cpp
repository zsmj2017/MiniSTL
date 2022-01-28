#include "Algorithms/algo/stl_algo.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class UnaryTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(UnaryTest, unegate1) {
  int array[3] = {1, 2, 3};
  //unary_negate<odd>::argument_type arg_val = 0;
  int *p = find_if((int *) array, (int *) array + 3, unary_negate<odd<int>>(odd<int>()));
  ASSERT_TRUE((p != array + 3));
  ASSERT_TRUE(*p == 2);
}
TEST_F(UnaryTest, unegate2) {
  int array[3] = {1, 2, 3};
  int *p = find_if((int *) array, (int *) array + 3, not1(odd<int>()));
  ASSERT_TRUE(p != array + 3);
  ASSERT_TRUE(*p == 2);
}

bool test_func(int param) {
  return param < 3;
}
TEST_F(UnaryTest, unegate3) {
  int array[3] = {1, 2, 3};
  int *p = find_if((int *) array, (int *) array + 3, not1(ptr_fun(test_func)));
  ASSERT_TRUE(p != array + 3);
  ASSERT_TRUE(*p == 3);
}