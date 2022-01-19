#include "Algorithms/algo/stl_algo.h"
#include "Function/stl_function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class LessTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

// TODO::sort() need pred
#if 0
TEST_F(LessTest, lesst) {
  int array[4] = {3, 1, 4, 2};
  sort(array, array + 4, less<int>());

  CPPUNIT_ASSERT(array[0] == 1);
  CPPUNIT_ASSERT(array[1] == 2);
  CPPUNIT_ASSERT(array[2] == 3);
  CPPUNIT_ASSERT(array[3] == 4);
}
TEST_F(LessTest, lesseqt) {
  int array[4] = {3, 1, 4, 2};
  sort(array, array + 4, less_equal<int>());

  CPPUNIT_ASSERT(array[0] == 1);
  CPPUNIT_ASSERT(array[1] == 2);
  CPPUNIT_ASSERT(array[2] == 3);
  CPPUNIT_ASSERT(array[3] == 4);
}
#endif