#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class PartialTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(PartialTest, parsrt0) {
  int numbers[6] = {5, 2, 4, 3, 1, 6};

  partial_sort((int *) numbers, (int *) numbers + 3, (int *) numbers + 6);

  // 1 2 3 5 4 6
  ASSERT_TRUE(numbers[0] == 1);
  ASSERT_TRUE(numbers[1] == 2);
  ASSERT_TRUE(numbers[2] == 3);
  ASSERT_TRUE(numbers[3] == 5);
  ASSERT_TRUE(numbers[4] == 4);
  ASSERT_TRUE(numbers[5] == 6);
}

TEST_F(PartialTest, parsrt1) {
  // 8 8 5 3 7 6 5 3 2 4
  // 2 3 3 4 5 8 8 7 6 5
  int numbers[10] = {8, 8, 5, 3, 7, 6, 5, 3, 2, 4};

  vector<int> v1(numbers, numbers + 10);
  partial_sort(v1.begin(), v1.begin() + v1.size() / 2, v1.end());

  ASSERT_TRUE(v1[0] == 2);
  ASSERT_TRUE(v1[1] == 3);
  ASSERT_TRUE(v1[2] == 3);
  ASSERT_TRUE(v1[3] == 4);
  ASSERT_TRUE(v1[4] == 5);
  ASSERT_TRUE(v1[5] == 8);
  ASSERT_TRUE(v1[6] == 8);
  ASSERT_TRUE(v1[7] == 7);
  ASSERT_TRUE(v1[8] == 6);
  ASSERT_TRUE(v1[9] == 5);
}

bool str_compare(const char *a_, const char *b_) {
  return strcmp(a_, b_) < 0 ? 1 : 0;
}

TEST_F(PartialTest, parsrt2) {
  char const *names[] = {"aa", "ff", "dd", "ee", "cc", "bb"};

  const unsigned nameSize = sizeof(names) / sizeof(names[0]);
  vector<char const *> v1(nameSize);
  for (size_t i = 0; i < v1.size(); i++)
    v1[i] = names[i];

  partial_sort(v1.begin(), v1.begin() + nameSize / 2, v1.end(), str_compare);

  // aa bb cc ff ee dd
  ASSERT_TRUE(strcmp(v1[0], "aa") == 0);
  ASSERT_TRUE(v1[0] == names[0]);
  ASSERT_TRUE(strcmp(v1[1], "bb") == 0);
  ASSERT_TRUE(v1[1] == names[5]);
  ASSERT_TRUE(strcmp(v1[2], "cc") == 0);
  ASSERT_TRUE(v1[2] == names[4]);
  ASSERT_TRUE(strcmp(v1[3], "ff") == 0);
  ASSERT_TRUE(v1[3] == names[1]);
  ASSERT_TRUE(strcmp(v1[4], "ee") == 0);
  ASSERT_TRUE(v1[4] == names[3]);
  ASSERT_TRUE(strcmp(v1[5], "dd") == 0);
  ASSERT_TRUE(v1[5] == names[2]);
}