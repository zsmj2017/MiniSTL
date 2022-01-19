#include "Iterator/stl_iterator.h"
#include "SequenceContainers/List/stl_list.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class IterTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};
TEST_F(IterTest, iter1) {
  vector<const char *> v; // Vector of character strings.
  v.push_back("zippy");   // First element.
  v.push_back("motorboy");// Second element.
  typedef vector<const char *> vec;
  unsigned counter = 0;
  for (vec::iterator i = v.begin(); i != v.end(); ++i, ++counter) {
    switch (counter) {
      case 0:
        ASSERT_TRUE(!strcmp(*i, "zippy"));
        break;
      case 1:
        ASSERT_TRUE(!strcmp(*i, "motorboy"));
        break;
      default:
        ASSERT_TRUE(false);
    }
  }
}
TEST_F(IterTest, iter2) {
  typedef vector<const char *> Vec;
  Vec v;                  // Vector of character strings.
  v.push_back("zippy");   // First element.
  v.push_back("motorboy");// Second element.
  Vec::reverse_iterator it;
  unsigned counter = 0;
  for (it = v.rbegin(); it != v.rend(); ++it, ++counter) {
    switch (counter) {
      case 1:
        ASSERT_TRUE(!strcmp(*it, "zippy"));
        break;
      case 0:
        ASSERT_TRUE(!strcmp(*it, "motorboy"));
        break;
      default:
        ASSERT_TRUE(false);
    }
  }
}
TEST_F(IterTest, iter3) {
  vector<int> v;// Empty vector of integers.
  v.push_back(1);
  v.push_back(2);
  v.push_back(3);
  // Position immediately after last item.
  vector<int>::iterator i = v.end();
  // Move back one and then access.
  ASSERT_TRUE((*--i) == 3);
  i -= 2;// Jump back two items.
  ASSERT_TRUE((*i) == 1);
}
TEST_F(IterTest, iterswap0) {
  int numbers[6] = {0, 1, 2, 3, 4, 5};

  iter_swap(numbers, numbers + 3);

  ASSERT_TRUE(numbers[0] == 3);
  ASSERT_TRUE(numbers[1] == 1);
  ASSERT_TRUE(numbers[2] == 2);
  ASSERT_TRUE(numbers[3] == 0);
  ASSERT_TRUE(numbers[4] == 4);
  ASSERT_TRUE(numbers[5] == 5);
}
TEST_F(IterTest, iterswap1) {
  vector<int> v1(6);
  for (int i = 0; i < (int)v1.size(); ++i)
    v1[i] = i;
  iter_swap(v1.begin(), v1.begin() + 3);

  ASSERT_TRUE(v1[0] == 3);
  ASSERT_TRUE(v1[1] == 1);
  ASSERT_TRUE(v1[2] == 2);
  ASSERT_TRUE(v1[3] == 0);
  ASSERT_TRUE(v1[4] == 4);
  ASSERT_TRUE(v1[5] == 5);
}
TEST_F(IterTest, iterswap2) {
  vector<bool> boolVector;
  boolVector.push_back(true);
  boolVector.push_back(false);
  vector<bool>::iterator i1 = boolVector.begin();
  vector<bool>::iterator i2 = boolVector.begin();
  ++i2;

  bool v0 = *i1;
  bool v1 = *i2;

  iter_swap(i1, i2);

  ASSERT_TRUE((*i1 == v1 && *i2 == v0));
}

TEST_F(IterTest, iterswap3) {
  vector<int> vvref(10, 10);
  vector<int> lvref(10, 20);

  vector<vector<int>> vvints(4, vvref);
  list<vector<int>> lvints(4, lvref);

  iter_swap(vvints.begin(), lvints.begin());
  ASSERT_TRUE(vvints.front() == lvref);
  ASSERT_TRUE(lvints.front() == vvref);
}