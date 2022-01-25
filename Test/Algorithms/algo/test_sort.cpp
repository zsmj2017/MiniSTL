#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class SortTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool string_less(const char *a, const char *b) {
    return strcmp(a, b) < 0;
  }
};

// TODO::need stable_sort()
#if 0
TEST_F(SortTest, stblsrt1) {
  //Check that stable_sort do sort
  int numbers[6] = {1, 50, -10, 11, 42, 19};
  stable_sort(numbers, numbers + 6);
  //-10 1 11 19 42 50
  ASSERT_TRUE(numbers[0] == -10);
  ASSERT_TRUE(numbers[1] == 1);
  ASSERT_TRUE(numbers[2] == 11);
  ASSERT_TRUE(numbers[3] == 19);
  ASSERT_TRUE(numbers[4] == 42);
  ASSERT_TRUE(numbers[5] == 50);

  char const *letters[6] = {"bb", "aa", "ll", "dd", "qq", "cc"};
  stable_sort(letters, letters + 6, string_less);
  // aa bb cc dd ll qq
  ASSERT_TRUE(strcmp(letters[0], "aa") == 0);
  ASSERT_TRUE(strcmp(letters[1], "bb") == 0);
  ASSERT_TRUE(strcmp(letters[2], "cc") == 0);
  ASSERT_TRUE(strcmp(letters[3], "dd") == 0);
  ASSERT_TRUE(strcmp(letters[4], "ll") == 0);
  ASSERT_TRUE(strcmp(letters[5], "qq") == 0);
}



TEST_F(SortTest, stblsrt2) {
  struct Data {
    Data(int index, int value)
        : m_index(index), m_value(value) {}

    bool operator==(const Data &other) const { return m_index == other.m_index && m_value == other.m_value; }
    bool operator<(const Data &other) const { return m_value < other.m_value; }

   private:
    int m_index, m_value;
  };

  //Check that stable_sort is stable:
  Data datas[] = {
      Data(0, 10),
      Data(1, 8),
      Data(2, 6),
      Data(3, 6),
      Data(4, 6),
      Data(5, 4),
      Data(6, 9)};
  stable_sort(datas, datas + 7);

  ASSERT_TRUE(datas[0] == Data(5, 4));
  ASSERT_TRUE(datas[1] == Data(2, 6));
  ASSERT_TRUE(datas[2] == Data(3, 6));
  ASSERT_TRUE(datas[3] == Data(4, 6));
  ASSERT_TRUE(datas[4] == Data(1, 8));
  ASSERT_TRUE(datas[5] == Data(6, 9));
  ASSERT_TRUE(datas[6] == Data(0, 10));
}
#endif

TEST_F(SortTest, sort1) {
  int numbers[6] = {1, 50, -10, 11, 42, 19};

  sort(numbers, numbers + 6);
  // -10 1 11 19 42 50
  ASSERT_TRUE(numbers[0] == -10);
  ASSERT_TRUE(numbers[1] == 1);
  ASSERT_TRUE(numbers[2] == 11);
  ASSERT_TRUE(numbers[3] == 19);
  ASSERT_TRUE(numbers[4] == 42);
  ASSERT_TRUE(numbers[5] == 50);
}

// TODO:: sort() need pred
#if 0
TEST_F(SortTest, sort2) {
  int numbers[] = {1, 50, -10, 11, 42, 19};

  int count = sizeof(numbers) / sizeof(numbers[0]);
  sort(numbers, numbers + count, greater<int>());

  //  50 42 19 11 1 -10
  ASSERT_TRUE(numbers[5] == -10);
  ASSERT_TRUE(numbers[4] == 1);
  ASSERT_TRUE(numbers[3] == 11);
  ASSERT_TRUE(numbers[2] == 19);
  ASSERT_TRUE(numbers[1] == 42);
  ASSERT_TRUE(numbers[0] == 50);
}
#endif

TEST_F(SortTest, sort3) {
  vector<bool> boolVector;

  boolVector.push_back(true);
  boolVector.push_back(false);

  sort(boolVector.begin(), boolVector.end());

  ASSERT_TRUE(boolVector[0] == false);
  ASSERT_TRUE(boolVector[1] == true);
}

/*
 * A small utility class to check a potential compiler bug
 * that can result in a bad sort algorithm behavior. The type
 * _Tp of the SortTestFunc has to be SortTestAux without any
 * reference qualifier.
 */
struct SortTestAux {
  SortTestAux(bool &b) : b_(b) {}

  SortTestAux(SortTestAux const &other) : b_(other.b_) {
    b_ = true;
  }

  bool &b_;

 private:
  //explicitely defined as private to avoid warnings:
  SortTestAux &operator=(SortTestAux const &) = delete;
};

template<class T>
void SortTestFunc(T) {
}

TEST_F(SortTest, sort4) {
  bool copy_constructor_called = false;
  SortTestAux instance(copy_constructor_called);
  SortTestAux &r_instance = instance;
  SortTestAux const &rc_instance = instance;

  SortTestFunc(r_instance);
  ASSERT_TRUE(copy_constructor_called);
  copy_constructor_called = false;
  SortTestFunc(rc_instance);
  ASSERT_TRUE(copy_constructor_called);
}
