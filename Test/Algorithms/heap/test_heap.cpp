#include "Algorithms/heap/heap_algorithm.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class HeapTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(HeapTest, mkheap0) {
  int numbers[6] = {5, 10, 4, 13, 11, 19};

  make_heap(numbers, numbers + 6);
  ASSERT_TRUE(numbers[0] == 19);
  pop_heap(numbers, numbers + 6);
  ASSERT_TRUE(numbers[0] == 13);
  pop_heap(numbers, numbers + 5);
  ASSERT_TRUE(numbers[0] == 11);
  pop_heap(numbers, numbers + 4);
  ASSERT_TRUE(numbers[0] == 10);
  pop_heap(numbers, numbers + 3);
  ASSERT_TRUE(numbers[0] == 5);
  pop_heap(numbers, numbers + 2);
  ASSERT_TRUE(numbers[0] == 4);
  pop_heap(numbers, numbers + 1);
}
TEST_F(HeapTest, mkheap1) {
  int numbers[6] = {5, 10, 4, 13, 11, 19};

  make_heap(numbers, numbers + 6, greater<int>());

  ASSERT_TRUE(numbers[0] == 4);
  pop_heap(numbers, numbers + 6, greater<int>());
  ASSERT_TRUE(numbers[0] == 5);
  pop_heap(numbers, numbers + 5, greater<int>());
  ASSERT_TRUE(numbers[0] == 10);
  pop_heap(numbers, numbers + 4, greater<int>());
  ASSERT_TRUE(numbers[0] == 11);
  pop_heap(numbers, numbers + 3, greater<int>());
  ASSERT_TRUE(numbers[0] == 13);
  pop_heap(numbers, numbers + 2, greater<int>());
  ASSERT_TRUE(numbers[0] == 19);
}
TEST_F(HeapTest, pheap1) {
  vector<int> v;

  v.push_back(1);
  v.push_back(20);
  v.push_back(4);
  make_heap(v.begin(), v.end());

  v.push_back(7);
  push_heap(v.begin(), v.end());

  sort_heap(v.begin(), v.end());

  ASSERT_TRUE(v[0] == 1);
  ASSERT_TRUE(v[1] == 4);
  ASSERT_TRUE(v[2] == 7);
  ASSERT_TRUE(v[3] == 20);
}
TEST_F(HeapTest, pheap2) {
  vector<int> v;

  v.push_back(1);
  v.push_back(20);
  v.push_back(4);
  make_heap(v.begin(), v.end(), greater<int>());

  v.push_back(7);
  push_heap(v.begin(), v.end(), greater<int>());

  sort_heap(v.begin(), v.end(), greater<int>());

  ASSERT_TRUE(v[0] == 20);
  ASSERT_TRUE(v[1] == 7);
  ASSERT_TRUE(v[2] == 4);
  ASSERT_TRUE(v[3] == 1);
}