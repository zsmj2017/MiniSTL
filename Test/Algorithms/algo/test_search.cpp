#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class SearchTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool str_equal(const char *a, const char *b) {
    return strcmp(a, b) == 0;
  }
};

TEST_F(SearchTest, search0) {
  int v1[6] = {1, 1, 2, 3, 5, 8};
  int v2[6] = {0, 1, 2, 3, 4, 5};
  int v3[2] = {3, 4};

  int *location;
  location = search((int *) v1, (int *) v1 + 6, (int *) v3, (int *) v3 + 2);
  ASSERT_TRUE(location == v1 + 6);

  location = search((int *) v2, (int *) v2 + 6, (int *) v3, (int *) v3 + 2);
  ASSERT_TRUE(location != v2 + 6);
  ASSERT_TRUE(location - v2 == 3);
}

TEST_F(SearchTest, search1) {
  typedef vector<int> IntVec;
  IntVec v1(10);
  for (int i = 0; i < (int) v1.size(); ++i) {
    v1[i] = i;
  }
  IntVec v2(3);
  for (int i = 0; i < (int) v2.size(); ++i) {
    v2[i] = i + 50;
  }

  IntVec::iterator location;
  location = search(v1.begin(), v1.end(), v2.begin(), v2.end());

  ASSERT_TRUE(location == v1.end());

  for (int i = 0; i < (int) v2.size(); ++i) {
    v2[i] = i + 4;
  }

  location = search(v1.begin(), v1.end(), v2.begin(), v2.end());

  ASSERT_TRUE(location != v1.end());
  ASSERT_TRUE(location - v1.begin() == 4);
}

TEST_F(SearchTest, search2) {
  char const *grades[] = {"A", "B", "C", "D", "F"};
  char const *letters[] = {"Q", "E", "D"};
  const unsigned gradeCount = sizeof(grades) / sizeof(grades[0]);
  const unsigned letterCount = sizeof(letters) / sizeof(letters[0]);
  char const **location = search((char const **) grades, (char const **) grades + gradeCount, (char const **) letters, (char const **) letters + letterCount, str_equal);

  ASSERT_TRUE(location == grades + gradeCount);

  copy((char const **) grades + 1, (char const **) grades + 1 + letterCount, (char const **) letters);
  location = search((char const **) grades, (char const **) grades + gradeCount, (char const **) letters, (char const **) letters + letterCount, str_equal);

  ASSERT_TRUE(location != grades + gradeCount);
  ASSERT_TRUE(location - grades == 1);
}