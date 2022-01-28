#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class UniqueTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool str_equal(const char *a, const char *b) {
    return strcmp(a, b) == 0;
  }
};

TEST_F(UniqueTest, unique1) {
  int numbers[8] = {0, 1, 1, 2, 2, 2, 3, 4};
  unique((int *) numbers, (int *) numbers + 8);
  // 0 1 2 3 4 2 3 4
  ASSERT_TRUE(numbers[0] == 0);
  ASSERT_TRUE(numbers[1] == 1);
  ASSERT_TRUE(numbers[2] == 2);
  ASSERT_TRUE(numbers[3] == 3);
  ASSERT_TRUE(numbers[4] == 4);
  ASSERT_TRUE(numbers[5] == 2);
  ASSERT_TRUE(numbers[6] == 3);
  ASSERT_TRUE(numbers[7] == 4);
}

// TODO::unique_copy() need pred

#if 0
TEST_F(UniqueTest, unique2) {
  const char *labels[] = {"Q", "Q", "W", "W", "E", "E", "R", "T", "T", "Y", "Y"};

  const unsigned count = sizeof(labels) / sizeof(labels[0]);

  unique((const char **) labels, (const char **) labels + count, str_equal);

  // QWERTY
  ASSERT_TRUE(*labels[0] == 'Q');
  ASSERT_TRUE(*labels[1] == 'W');
  ASSERT_TRUE(*labels[2] == 'E');
  ASSERT_TRUE(*labels[3] == 'R');
  ASSERT_TRUE(*labels[4] == 'T');
  ASSERT_TRUE(*labels[5] == 'Y');
}
#endif

TEST_F(UniqueTest, uniqcpy1) {
  int numbers[8] = {0, 1, 1, 2, 2, 2, 3, 4};
  int result[8] = {0, 0, 0, 0, 0, 0, 0, 0};

  unique_copy((int *) numbers, (int *) numbers + 8, (int *) result);

  // 0 1 2 3 4 0 0 0
  ASSERT_TRUE(result[0] == 0);
  ASSERT_TRUE(result[1] == 1);
  ASSERT_TRUE(result[2] == 2);
  ASSERT_TRUE(result[3] == 3);
  ASSERT_TRUE(result[4] == 4);
  ASSERT_TRUE(result[5] == 0);
  ASSERT_TRUE(result[6] == 0);
  ASSERT_TRUE(result[7] == 0);
}

// TODO::unique_copy() need pred
#if 0
TEST_F(UniqueTest, uniqcpy2) {
  const char *labels[] = {"Q", "Q", "W", "W", "E", "E", "R", "T", "T", "Y", "Y"};
  const char **plabels = (const char **) labels;

  const size_t count = sizeof(labels) / sizeof(labels[0]);
  const char *uCopy[count];
  const char **puCopy = &uCopy[0];
  fill(puCopy, puCopy + count, "");

  unique_copy(plabels, plabels + count, puCopy, str_equal);

  //QWERTY
  ASSERT_TRUE(*uCopy[0] == 'Q');
  ASSERT_TRUE(*uCopy[1] == 'W');
  ASSERT_TRUE(*uCopy[2] == 'E');
  ASSERT_TRUE(*uCopy[3] == 'R');
  ASSERT_TRUE(*uCopy[4] == 'T');
  ASSERT_TRUE(*uCopy[5] == 'Y');
}
#endif