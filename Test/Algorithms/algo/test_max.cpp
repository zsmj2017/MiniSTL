#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class MaxTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool str_compare(const char *a, const char *b) { return strcmp(a, b) < 0; }
};

TEST_F(MaxTest, max1) {
  int r = max(42, 100);
  ASSERT_TRUE(r == 100);

  int t = max(++r, 0);
  ASSERT_TRUE(t == 101);
}
TEST_F(MaxTest, max2) {
  const char *r = max((const char *) "shoe", (const char *) "shine", str_compare);
  ASSERT_TRUE(!strcmp(r, "shoe"));
}
TEST_F(MaxTest, maxelem1) {
  int numbers[6] = {4, 10, 56, 11, -42, 19};

  int *r = max_element((int *) numbers, (int *) numbers + 6);
  ASSERT_TRUE(*r == 56);
}
TEST_F(MaxTest, maxelem2) {
  const char *names[] = {"Brett", "Graham", "Jack", "Mike", "Todd"};

  const unsigned namesCt = sizeof(names) / sizeof(names[0]);
  const char **r = max_element((const char **) names, (const char **) names + namesCt, str_compare);
  ASSERT_TRUE(!strcmp(*r, "Todd"));
}
