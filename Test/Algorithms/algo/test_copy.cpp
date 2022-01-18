#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class CopyTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool char_str_less(const char *a_, const char *b_) {
    return strcmp(a_, b_) < 0;
  }
};

TEST_F(CopyTest, copy_array) {
  char string[23] = "A string to be copied.";
  char result[23];
  copy(string, string + 23, result);
  ASSERT_TRUE(!strncmp(string, result, 23));
}

TEST_F(CopyTest, copy_volatile) {
  {
    int a[] = {0, 1, 2, 3, 4, 5};
    const size_t size = sizeof(a) / sizeof(a[0]);
    volatile int va[size];
    copy(a, a + size, va);
    for (size_t i = 0; i != size; ++i) {
      ASSERT_TRUE(a[i] == va[i]);
    }
  }

  {
    const int a[] = {0, 1, 2, 3, 4, 5};
    const size_t size = sizeof(a) / sizeof(a[0]);
    volatile int va[size];
    copy(a, a + size, va);
    for (size_t i = 0; i != size; ++i) {
      ASSERT_TRUE(a[i] == va[i]);
    }
  }
}

TEST_F(CopyTest, copy_vector) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i;

  vector<int> v2(v1.size());
  copy(v1.begin(), v1.end(), v2.begin());

  ASSERT_TRUE(v2 == v1);
}

TEST_F(CopyTest, copy_insert) {
  vector<int> v1(10);
  for (int loc = 0; (size_t) loc < v1.size(); ++loc)
    v1[loc] = loc;
  vector<int> v2;
  insert_iterator<vector<int>> i(v2, v2.begin());
  copy(v1.begin(), v1.end(), i);

  ASSERT_TRUE(v2 == v1);
}

TEST_F(CopyTest, copy_back) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i;
  vector<int> v2(v1.size());
  copy_backward(v1.begin(), v1.end(), v2.end());

  ASSERT_TRUE(v2 == v1);
}

TEST_F(CopyTest, copy_back_array) {
  int numbers[5] = {1, 2, 3, 4, 5};

  int result[5];
  copy_backward(numbers, numbers + 5, (int *) result + 5);
  ASSERT_TRUE(result[0] == numbers[0]);
  ASSERT_TRUE(result[1] == numbers[1]);
  ASSERT_TRUE(result[2] == numbers[2]);
  ASSERT_TRUE(result[3] == numbers[3]);
  ASSERT_TRUE(result[4] == numbers[4]);
}