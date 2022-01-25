#include "Algorithms/algo/stl_algo.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class LexcmpTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(LexcmpTest, lexcmp1) {
  const unsigned size = 6;
  char n1[size] = "shoe";
  char n2[size] = "shine";

  bool before = lexicographical_compare(n1, n1 + size, n2, n2 + size);
  ASSERT_TRUE(!before);
}
TEST_F(LexcmpTest, lexcmp2) {
  const unsigned size = 6;
  char n1[size] = "shoe";
  char n2[size] = "shine";

  bool before = lexicographical_compare(n1, n1 + size, n2, n2 + size, greater<char>());
  ASSERT_TRUE(before);
}