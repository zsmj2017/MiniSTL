#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class MismatchTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

bool str_equal(const char *a, const char *b) {
  return strcmp(a, b) == 0;
}

TEST_F(MismatchTest, mismatch0) {
  int n1[5] = {1, 2, 3, 4, 5};
  int n2[5] = {1, 2, 3, 4, 5};
  int n3[5] = {1, 2, 3, 2, 1};

  pair<int *, int *> result = mismatch((int *) n1, (int *) n1 + 5, (int *) n2);
  ASSERT_TRUE(result.first == (n1 + 5) && result.second == (n2 + 5));

  result = mismatch((int *) n1, (int *) n1 + 5, (int *) n3);
  ASSERT_TRUE(!(result.first == (n1 + 5) && result.second == (n3 + 5)));
  ASSERT_TRUE((result.first - n1) == 3);
}
TEST_F(MismatchTest, mismatch1) {
  typedef vector<int> IntVec;
  IntVec v1(10);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }
  IntVec v2(v1);

  pair<IntVec::iterator, IntVec::iterator> result = mismatch(v1.begin(), v1.end(), v2.begin());

  ASSERT_TRUE(result.first == v1.end() && result.second == v2.end());

  v2[v2.size() / 2] = 42;
  result = mismatch(v1.begin(), v1.end(), v2.begin());
  ASSERT_TRUE(!(result.first == v1.end() && result.second == v2.end()));
  ASSERT_TRUE((result.first - v1.begin()) == 5);
}
TEST_F(MismatchTest, mismatch2) {
  const unsigned size = 5;
  char const *n1[size] = {"Brett", "Graham", "Jack", "Mike", "Todd"};

  char const *n2[size];
  copy(n1, n1 + 5, (char const **) n2);
  pair<char const **, char const **> result = mismatch((char const **) n1, (char const **) n1 + size, (char const **) n2, str_equal);

  ASSERT_TRUE(result.first == n1 + size && result.second == n2 + size);

  n2[2] = "QED";
  result = mismatch((char const **) n1, (char const **) n1 + size, (char const **) n2, str_equal);
  ASSERT_TRUE(!(result.first == n2 + size && result.second == n2 + size));
  ASSERT_TRUE((result.first - n1) == 2);
}