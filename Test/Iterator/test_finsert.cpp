#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Deque/stl_deque.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class FinsertTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(FinsertTest, finsert1) {
  char const *array[] = {"laurie", "jennifer", "leisa"};
  deque<char const *> names;
  front_insert_iterator<deque<char const *>> fit(names);
  fit = copy(array, array + 3, front_insert_iterator<deque<char const *>>(names));

  ASSERT_TRUE(names[0] == array[2]);
  ASSERT_TRUE(names[1] == array[1]);
  ASSERT_TRUE(names[2] == array[0]);

  copy(array, array + 3, fit);
  ASSERT_TRUE(names[3] == array[2]);
  ASSERT_TRUE(names[4] == array[1]);
  ASSERT_TRUE(names[5] == array[0]);
}

TEST_F(FinsertTest, finsert2) {
  char const *array[] = {"laurie", "jennifer", "leisa"};

  deque<char const *> names;
  copy(array, array + 3, front_inserter(names));

  ASSERT_TRUE(names[0] == array[2]);
  ASSERT_TRUE(names[1] == array[1]);
  ASSERT_TRUE(names[2] == array[0]);
}
