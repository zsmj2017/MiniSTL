#include "Algorithms/algo/stl_algo.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class MinTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool str_compare(const char *a, const char *b) { return strcmp(a, b) < 0; }
};

TEST_F(MinTest, min1) {
  int r = min(42, 100);
  ASSERT_TRUE(r == 42);

  r = min(--r, r);
  ASSERT_TRUE(r == 41);
}
TEST_F(MinTest, min2) {
  const char *r = min((const char *) "shoe", (const char *) "shine", str_compare);
  ASSERT_TRUE(!strcmp(r, "shine"));
}
TEST_F(MinTest, minelem1) {
  int numbers[6] = {-10, 15, -100, 36, -242, 42};
  int *r = min_element((int *) numbers, (int *) numbers + 6);
  ASSERT_TRUE(*r == -242);
}
TEST_F(MinTest, minelem2) {
  const char *names[] = {"Brett", "Graham", "Jack", "Mike", "Todd"};

  const unsigned namesCt = sizeof(names) / sizeof(names[0]);
  const char **r = min_element((const char **) names, (const char **) names + namesCt, str_compare);
  ASSERT_TRUE(!strcmp(*r, "Brett"));
}