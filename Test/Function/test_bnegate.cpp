#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class BnegateTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(BnegateTest, bnegate1) {
  int array [4] = { 4, 9, 7, 1 };
  sort(array, array + 4, binary_negate<greater<int> >(greater<int>()));
  ASSERT_TRUE(array[0]==1);
  ASSERT_TRUE(array[1]==4);
  ASSERT_TRUE(array[2]==7);
  ASSERT_TRUE(array[3]==9);
}
TEST_F(BnegateTest, bnegate2) {
  int array [4] = { 4, 9, 7, 1 };
  sort(array, array + 4, not2(greater<int>()));
  ASSERT_TRUE(array[0]==1);
  ASSERT_TRUE(array[1]==4);
  ASSERT_TRUE(array[2]==7);
  ASSERT_TRUE(array[3]==9);
}
