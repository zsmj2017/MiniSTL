#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class AdvanceTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

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
