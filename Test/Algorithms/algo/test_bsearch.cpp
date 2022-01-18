#include "Algorithms/algo/stl_algo.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class BsearchTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool char_str_less(const char *a_, const char *b_) {
    return strcmp(a_, b_) < 0;
  }
};

TEST_F(BsearchTest, bsearch1) {
  int vector[100];
  for(int i = 0; i < 100; i++)
    vector[i] = i;
  ASSERT_TRUE(binary_search(vector, vector + 100, 42));
}

#if 0
bool str_compare(const char* a_, const char* b_)
{
  return strcmp(a_, b_) < 0 ? 1 : 0;
}

TEST_F(BsearchTest, bsearch2) {
  char const* labels[] = { "aa", "dd", "ff", "jj", "ss", "zz" };
  const unsigned count = sizeof(labels) / sizeof(labels[0]);
  // DEC C++ generates incorrect template instatiation code
  // for "ff" so must cast
  ASSERT_TRUE(binary_search(labels, labels + count, (const char *)"ff", str_compare));
}
#endif
