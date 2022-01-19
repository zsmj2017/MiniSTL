#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class EqualTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  static bool values_squared(int a, int b) {
    return (a * a == b);
  }
};

// TODO::need equal_range()
#if 0
TEST_F(EqualTest, equal_range0) {
  int numbers[10] = {0, 0, 1, 1, 2, 2, 2, 2, 3, 3};
  pair<int *, int *> range = equal_range((int *) numbers, (int *) numbers + 10, 2);
  ASSERT_TRUE((range.first - numbers) == 4);
  ASSERT_TRUE((range.second - numbers) == 8);
}

TEST_F(EqualTest, equal_range1) {
  typedef vector<int> IntVec;
  IntVec v(10);
  for (int i = 0; (size_t) i < v.size(); ++i)
    v[i] = i / 3;

  pair<IntVec::iterator, IntVec::iterator> range = equal_range(v.begin(), v.end(), 2);
  ASSERT_TRUE((range.first - v.begin()) == 6);
  ASSERT_TRUE((range.second - v.begin()) == 9);
  for (; range.first != range.second; ++range.first)
    ASSERT_TRUE(*range.first == 2);

  range = equal_range(v.begin(), v.end(), 4);
  ASSERT_TRUE(range.first == range.second);
  ASSERT_TRUE(range.first == v.end());
}

struct Test {
  Test();
  Test(int val) : value(val) {}
  int value;
  bool operator==(int i) const { return value == i; }
};

bool operator<(const Test &v1, int v2) { return v1.value < v2; }
bool operator<(int v1, const Test &v2) { return v1 < v2.value; }
bool operator<(const Test &v1, const Test &v2) { return v1.value < v2.value; }

TEST_F(EqualTest, equal_range2) {
  char chars[] = "aabbccddggghhklllmqqqqssyyzz";

  const unsigned count = sizeof(chars) - 1;
  pair<char *, char *> range = equal_range((char *) chars, (char *) chars + count, 'q', less<char>());
  ASSERT_TRUE((range.first - chars) == 18);
  ASSERT_TRUE((range.second - chars) == 22);
  for (; range.first != range.second; ++range.first)
    ASSERT_TRUE(*range.first == 'q');

  range = equal_range((char *) chars, (char *) chars + count, 'm', less<char>());
  ASSERT_TRUE((range.second - range.first) == 1);
  ASSERT_TRUE(*range.first == 'm');

  vector<Test> tv;
  vector<Test>::iterator it;
  pair<vector<Test>::iterator, vector<Test>::iterator> p;

  for (int i = 0; i < 10; ++i) {
    tv.push_back(i);
  }

  it = upper_bound(tv.begin(), tv.end(), 5);
  ASSERT_TRUE(it != tv.end());
  ASSERT_TRUE(*it == 6);

  it = lower_bound(tv.begin(), tv.end(), 5);
  ASSERT_TRUE(it != tv.end());
  ASSERT_TRUE(*it == 5);

  p = equal_range(tv.begin(), tv.end(), 5);
  ASSERT_TRUE(p.first != p.second);
  ASSERT_TRUE(p.first != tv.end());
  ASSERT_TRUE(p.second != tv.end());
  ASSERT_TRUE(*p.first == 5);
  ASSERT_TRUE(*p.second == 6);
}
#endif

TEST_F(EqualTest, equal0) {
  int numbers1[5] = {1, 2, 3, 4, 5};
  int numbers2[5] = {1, 2, 4, 8, 16};
  int numbers3[2] = {1, 2};

  ASSERT_TRUE(!equal(numbers1, numbers1 + 5, numbers2));
  ASSERT_TRUE(equal(numbers3, numbers3 + 2, numbers1));
}

TEST_F(EqualTest, equal1) {
  vector<int> v1(10);
  for (int i = 0; (size_t) i < v1.size(); ++i)
    v1[i] = i;
  vector<int> v2(10);
  ASSERT_TRUE(!equal(v1.begin(), v1.end(), v2.begin()));

  copy(v1.begin(), v1.end(), v2.begin());
  ASSERT_TRUE(equal(v1.begin(), v1.end(), v2.begin()));
}

TEST_F(EqualTest, equal2) {
  vector<int> v1(10);
  vector<int> v2(10);
  for (int i = 0; (size_t) i < v1.size(); ++i) {
    v1[i] = i;
    v2[i] = i * i;
  }
  ASSERT_TRUE(equal(v1.begin(), v1.end(), v2.begin(), values_squared));
}

TEST_F(EqualTest, equal_to) {
  int input1[4] = {1, 7, 2, 2};
  int input2[4] = {1, 6, 2, 3};

  int output[4];
  transform((int *) input1, (int *) input1 + 4, (int *) input2, (int *) output, equal_to<int>());
  ASSERT_TRUE(output[0] == 1);
  ASSERT_TRUE(output[1] == 0);
  ASSERT_TRUE(output[2] == 1);
  ASSERT_TRUE(output[3] == 0);
}
