#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class PartitionTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  struct less_n {
    less_n(int limit, size_t &nb_calls)
        : limit_(limit), nb_calls_(nb_calls) {}

    bool operator()(int a) const {
      ++nb_calls_;
      return a < limit_;
    }

    int limit_;
    size_t &nb_calls_;

   private:
    //explicitely defined as private to avoid warnings:
    less_n &operator=(less_n const &) = delete;
  };
};

// TODO:: need stable_partition
#if 0
TEST_F(PartitionTest, stblptn0) {
  int numbers[6] = {10, 5, 11, 20, 6, -2};

  size_t nb_pred_calls = 0;
  stable_partition((int *) numbers, (int *) numbers + 6, less_n(10, nb_pred_calls));
  // 5 6 -2 10 11 20
  ASSERT_TRUE(numbers[0] == 5);
  ASSERT_TRUE(numbers[1] == 6);
  ASSERT_TRUE(numbers[2] == -2);
  ASSERT_TRUE(numbers[3] == 10);
  ASSERT_TRUE(numbers[4] == 11);
  ASSERT_TRUE(numbers[5] == 20);

  //Complexity check:
  ASSERT_TRUE(nb_pred_calls == sizeof(numbers) / sizeof(numbers[0]));
}
TEST_F(PartitionTest, stblptn1) {
  //5 5 2 10 0 12 5 0 0 19
  //5 5 2 10 0 5 0 0 12 19
  int numbers[] = {5, 5, 2, 10, 0, 12, 5, 0, 0, 19};
  vector<int> v1(numbers, numbers + 10);

  size_t nb_pred_calls = 0;
  stable_partition(v1.begin(), v1.end(), less_n(11, nb_pred_calls));

  ASSERT_TRUE(v1[0] == 5);
  ASSERT_TRUE(v1[1] == 5);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 10);
  ASSERT_TRUE(v1[4] == 0);
  ASSERT_TRUE(v1[5] == 5);
  ASSERT_TRUE(v1[6] == 0);
  ASSERT_TRUE(v1[7] == 0);
  ASSERT_TRUE(v1[8] == 12);
  ASSERT_TRUE(v1[9] == 19);
  ASSERT_TRUE(nb_pred_calls == v1.size());
}
#endif
TEST_F(PartitionTest, ptition0) {
  int numbers[6] = {6, 12, 3, 10, 1, 20};
  size_t nb_pred_calls = 0;
  // 6 1 3 10 12 20
  partition((int *) numbers, (int *) numbers + 6, less_n(10, nb_pred_calls));
  ASSERT_TRUE(numbers[0] == 6);
  ASSERT_TRUE(numbers[1] == 1);
  ASSERT_TRUE(numbers[2] == 3);
  ASSERT_TRUE(numbers[3] == 10);
  ASSERT_TRUE(numbers[4] == 12);
  ASSERT_TRUE(numbers[5] == 20);

  ASSERT_TRUE(nb_pred_calls == sizeof(numbers) / sizeof(numbers[0]));
}
TEST_F(PartitionTest, ptition1) {
  // 19 3 11 14 10 19 8 17 9 6
  // 6 3 9 8 10 19 14 17 11 19

  int numbers[10] = {19, 3, 11, 14, 10, 19, 8, 17, 9, 6};

  vector<int> v1(numbers, numbers + 10);
  size_t nb_pred_calls = 0;
  partition(v1.begin(), v1.end(), less_n(11, nb_pred_calls));

  ASSERT_TRUE(v1[0] == 6);
  ASSERT_TRUE(v1[1] == 3);
  ASSERT_TRUE(v1[2] == 9);
  ASSERT_TRUE(v1[3] == 8);
  ASSERT_TRUE(v1[4] == 10);
  ASSERT_TRUE(v1[5] == 19);
  ASSERT_TRUE(v1[6] == 14);
  ASSERT_TRUE(v1[7] == 17);
  ASSERT_TRUE(v1[8] == 11);
  ASSERT_TRUE(v1[9] == 19);
  ASSERT_TRUE(nb_pred_calls == v1.size());
}
