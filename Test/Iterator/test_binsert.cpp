#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class BackInsertTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(BackInsertTest, binsert1) {
  const char *array[] = {"laurie", "jennifer", "leisa"};
  vector<const char *> names;
  back_insert_iterator<vector<const char *>> bit(names);
  bit = copy(array, array + 3, bit);

  ASSERT_TRUE(!strcmp(names[0], array[0]));
  ASSERT_TRUE(!strcmp(names[1], array[1]));
  ASSERT_TRUE(!strcmp(names[2], array[2]));

  copy(array, array + 3, bit);
  ASSERT_TRUE(!strcmp(names[3], array[0]));
  ASSERT_TRUE(!strcmp(names[4], array[1]));
  ASSERT_TRUE(!strcmp(names[5], array[2]));
}
TEST_F(BackInsertTest, binsert2) {

  const char *array[] = {"laurie", "jennifer", "leisa"};
  vector<const char *> names;
  copy(array, array + 3, back_inserter(names));
  ASSERT_TRUE(!strcmp(names[0], array[0]));
  ASSERT_TRUE(!strcmp(names[1], array[1]));
  ASSERT_TRUE(!strcmp(names[2], array[2]));
}