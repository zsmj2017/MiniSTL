#include "Iterator/stl_iterator.h"
#include "SequenceContainers/Deque/stl_deque.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class InsertTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(InsertTest, insert1) {
  char const *array1[] = {"laurie", "jennifer", "leisa"};
  char const *array2[] = {"amanda", "saskia", "carrie"};

  deque<char const *> names(array1, array1 + 3);
  deque<char const *>::iterator i = names.begin() + 2;

  insert_iterator<deque<char const *>> itd(names, i);
  itd = copy(array2, array2 + 3, insert_iterator<deque<char const *>>(names, i));

  ASSERT_TRUE(!strcmp(names[0], "laurie"));
  ASSERT_TRUE(!strcmp(names[1], "jennifer"));
  ASSERT_TRUE(!strcmp(names[2], "amanda"));
  ASSERT_TRUE(!strcmp(names[3], "saskia"));
  ASSERT_TRUE(!strcmp(names[4], "carrie"));
  ASSERT_TRUE(!strcmp(names[5], "leisa"));

  copy(array1, array1 + 3, itd);
  ASSERT_TRUE(!strcmp(names[5], "laurie"));
  ASSERT_TRUE(!strcmp(names[6], "jennifer"));
  ASSERT_TRUE(!strcmp(names[7], "leisa"));
  ASSERT_TRUE(!strcmp(names[8], "leisa"));
}

TEST_F(InsertTest, insert2) {
  char const *array1[] = {"laurie", "jennifer", "leisa"};
  char const *array2[] = {"amanda", "saskia", "carrie"};

  deque<char const *> names(array1, array1 + 3);
  deque<char const *>::iterator i = names.begin() + 2;
  copy(array2, array2 + 3, inserter(names, i));

  ASSERT_TRUE(!strcmp(names[0], "laurie"));
  ASSERT_TRUE(!strcmp(names[1], "jennifer"));
  ASSERT_TRUE(!strcmp(names[2], "amanda"));
  ASSERT_TRUE(!strcmp(names[3], "saskia"));
  ASSERT_TRUE(!strcmp(names[4], "carrie"));
  ASSERT_TRUE(!strcmp(names[5], "leisa"));
}