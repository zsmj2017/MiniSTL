#include "Algorithms/algo/stl_algo.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class BcomposTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(BcomposTest, bcompos1) {
  int array[6] = {-2, -1, 0, 1, 2, 3};

  auto b = binary_compose<logical_and<bool>, odd<int>, positive<int>>(logical_and<bool>(), odd<int>(), positive<int>());

  int *p = find_if((int *) array, (int *) array + 6, b);
  ASSERT_TRUE(p != array + 6);
}

TEST_F(BcomposTest, bcompos2) {
  int array[6] = {-2, -1, 0, 1, 2, 3};

  int *p = find_if((int *) array, (int *) array + 6,
                   compose2(logical_and<bool>(), odd<int>(), positive<int>()));
  ASSERT_TRUE(p != array + 6);
}
