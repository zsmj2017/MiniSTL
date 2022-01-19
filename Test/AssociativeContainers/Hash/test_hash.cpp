#include "AssociativeContainers/HashMultimap/hash_multimap.h"
#include "AssociativeContainers/HashMultiset/hash_multiset.h"
#include "AssociativeContainers/Hashmap/hash_map.h"
#include "AssociativeContainers/Hashset/hash_set.h"
#include <Function/stl_function.h>
#include <Hashtable/hash_func.h>
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class HashTest : public testing::Test {
 protected:
  void SetUp() override {
  }
  const int NB_ELEMS = 2000;
};

TEST_F(HashTest, uset) {
  typedef hash_set<int, hash<int>, equal_to<int>> usettype;
  usettype us;

  //Small compilation check of the copy constructor:
  usettype us2(us);
  //And assignment operator
  us = us2;

  int i;
  pair<usettype::iterator, bool> ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    ret = us.insert(i);
    ASSERT_TRUE(ret.second);
    ASSERT_TRUE(*ret.first == i);

    ret = us.insert(i);
    ASSERT_TRUE(!ret.second);
    ASSERT_TRUE(*ret.first == i);
  }
}

TEST_F(HashTest, umultiset) {
  typedef hash_multiset<int, hash<int>, equal_to<int>> usettype;
  usettype us;

  int i;
  usettype::iterator ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    ret = us.insert(i);
    ASSERT_TRUE(*ret == i);
    ret = us.insert(i);
    ASSERT_TRUE(*ret == i);
  }

  ASSERT_TRUE(us.size() == 2 * NB_ELEMS);
}

TEST_F(HashTest, umap) {
  typedef hash_map<int, int, hash<int>, equal_to<int>> umaptype;
  umaptype us;

  //Compilation check of the [] operator:
  umaptype us2;
  us[0] = us2[0];
  us.clear();

  {
    //An other compilation check
    typedef hash_map<int, umaptype> uumaptype;
    uumaptype uus;
    umaptype const &uref = uus[0];
    umaptype ucopy = uus[0];
    ucopy = uref;
  }

  int i;
  pair<umaptype::iterator, bool> ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    umaptype::value_type p1(i, i);
    ret = us.insert(p1);
    ASSERT_TRUE(ret.second);
    ASSERT_TRUE(*ret.first == p1);

    umaptype::value_type p2(i, i + 1);
    ret = us.insert(p2);
    ASSERT_TRUE(!ret.second);
    ASSERT_TRUE(*ret.first == p1);
  }

  {
    //Lets look for some values to see if everything is normal:
    umaptype::iterator umit;
    for (int j = 0; j < NB_ELEMS; j += NB_ELEMS / 100) {
      umit = us.find(j);

      ASSERT_TRUE(umit != us.end());
      ASSERT_TRUE((*umit).second == j);
    }
  }

  ASSERT_TRUE(us.size() == (size_t) NB_ELEMS);
}

TEST_F(HashTest, umultimap) {
  typedef hash_multimap<int, int, hash<int>, equal_to<int>> umaptype;
  umaptype us;
  int i;
  umaptype::iterator ret;
  for (i = 0; i < NB_ELEMS; ++i) {
    umaptype::value_type p(i, i);
    ret = us.insert(p);
    ASSERT_TRUE(*ret == p);

    ret = us.insert(p);
    ASSERT_TRUE(*ret == p);
  }
  ASSERT_TRUE(us.size() == 2 * NB_ELEMS);
}

TEST_F(HashTest, user_case) {
  typedef hash_map<int, string> UnorderedMap1;
  typedef hash_map<int, UnorderedMap1> UnorderedMap2;

  UnorderedMap1 foo;
  UnorderedMap2 bar;

  foo.insert(UnorderedMap1::value_type(1, string("test1")));
  foo.insert(UnorderedMap1::value_type(2, string("test2")));
  foo.insert(UnorderedMap1::value_type(3, string("test3")));
  foo.insert(UnorderedMap1::value_type(4, string("test4")));
  foo.insert(UnorderedMap1::value_type(5, string("test5")));

  bar.insert(UnorderedMap2::value_type(0, foo));
  UnorderedMap2::iterator it = bar.find(0);
  ASSERT_TRUE(it != bar.end());

  UnorderedMap1 &body = it->second;
  UnorderedMap1::iterator cur = body.find(3);
  ASSERT_TRUE(cur != body.end());

  body.erase(body.begin(), body.end());
  ASSERT_TRUE(body.empty());
}

TEST_F(HashTest, template_methods) {
  struct Key {
    Key() : m_data(0) {}
    Key(int data) : m_data(data) {}
    int m_data;
  };

  struct KeyHash {
    size_t operator()(Key key) const { return (size_t) key.m_data; }
    size_t operator()(int data) const { return (size_t) data; }
  };

  struct KeyEqual {
    bool operator()(Key lhs, Key rhs) const { return lhs.m_data == rhs.m_data; }
    bool operator()(Key lhs, int rhs) const { return lhs.m_data == rhs; }
    bool operator()(int lhs, Key rhs) const { return lhs == rhs.m_data; }
  };
  {
    typedef hash_set<Key, KeyHash, KeyEqual> Container;
    Container cont;
    cont.insert(Key(1));
    cont.insert(Key(2));
    cont.insert(Key(3));
    cont.insert(Key(4));

    ASSERT_TRUE(cont.count(Key(1)) == 1);
    ASSERT_TRUE(cont.count(1) == 1);
    ASSERT_TRUE(cont.count(5) == 0);

    ASSERT_TRUE(cont.find(2) != cont.end());
    ASSERT_TRUE(cont.equal_range(2) != make_pair(cont.begin(), cont.end()));

    Container const &ccont = cont;
    ASSERT_TRUE(ccont.find(2) != ccont.end());
    ASSERT_TRUE(ccont.equal_range(2) != make_pair(ccont.begin(), ccont.end()));
  }

  {
    typedef hash_multiset<Key, KeyHash, KeyEqual> Container;
    Container cont;
    cont.insert(Key(1));
    cont.insert(Key(2));
    cont.insert(Key(1));
    cont.insert(Key(2));

    ASSERT_TRUE(cont.count(Key(1)) == 2);
    ASSERT_TRUE(cont.count(1) == 2);
    ASSERT_TRUE(cont.count(5) == 0);

    ASSERT_TRUE(cont.find(2) != cont.end());
    ASSERT_TRUE(cont.equal_range(1) != make_pair(cont.end(), cont.end()));

    Container const &ccont = cont;
    ASSERT_TRUE(ccont.find(2) != ccont.end());
    ASSERT_TRUE(ccont.equal_range(2) != make_pair(ccont.end(), ccont.end()));
  }
}