#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include "string"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
using std::string;

class Ptr2Test : public testing::Test {
 protected:
  void SetUp() override {
  }
  static int sum(int x, int y) {
    return x + y;
  }
  static bool even(int n) {
    return (n % 2) == 0;
  }
};

TEST_F(Ptr2Test, ptrbin1) {
  int input1[4] = {7, 2, 3, 5};
  int input2[4] = {1, 5, 5, 8};

  int output[4];
  transform((int *) input1, (int *) input1 + 4, (int *) input2, (int *) output, pointer_to_binary_function<int, int, int>(sum));

  ASSERT_TRUE(output[0] == 8);
  ASSERT_TRUE(output[1] == 7);
  ASSERT_TRUE(output[2] == 8);
  ASSERT_TRUE(output[3] == 13);
}
TEST_F(Ptr2Test, ptrbin2) {
  int input1[4] = {7, 2, 3, 5};
  int input2[4] = {1, 5, 5, 8};

  int output[4];
  transform((int *) input1, (int *) input1 + 4, (int *) input2, (int *) output, ptr_fun(sum));

  ASSERT_TRUE(output[0] == 8);
  ASSERT_TRUE(output[1] == 7);
  ASSERT_TRUE(output[2] == 8);
  ASSERT_TRUE(output[3] == 13);
}
TEST_F(Ptr2Test, ptrun1) {
  int array[3] = {1, 2, 3};

  int *p = find_if((int *) array, (int *) array + 3, pointer_to_unary_function<int, bool>(even));
  ASSERT_TRUE(p != array + 3);
  ASSERT_TRUE(*p == 2);
}
TEST_F(Ptr2Test, ptrun2) {
  int array[3] = {1, 2, 3};

  int *p = find_if((int *) array, (int *) array + 3, ptr_fun(even));
  ASSERT_TRUE(p != array + 3);
  ASSERT_TRUE(*p == 2);
}
