#include "Algorithms/numeric/stl_numeric.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class InnerprodTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static size_t add(size_t a, size_t b) {
    return a + b;
  }

  static size_t mult(size_t a, size_t b) {
    return a * b;
  }
};

TEST_F(InnerprodTest, inprod0) {
  int vector1[5] = {1, 2, 3, 4, 5};
  int vector2[5] = {1, 2, 3, 4, 5};

  int result;
  result = inner_product(vector1, vector1 + 5, vector2, 0);
  ASSERT_TRUE(result == 55);
}
TEST_F(InnerprodTest, inprod1) {
  vector<size_t> v1(3);
  vector<size_t> v2(v1.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    v1[i] = i + 1;
    v2[i] = v1.size() - i;
  }
  size_t result = inner_product(v1.begin(), v1.end(), v2.begin(), (size_t) 0);
  ASSERT_TRUE(result == 10);
}
TEST_F(InnerprodTest, inprod2) {
  vector<size_t> v1(3);
  vector<size_t> v2(v1.size());
  for (size_t i = 0; i < v1.size(); ++i) {
    v1[i] = i + 1;
    v2[i] = v1.size() - i;
  }
  size_t result = inner_product(v1.begin(), v1.end(), v2.begin(), (size_t) 1, mult, add);
  ASSERT_TRUE(result == 64);
}
