#include "SequenceContainers/List/stl_list.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class ListTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

// test cases from https://sourceforge.net/projects/stlport/
TEST_F(ListTest, list1) {
  int array1[] = {9, 16, 36};
  int array2[] = {1, 4};

  list<int> l1(array1, array1 + 3);
  list<int> l2(array2, array2 + 2);
  list<int>::iterator i1 = l1.begin();
  list<int>::iterator i2 = l2.begin();
  list<int>::const_iterator ci(i1);
  list<int>::const_iterator ci1(ci);
  l1.splice(i1, l2);
  i1 = l1.begin();
  ASSERT_TRUE(*i1++ == 1);
  ASSERT_TRUE(*i1++ == 4);
  ASSERT_TRUE(*i1++ == 9);
  ASSERT_TRUE(*i1++ == 16);
  ASSERT_TRUE(*i1++ == 36);
  ASSERT_TRUE(i2 == l1.begin());

  //Default construct check
  list<int> l(2);
  i1 = l.begin();
  ASSERT_TRUE(*(i1++) == 0);
  ASSERT_TRUE(*i1 == 0);
}

TEST_F(ListTest, list2) {
  int array1[] = {1, 16};
  int array2[] = {4, 9};

  list<int> l1(array1, array1 + 2);
  list<int> l2(array2, array2 + 2);
  list<int>::iterator i = l1.begin();
  i++;
  l1.splice(i, l2, l2.begin(), l2.end());
  i = l1.begin();
  ASSERT_TRUE(*i++ == 1);
  ASSERT_TRUE(*i++ == 4);
  ASSERT_TRUE(*i++ == 9);
  ASSERT_TRUE(*i++ == 16);
}

TEST_F(ListTest, list3) {
  char array[] = {'x', 'l', 'x', 't', 's', 's'};

  list<char> str(array, array + 6);
  list<char>::iterator i;

  str.reverse();
  i = str.begin();
  ASSERT_TRUE(*i++ == 's');
  ASSERT_TRUE(*i++ == 's');
  ASSERT_TRUE(*i++ == 't');
  ASSERT_TRUE(*i++ == 'x');
  ASSERT_TRUE(*i++ == 'l');
  ASSERT_TRUE(*i++ == 'x');

  str.remove('x');
  i = str.begin();
  ASSERT_TRUE(*i++ == 's');
  ASSERT_TRUE(*i++ == 's');
  ASSERT_TRUE(*i++ == 't');
  ASSERT_TRUE(*i++ == 'l');

  str.unique();
  i = str.begin();
  ASSERT_TRUE(*i++ == 's');
  ASSERT_TRUE(*i++ == 't');
  ASSERT_TRUE(*i++ == 'l');

  str.sort();
  i = str.begin();
  ASSERT_TRUE(*i++ == 'l');
  ASSERT_TRUE(*i++ == 's');
  ASSERT_TRUE(*i++ == 't');
}

TEST_F(ListTest, list4) {
  int array1[] = {1, 3, 6, 7};
  int array2[] = {2, 4};

  list<int> l1(array1, array1 + 4);
  list<int> l2(array2, array2 + 2);
  l1.merge(l2);
  list<int>::iterator i = l1.begin();
  ASSERT_TRUE(*i++ == 1);
  ASSERT_TRUE(*i++ == 2);
  ASSERT_TRUE(*i++ == 3);
  ASSERT_TRUE(*i++ == 4);
  ASSERT_TRUE(*i++ == 6);
  ASSERT_TRUE(*i++ == 7);

  //We use distance to avoid a simple call to an internal counter
  ASSERT_TRUE(distance(l1.begin(), l1.end()) == 6);
  ASSERT_TRUE(distance(l2.begin(), l2.end()) == 0);

  l1.swap(l2);

  ASSERT_TRUE(distance(l1.begin(), l1.end()) == 0);
  ASSERT_TRUE(distance(l2.begin(), l2.end()) == 6);
}

TEST_F(ListTest, erase) {
  list<int> l;
  l.push_back(1);
  l.erase(l.begin());
  ASSERT_TRUE(l.empty());

  int array[] = {0, 1, 2, 3};
  l.assign(array, array + 4);
  list<int>::iterator lit;
  lit = l.erase(l.begin());
  ASSERT_TRUE(*lit == 1);

  lit = l.erase(l.begin(), --l.end());
  ASSERT_TRUE(*lit == 3);

  l.clear();
  ASSERT_TRUE(l.empty());
}

TEST_F(ListTest, resize) {
  {
    list<int> l;
    l.resize(5, 1);

    size_t i;
    list<int>::iterator lit(l.begin());
    for (i = 0; i < 5; ++i) {
      ASSERT_TRUE(lit != l.end());
      ASSERT_TRUE(*(lit++) == 1);
    }
    ASSERT_TRUE(lit == l.end());

    l.resize(3);
    lit = l.begin();
    for (i = 0; i < 3; ++i) {
      ASSERT_TRUE(lit != l.end());
      ASSERT_TRUE(*(lit++) == 1);
    }
    ASSERT_TRUE(lit == l.end());
  }

  {
    list<int> l;
    l.resize(5);

    size_t i;
    list<int>::iterator lit(l.begin());
    for (i = 0; i < 5; ++i) {
      ASSERT_TRUE(lit != l.end());
      ASSERT_TRUE(*(lit++) == 0);
    }
    ASSERT_TRUE(lit == l.end());
  }
}

TEST_F(ListTest, push_back) {
  list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  list<int>::reverse_iterator r = l.rbegin();

  ASSERT_TRUE(*r == 3);
}

TEST_F(ListTest, push_front) {
  list<int> l;
  l.push_back(1);
  l.push_back(2);
  l.push_back(3);

  list<int>::iterator i = l.begin();

  ASSERT_TRUE(*i == 1);
  l.push_front(0);
  ASSERT_TRUE(*i == 1);
  ++i;
  ASSERT_TRUE(*i == 2);
}

TEST_F(ListTest, swap) {
  list<int> lst1;
  list<int> lst2;

  lst1.push_back(1);
  lst2.push_back(2);

  lst1.swap(lst2);

  ASSERT_TRUE(lst1.front() == 2);
  ASSERT_TRUE(lst2.front() == 1);
  ASSERT_TRUE(lst1.size() == 1);
  ASSERT_TRUE(lst2.size() == 1);

  lst1.pop_front();
  lst2.pop_front();

  ASSERT_TRUE(lst1.empty());
  ASSERT_TRUE(lst2.empty());
}

namespace foo {
class bar {};

template<class It>
size_t distance(It, It);
}// namespace foo

TEST_F(ListTest, adl) {
  list<foo::bar> lbar;
  ASSERT_TRUE(lbar.empty());
}