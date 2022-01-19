#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class CountTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(CountTest, count0) {
  int numbers[10] = {1, 2, 4, 1, 2, 4, 1, 2, 4, 1};
  int result = count(numbers, numbers + 10, 1);
  ASSERT_TRUE(result == 4);
}
TEST_F(CountTest, count1) {
  vector<int> numbers(100);
  for (int i = 0; i < 100; i++)
    numbers[i] = i % 3;
  int elements = count(numbers.begin(), numbers.end(), 2);
  ASSERT_TRUE(elements == 33);
}
TEST_F(CountTest, countif0) {
  vector<int> numbers(100);
  for (int i = 0; i < 100; i++)
    numbers[i] = i % 3;
  int elements = count_if(numbers.begin(), numbers.end(), odd<int>());
  ASSERT_TRUE(elements == 33);
}
