#include "AssociativeContainers/Multiset/stl_multiset.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class MultiSet : public testing::Test {
 protected:
  void SetUp() override {
  }
  typedef multiset<int, less<int>> mset;
  static bool less_than(int a_, int b_) {
    return a_ < b_;
  }

  static bool greater_than(int a_, int b_) {
    return a_ > b_;
  }
};

// test cases from https://sourceforge.net/projects/stlport/
TEST_F(MultiSet, mset1) {
  mset s;
  ASSERT_TRUE(s.count(42) == 0);
  s.insert(42);
  ASSERT_TRUE(s.count(42) == 1);
  s.insert(42);
  ASSERT_TRUE(s.count(42) == 2);

  mset::iterator i = s.find(40);
  ASSERT_TRUE(i == s.end());

  i = s.find(42);
  ASSERT_TRUE(i != s.end());
  size_t count = s.erase(42);
  ASSERT_TRUE(count == 2);
}
TEST_F(MultiSet, mset3) {
  int array[] = {3, 6, 1, 2, 3, 2, 6, 7, 9};

  //Check iterator on a mutable set
  mset s(array, array + 9);
  mset::iterator i;
  i = s.lower_bound(3);
  ASSERT_TRUE(*i == 3);
  i = s.upper_bound(3);
  ASSERT_TRUE(*i == 6);
  pair<mset::iterator, mset::iterator> p = s.equal_range(5);
  ASSERT_TRUE(*(p.first) == 6);
  ASSERT_TRUE(*(p.second) == 6);

  //Check const_iterator on a mutable multiset
  mset::const_iterator ci;
  ci = s.lower_bound(3);
  ASSERT_TRUE(*ci == 3);
  ci = s.upper_bound(3);
  ASSERT_TRUE(*ci == 6);
  pair<mset::const_iterator, mset::const_iterator> cp;
  cp = s.equal_range(5);
  ASSERT_TRUE(*(cp.first) == 6);
  ASSERT_TRUE(*(cp.second) == 6);
  //Check const_iterator on a const multiset
  mset const &crs = s;
  ci = crs.lower_bound(3);
  ASSERT_TRUE(*ci == 3);
  ci = crs.upper_bound(3);
  ASSERT_TRUE(*ci == 6);
  cp = crs.equal_range(5);
  ASSERT_TRUE(*(cp.first) == 6);
  ASSERT_TRUE(*(cp.second) == 6);
}
TEST_F(MultiSet, mset5) {
  int array[] = {3, 6, 1, 9};
  int j;

  typedef pointer_to_binary_function<int, int, bool> fn_type;
  typedef multiset<int, fn_type> fn_mset;

  fn_type f(less_than);
  fn_mset s1(array + 0, array + 4, f);
  fn_mset::const_iterator i = s1.begin();
  for (j = 0; i != s1.end(); ++i, ++j) {
    ASSERT_TRUE(j != 0 || *i == 1);
    ASSERT_TRUE(j != 1 || *i == 3);
    ASSERT_TRUE(j != 2 || *i == 6);
    ASSERT_TRUE(j != 3 || *i == 9);
  }

  fn_type g(greater_than);
  fn_mset s2(array, array + 4, g);
  i = s2.begin();
  for (j = 0; i != s2.end(); ++i, ++j) {
    ASSERT_TRUE(j != 0 || *i == 9);
    ASSERT_TRUE(j != 1 || *i == 6);
    ASSERT_TRUE(j != 2 || *i == 3);
    ASSERT_TRUE(j != 3 || *i == 1);
  }
}