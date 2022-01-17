#include "AssociativeContainers/Set/stl_set.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class SetTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

// test cases from https://sourceforge.net/projects/stlport/
TEST_F(SetTest, set1) {
  set<int, less<int>> s;
  ASSERT_TRUE(s.count(42) == 0);
  s.insert(42);
  ASSERT_TRUE(s.count(42) == 1);
  s.insert(42);
  ASSERT_TRUE(s.count(42) == 1);
  size_t count = s.erase(42);
  ASSERT_TRUE(count == 1);
}

TEST_F(SetTest, set2) {
  typedef set<int, less<int>> int_set;
  int_set s;
  pair<int_set::iterator, bool> p = s.insert(42);
  ASSERT_TRUE(p.second == true);
  p = s.insert(42);
  ASSERT_TRUE(p.second == false);

  int array1[] = {1, 3, 6, 7};
  s.insert(array1, array1 + 4);
  ASSERT_TRUE(distance(s.begin(), s.end()) == 5);

  int_set s2;
  s2.swap(s);
  ASSERT_TRUE(distance(s2.begin(), s2.end()) == 5);
  ASSERT_TRUE(distance(s.begin(), s.end()) == 0);

  int_set s3;
  s3.swap(s);
  s3.swap(s2);
  ASSERT_TRUE(distance(s.begin(), s.end()) == 0);
  ASSERT_TRUE(distance(s2.begin(), s2.end()) == 0);
  ASSERT_TRUE(distance(s3.begin(), s3.end()) == 5);
}

TEST_F(SetTest, erase) {
  set<int, less<int>> s;
  s.insert(1);
  s.erase(s.begin());
  ASSERT_TRUE(s.empty());

  size_t nb = s.erase(1);
  ASSERT_TRUE(nb == 0);
}

TEST_F(SetTest, insert) {
  set<int> s;
  set<int>::iterator i = s.insert(s.end(), 0);
  ASSERT_TRUE(*i == 0);
}

TEST_F(SetTest, find) {
  set<int> s;
  ASSERT_TRUE(s.find(0) == s.end());
  set<int> const &crs = s;
  ASSERT_TRUE(crs.find(0) == crs.end());
}

TEST_F(SetTest, bounds) {
  int array1[] = {1, 3, 6, 7};
  set<int> s(array1, array1 + sizeof(array1) / sizeof(array1[0]));
  set<int> const &crs = s;

  set<int>::iterator sit;
  set<int>::const_iterator scit;
  pair<set<int>::iterator, set<int>::iterator> pit;
  pair<set<int>::const_iterator, set<int>::const_iterator> pcit;

  //Check iterator on mutable set
  sit = s.lower_bound(2);
  ASSERT_TRUE(sit != s.end());
  ASSERT_TRUE(*sit == 3);

  sit = s.upper_bound(5);
  ASSERT_TRUE(sit != s.end());
  ASSERT_TRUE(*sit == 6);

  pit = s.equal_range(6);
  ASSERT_TRUE(pit.first != pit.second);
  ASSERT_TRUE(pit.first != s.end());
  ASSERT_TRUE(*pit.first == 6);
  ASSERT_TRUE(pit.second != s.end());
  ASSERT_TRUE(*pit.second == 7);

  pit = s.equal_range(4);
  ASSERT_TRUE(pit.first == pit.second);
  ASSERT_TRUE(pit.first != s.end());
  ASSERT_TRUE(*pit.first == 6);
  ASSERT_TRUE(pit.second != s.end());
  ASSERT_TRUE(*pit.second == 6);

  //Check const_iterator on mutable set
  scit = s.lower_bound(2);
  ASSERT_TRUE(scit != s.end());
  ASSERT_TRUE(*scit == 3);

  scit = s.upper_bound(5);
  ASSERT_TRUE(scit != s.end());
  ASSERT_TRUE(*scit == 6);

  pcit = s.equal_range(6);
  ASSERT_TRUE(pcit.first != pcit.second);
  ASSERT_TRUE(pcit.first != s.end());
  ASSERT_TRUE(*pcit.first == 6);
  ASSERT_TRUE(pcit.second != s.end());
  ASSERT_TRUE(*pcit.second == 7);

  //Check const_iterator on const set
  scit = crs.lower_bound(2);
  ASSERT_TRUE(scit != crs.end());
  ASSERT_TRUE(*scit == 3);

  scit = crs.upper_bound(5);
  ASSERT_TRUE(scit != crs.end());
  ASSERT_TRUE(*scit == 6);

  pcit = crs.equal_range(6);
  ASSERT_TRUE(pcit.first != pcit.second);
  ASSERT_TRUE(pcit.first != crs.end());
  ASSERT_TRUE(*pcit.first == 6);
  ASSERT_TRUE(pcit.second != crs.end());
  ASSERT_TRUE(*pcit.second == 7);
}

class SetTestClass {
 public:
  explicit SetTestClass(int data) : _data(data) {}
  int data() const {
    return _data;
  }

 private:
  int _data;
};

template<>
struct less<SetTestClass> {
  bool operator()(SetTestClass const &lhs, SetTestClass const &rhs) const {
    return lhs.data() < rhs.data();
  }
};

TEST_F(SetTest, specialized_less) {
  set<SetTestClass> s;
  s.insert(SetTestClass(1));
  s.insert(SetTestClass(3));
  s.insert(SetTestClass(2));
  s.insert(SetTestClass(0));

  set<SetTestClass>::iterator sit(s.begin()), sitEnd(s.end());
  int i = 0;
  for (; sit != sitEnd; ++sit, ++i) {
    ASSERT_TRUE(sit->data() == i);
  }
}

TEST_F(SetTest, implementation_check) {
  set<int> tree;
  tree.insert(1);
  set<int>::iterator it = tree.begin();
  int const &int_ref = *it++;
  ASSERT_TRUE(int_ref == 1);

  ASSERT_TRUE(it == tree.end());
  ASSERT_TRUE(it != tree.begin());

  set<int>::const_iterator cit = tree.begin();
  int const &int_cref = *cit++;
  ASSERT_TRUE(int_cref == 1);
}

TEST_F(SetTest, reverse_iterator_test) {
  set<int> tree;
  tree.insert(1);
  tree.insert(2);

  {
    set<int>::reverse_iterator rit(tree.rbegin());
    ASSERT_TRUE(*(rit++) == 2);
    ASSERT_TRUE(*(rit++) == 1);
    ASSERT_TRUE(rit == tree.rend());
  }

  {
    set<int> const &ctree = tree;
    set<int>::const_reverse_iterator rit(ctree.rbegin());
    ASSERT_TRUE(*(rit++) == 2);
    ASSERT_TRUE(*(rit++) == 1);
    ASSERT_TRUE(rit == ctree.rend());
  }
}

TEST_F(SetTest, template_methods) {
  struct Key {
    Key() : m_data(0) {}
    Key(int data) : m_data(data) {}
    int m_data;
  };

  struct KeyCmp {
    bool operator()(Key lhs, Key rhs) const { return lhs.m_data < rhs.m_data; }

    bool operator()(Key lhs, int rhs) const { return lhs.m_data < rhs; }

    bool operator()(int lhs, Key rhs) const { return lhs < rhs.m_data; }
  };
  typedef set<Key, KeyCmp> KeySet;
  KeySet keySet;
  keySet.insert(Key(1));
  keySet.insert(Key(2));
  keySet.insert(Key(3));
  keySet.insert(Key(4));

  ASSERT_TRUE(keySet.count(Key(1)) == 1);
  ASSERT_TRUE(keySet.count(1) == 1);
  ASSERT_TRUE(keySet.count(5) == 0);

  ASSERT_TRUE(keySet.find(2) != keySet.end());
  ASSERT_TRUE(keySet.lower_bound(2) != keySet.end());
  ASSERT_TRUE(keySet.upper_bound(2) != keySet.end());
  ASSERT_TRUE(keySet.equal_range(2) != make_pair(keySet.begin(), keySet.end()));

  KeySet const &ckeySet = keySet;
  ASSERT_TRUE(ckeySet.find(2) != ckeySet.end());
  ASSERT_TRUE(ckeySet.lower_bound(2) != ckeySet.end());
  ASSERT_TRUE(ckeySet.upper_bound(2) != ckeySet.end());
  ASSERT_TRUE(ckeySet.equal_range(2) != make_pair(ckeySet.begin(), ckeySet.end()));
}
