#include "Algorithms/algo/stl_algo.h"
#include "AssociativeContainers/Map/stl_map.h"
#include "SequenceContainers/Deque/stl_deque.h"
#include "SequenceContainers/List/stl_list.h"
#include "SequenceContainers/Slist/slist.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class AlgoTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(AlgoTest, min_max) {
  int i = min(4, 7);
  ASSERT_TRUE(i == 4);
  char c = max('a', 'z');
  ASSERT_TRUE(c == 'z');

  c = min('a', 'z', greater<char>());
  ASSERT_TRUE(c == 'z');
  i = max(4, 7, greater<int>());
  ASSERT_TRUE(i == 4);
}

TEST_F(AlgoTest, count) {
  {
    int i[] = {1, 4, 2, 8, 2, 2};
    int n = count(i, i + 6, 2);
    ASSERT_TRUE(n == 3);
  }
  {
    vector<int> i;
    i.push_back(1);
    i.push_back(4);
    i.push_back(2);
    i.push_back(8);
    i.push_back(2);
    i.push_back(2);
    int n = count(i.begin(), i.end(), 2);
    ASSERT_TRUE(n == 3);
  }
}

TEST_F(AlgoTest, sort) {
  {
    vector<int> years;
    years.push_back(1962);
    years.push_back(1992);
    years.push_back(2001);
    years.push_back(1999);
    sort(years.begin(), years.end());
    ASSERT_TRUE(years[0] == 1962);
    ASSERT_TRUE(years[1] == 1992);
    ASSERT_TRUE(years[2] == 1999);
    ASSERT_TRUE(years[3] == 2001);
  }
  {
    deque<int> years;
    years.push_back(1962);
    years.push_back(1992);
    years.push_back(2001);
    years.push_back(1999);
    sort(years.begin(), years.end());// <-- changed!
    ASSERT_TRUE(years[0] == 1962);
    ASSERT_TRUE(years[1] == 1992);
    ASSERT_TRUE(years[2] == 1999);
    ASSERT_TRUE(years[3] == 2001);
  }
}

#define ARRAY_SIZE(arr) sizeof(arr) / sizeof(arr[0])

TEST_F(AlgoTest, search_n) {
  int ints[] = {0, 1, 2, 3, 3, 4, 4, 4, 2, 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5};

  //search_n
  //Forward iterator
  {
    slist<int> slint(ints, ints + ARRAY_SIZE(ints));
    slist<int>::iterator slit = search_n(slint.begin(), slint.end(), 2, 2);
    ASSERT_TRUE(slit != slint.end());
    ASSERT_TRUE(*(slit++) == 2);
    ASSERT_TRUE(*slit == 2);
  }

  //Bidirectionnal iterator
  {
    list<int> lint(ints, ints + ARRAY_SIZE(ints));
    list<int>::iterator lit = search_n(lint.begin(), lint.end(), 3, 3);
    ASSERT_TRUE(lit != lint.end());
    ASSERT_TRUE(*(lit++) == 3);
    ASSERT_TRUE(*(lit++) == 3);
    ASSERT_TRUE(*lit == 3);
  }

  //Random access iterator
  {
    deque<int> dint(ints, ints + ARRAY_SIZE(ints));
    deque<int>::iterator dit = search_n(dint.begin(), dint.end(), 4, 4);
    ASSERT_TRUE(dit != dint.end());
    ASSERT_TRUE(*(dit++) == 4);
    ASSERT_TRUE(*(dit++) == 4);
    ASSERT_TRUE(*(dit++) == 4);
    ASSERT_TRUE(*dit == 4);
  }

  //search_n with predicate
  //Forward iterator
  {
    slist<int> slint(ints, ints + ARRAY_SIZE(ints));
    slist<int>::iterator slit = search_n(slint.begin(), slint.end(), 2, 1, greater<int>());
    ASSERT_TRUE(slit != slint.end());
    ASSERT_TRUE(*(slit++) > 1);
    ASSERT_TRUE(*slit > 2);
  }

  //Bidirectionnal iterator
  {
    list<int> lint(ints, ints + ARRAY_SIZE(ints));
    list<int>::iterator lit = search_n(lint.begin(), lint.end(), 3, 2, greater<int>());
    ASSERT_TRUE(lit != lint.end());
    ASSERT_TRUE(*(lit++) > 2);
    ASSERT_TRUE(*(lit++) > 2);
    ASSERT_TRUE(*lit > 2);
  }

  //Random access iterator
  {
    deque<int> dint(ints, ints + ARRAY_SIZE(ints));
    deque<int>::iterator dit = search_n(dint.begin(), dint.end(), 4, 3, greater<int>());
    ASSERT_TRUE(dit != dint.end());
    ASSERT_TRUE(*(dit++) > 3);
    ASSERT_TRUE(*(dit++) > 3);
    ASSERT_TRUE(*(dit++) > 3);
    ASSERT_TRUE(*dit > 3);
  }

  {
    int array[] = {0, 0, 1, 0, 1, 1};
    int *array_end = array + sizeof(array) / sizeof(*array);
    ASSERT_TRUE(search_n(array, array_end, 3, 1) == array_end);
  }

  {
    int array[] = {0, 1, 2, 3, 4, 5};
    int *array_end = array + sizeof(array) / sizeof(*array);
    ASSERT_TRUE(search_n(array, array_end, 1, 1, equal_to<int>()) == &array[1]);
  }
}

TEST_F(AlgoTest, find_first_of) {
  struct MyIntComparable {
    MyIntComparable(int val) : _val(val) {}
    bool operator==(const MyIntComparable &other) const { return _val == other._val; }

   private:
    int _val;
  };

  slist<int> intsl;
  intsl.push_front(1);
  intsl.push_front(2);

  {
    vector<int> intv;
    intv.push_back(0);
    intv.push_back(1);
    intv.push_back(2);
    intv.push_back(3);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), intsl.begin(), intsl.end());
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 1);
  }
  {
    vector<int> intv;
    intv.push_back(3);
    intv.push_back(2);
    intv.push_back(1);
    intv.push_back(0);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), intsl.begin(), intsl.end());
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 2);
  }

  list<int> intl;
  intl.push_front(1);
  intl.push_front(2);

  {
    vector<int> intv;
    intv.push_back(0);
    intv.push_back(1);
    intv.push_back(2);
    intv.push_back(3);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), intl.begin(), intl.end());
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 1);
  }
  {
    vector<int> intv;
    intv.push_back(3);
    intv.push_back(2);
    intv.push_back(1);
    intv.push_back(0);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), intl.begin(), intl.end());
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 2);
  }
  {
    char chars[] = {1, 2};

    vector<int> intv;
    intv.push_back(0);
    intv.push_back(1);
    intv.push_back(2);
    intv.push_back(3);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), chars, chars + sizeof(chars));
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 1);
  }
  {
    unsigned char chars[] = {1, 2, 255};

    vector<int> intv;
    intv.push_back(-10);
    intv.push_back(1029);
    intv.push_back(255);
    intv.push_back(4);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), chars, chars + sizeof(chars));
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 255);
  }
  {
    signed char chars[] = {93, 2, -101, 13};

    vector<int> intv;
    intv.push_back(-10);
    intv.push_back(1029);
    intv.push_back(-2035);
    intv.push_back(-101);
    intv.push_back(4);

    vector<int>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), chars, chars + sizeof(chars));
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == -101);
  }
  {
    char chars[] = {1, 2};

    vector<MyIntComparable> intv;
    intv.push_back(0);
    intv.push_back(1);
    intv.push_back(2);
    intv.push_back(3);

    vector<MyIntComparable>::iterator first;
    first = find_first_of(intv.begin(), intv.end(), chars, chars + sizeof(chars));
    ASSERT_TRUE(first != intv.end());
    ASSERT_TRUE(*first == 1);
  }
}

