#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class FindTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool odd(int a) {
    return (a % 2) != 0;
  }

  static bool div_3(int a) {
    return a % 3 == 0;
  }
};

TEST_F(FindTest, find0) {
  int numbers[10] = {0, 1, 4, 9, 16, 25, 36, 49, 64};

  int *location = find((int *) numbers, (int *) numbers + 10, 25);

  ASSERT_TRUE((location - numbers) == 5);

  int *out_range = find((int *) numbers, (int *) numbers + 10, 128);

  ASSERT_TRUE(out_range == (int *) (numbers + 10));
}

TEST_F(FindTest, find1) {
  int years[] = {1942, 1952, 1962, 1972, 1982, 1992};

  const unsigned yearCount = sizeof(years) / sizeof(years[0]);
  int *location = find((int *) years, (int *) years + yearCount, 1972);

  ASSERT_TRUE((location - years) == 3);
}

TEST_F(FindTest, findif0) {
  {
    int numbers[6] = {2, 4, 8, 15, 32, 64};
    int *location = find_if((int *) numbers, (int *) numbers + 6, odd);

    ASSERT_TRUE((location - numbers) == 3);

    int numbers_even[6] = {2, 4, 8, 16, 32, 64};

    int *out_range = find_if((int *) numbers_even, (int *) numbers_even + 6, odd);

    ASSERT_TRUE(out_range == (int *) (numbers_even + 6));
  }

  {
    struct Key {
      Key(int i) : data(i) {}
      int data;
      bool operator==(int d) const {
        return data == d;
      }
      bool operator!=(int d) const {
        return !operator==(d);
      }
    };
    Key keys[10] = {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}};
    Key const *k = find(keys + 0, keys + 10, 5);
    ASSERT_TRUE(k == keys + 10);
  }
}

TEST_F(FindTest, findif1) {
  typedef vector<int> IntVec;
  IntVec v(10);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = (i + 1) * (i + 1);
  IntVec::iterator iter;
  iter = find_if(v.begin(), v.end(), div_3);
  ASSERT_TRUE((iter - v.begin()) == 2);
}

TEST_F(FindTest, find_char) {
  char str[] = "abcdefghij";
  char *pstr = (char *) str;
  const char *cpstr = (const char *) str;
  size_t str_size = sizeof(str) / sizeof(char);

  char *d = find(pstr, pstr + str_size, 'd');
  ASSERT_TRUE(*d == 'd');

  const char *e = find(cpstr, cpstr + str_size, 'e');
  ASSERT_TRUE(*e == 'e');

  char *last = find(pstr, pstr + str_size, 'x');
  ASSERT_TRUE(last == pstr + str_size);

  const char *clast = find(cpstr, cpstr + str_size, 'x');
  ASSERT_TRUE(clast == cpstr + str_size);
}
