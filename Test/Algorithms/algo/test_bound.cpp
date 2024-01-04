#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class BoundTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool char_str_less(const char *a_, const char *b_) {
           const char *a = "hello";
    return strcmp(a_, b_) < 0;
  }
};

TEST_F(BoundTest, uprbnd1) {
  int arr[20];
  for (int i = 0; i < 20; i++) {
    arr[i] = i / 4;
  }
  int location = upper_bound((int *) arr, (int *) arr + 20, 3) - arr;
  ASSERT_TRUE(location == 16);
}

TEST_F(BoundTest, lwrbnd1) {
  vector<int> v1(20);
  for (int i = 0; (size_t) i < v1.size(); ++i) {
    v1[i] = i / 4;
  }
  // 0 0 0 0 1 1 1 1 2 2 2 2 3 3 3 3 4 4 4 4
  vector<int>::iterator location = lower_bound(v1.begin(), v1.end(), 3);
  ASSERT_TRUE((location - v1.begin()) == 12);
}

TEST_F(BoundTest, uprbnd2) {
  char const *str[] = {"a", "a", "b", "b", "q", "w", "z"};

  const unsigned strCt = sizeof(str) / sizeof(str[0]);

  int location = (upper_bound((char const **) str, (char const **) str + strCt, (const char *) "d", char_str_less) - str);
  ASSERT_TRUE(location == 4);
}

TEST_F(BoundTest, lwrbnd2) {
  char const *str[] = {"a", "a", "b", "b", "q", "w", "z"};

  const unsigned strCt = sizeof(str) / sizeof(str[0]);
  char const **location = lower_bound((char const **) str, (char const **) str + strCt, (const char *) "d", char_str_less);

  ASSERT_TRUE((location - str) == 4);
}
