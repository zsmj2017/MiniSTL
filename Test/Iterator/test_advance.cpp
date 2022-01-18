#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class AdvanceTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static int equal_length(const char *v1_, const char *v2_);
  static int mult(int initial_, int element_);
};

int AdvanceTest::mult(int initial_, int element_) {
  return initial_ * element_;
}

int AdvanceTest::equal_length(const char *v1_, const char *v2_) {
  return std::strlen(v1_) == std::strlen(v2_);
}

TEST_F(AdvanceTest, adv) {
  typedef vector<int> IntVector;
  IntVector v(10);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i;
  IntVector::iterator location = v.begin();
  ASSERT_TRUE(*location == 0);
  advance(location, 5);
  ASSERT_TRUE(*location == 5);
}
