#include "Iterator/stl_iterator.h"
#include "SequenceContainers/List/stl_list.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class ReviterTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(ReviterTest, reviter1) {
  int array[] = {1, 5, 2, 3};

  vector<int> v(array, array + 4);
  typedef vector<int>::reverse_iterator reviter;
  reviter r(v.rend());
  r--;

  ASSERT_TRUE(*r-- == 1);
  ASSERT_TRUE(*r-- == 5);
  ASSERT_TRUE(*r-- == 2);
  ASSERT_TRUE(*r == 3);
  ASSERT_TRUE(r == v.rbegin());
}
TEST_F(ReviterTest, reviter2) {
  int array[] = {1, 5, 2, 3};

  vector<int> v(array, array + 4);
  vector<int>::reverse_iterator r;
  r = v.rbegin();
  ASSERT_TRUE(*r++ == 3);
  ASSERT_TRUE(*r++ == 2);
  ASSERT_TRUE(*r++ == 5);
  ASSERT_TRUE(*r++ == 1);
  ASSERT_TRUE(r == v.rend());
}
TEST_F(ReviterTest, revbit1) {
  int array[] = {1, 5, 2, 3};

  list<int> v(array, array + 4);
  list<int>::reverse_iterator r(v.rbegin());
  ASSERT_TRUE(*r++ == 3);
  ASSERT_TRUE(*r++ == 2);
  ASSERT_TRUE(*r++ == 5);
  ASSERT_TRUE(*r++ == 1);
  ASSERT_TRUE(r == v.rend());
}
