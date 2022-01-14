#include "SequenceContainers/List/stl_list.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class ListTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // set pre data
    li = {1, 2, 3, 4, 5};
    lf = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    ld = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    lb.resize(5);
    ls = {"hello", "hello"};
    lli = {{1, 2, 3}, {1, 2, 3}};
  }
  // pre structure
  struct BAR {
    int a;
    double b;
  };
  list<int> li;
  list<float> lf;
  list<double> ld;
  list<BAR> lb;
  list<string> ls;
  list<list<int>> lli;
};

TEST_F(ListTest, DEFAULT_CTOR) {
  EXPECT_NO_THROW(list<int>());
  EXPECT_NO_THROW(list<float>());
  EXPECT_NO_THROW(list<double>());
  EXPECT_NO_THROW(list<string>());
  EXPECT_NO_THROW(list<BAR>());
  EXPECT_NO_THROW(list<list<int>>());
}

TEST_F(ListTest, CTOR_WITH_SIZE) {
  EXPECT_NO_THROW(list<int>(5));
  EXPECT_NO_THROW(list<float>(5));
  EXPECT_NO_THROW(list<double>(5));
  EXPECT_NO_THROW(list<string>(5));
  EXPECT_NO_THROW(list<BAR>(5));
  EXPECT_NO_THROW(list<list<int>>(5));
}

TEST_F(ListTest, CTOR_WITH_VALUE) {
  EXPECT_NO_THROW(list<int>(5, 5));
  EXPECT_NO_THROW(list<float>(5, 5.0f));
  EXPECT_NO_THROW(list<double>(5, 5.0f));
  EXPECT_NO_THROW(list<string>(5, "hello"));
  EXPECT_NO_THROW(list<BAR>(5, BAR()));
  EXPECT_NO_THROW(list<list<int>>(5, {1, 2, 3}));
}

TEST_F(ListTest, CTOR_WITH_INPUTITERATOR) {
  EXPECT_NO_THROW(list<int>(li.begin(), li.end()));
  EXPECT_NO_THROW(list<int>(li.cbegin(), li.cend()));
  EXPECT_NO_THROW(list<float>(lf.begin(), lf.end()));
  EXPECT_NO_THROW(list<float>(lf.cbegin(), lf.cend()));
  EXPECT_NO_THROW(list<double>(ld.begin(), ld.end()));
  EXPECT_NO_THROW(list<double>(ld.cbegin(), ld.cend()));
  EXPECT_NO_THROW(list<BAR>(lb.begin(), lb.end()));
  EXPECT_NO_THROW(list<BAR>(lb.cbegin(), lb.cend()));
  EXPECT_NO_THROW(list<list<int>>(lli.begin(), lli.end()));
  EXPECT_NO_THROW(list<list<int>>(lli.cbegin(), lli.cend()));
  // C API
  int ai[5] = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW(list<int>(ai, ai + 5));
  double ad[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  EXPECT_NO_THROW(list<double>(ad, ad + 5));
  string as[5] = {"a", "b", "c", "d", "f"};
  EXPECT_NO_THROW(list<string>(as, as + 5));
}

TEST_F(ListTest, CTOR_WITH_INITIAL_LIST) {
  EXPECT_NO_THROW(list<int>({1, 2, 3, 4, 5}));
  EXPECT_NO_THROW(list<float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(list<double>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(list<string>({"hello", "world"}));
  EXPECT_NO_THROW(list<list<int>>({{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(ListTest, COPY_CTOR) {
  EXPECT_NO_THROW(list<int>(li));
  EXPECT_NO_THROW(list<float>(lf));
  EXPECT_NO_THROW(list<double>(ld));
  EXPECT_NO_THROW(list<string>(ls));
  EXPECT_NO_THROW(list<BAR>(lb));
  EXPECT_NO_THROW(list<list<int>>(lli));
}

TEST_F(ListTest, MOVE_CTOR) {
  EXPECT_NO_THROW(list<int>(std::move(li)));
  EXPECT_NO_THROW(list<float>(std::move(lf)));
  EXPECT_NO_THROW(list<double>(std::move(ld)));
  EXPECT_NO_THROW(list<string>(std::move(ls)));
  EXPECT_NO_THROW(list<BAR>(std::move(lb)));
  EXPECT_NO_THROW(list<list<int>>(std::move(lli)));
}

// Node-based container cannot pass this test, as does std::list.

/* TEST_F(ListTest,DTOR){
    list<int> *p = new list<int>(5,1);
    int* p2i = &(p->begin().node->data);
    EXPECT_EQ(*p2i,1);
    delete p;
    EXPECT_NE(*p2i,1);
} */

TEST_F(ListTest, COPYASSIGN_WITH_SELF) {
  list<int> temp_li(5, 2);
  li = temp_li;
  for (auto i : li) EXPECT_EQ(i, 2);
  list<float> temp_lf(5, 2.0f);
  lf = temp_lf;
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  list<double> temp_ld(5, 2.235f);
  ld = temp_ld;
  for (auto d : ld) EXPECT_EQ(d, 2.235f);
  list<string> temp_ls(5, "hello");
  ls = temp_ls;
  for (auto s : ls) EXPECT_EQ(s, "hello");
  list<list<int>> temp_lli(5, {1, 2, 3});
  lli = temp_lli;
  for (auto li : lli) EXPECT_EQ(li, list<int>({1, 2, 3}));
}

TEST_F(ListTest, COPYASSIGN_WITH_INITIAL_LIST) {
  li = {2, 2, 2, 2, 2};
  for (auto i : li) EXPECT_EQ(i, 2);
  lf = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  ld = {2.235f};
  for (auto d : ld) EXPECT_EQ(d, 2.235f);
  ls = {"hello", "hello", "hello"};
  for (auto s : ls) EXPECT_EQ(s, "hello");
  lli = {list<int>({1, 2, 3}), list<int>({1, 2, 3})};
  for (auto li : lli) EXPECT_EQ(li, list<int>({1, 2, 3}));
}

TEST_F(ListTest, MOVEASSIGN_WITH_SELF) {
  list<int> temp_li(5, 2);
  li = std::move(temp_li);
  for (auto i : li) EXPECT_EQ(i, 2);
  for (auto i : temp_li) EXPECT_NE(i, 2);
  list<float> temp_lf(5, 2.0f);
  lf = std::move(temp_lf);
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  for (auto f : temp_lf) EXPECT_NE(f, 2.0f);
  list<double> temp_ld(5, 2.235f);
  ld = std::move(temp_ld);
  for (auto d : ld) EXPECT_EQ(d, 2.235f);
  for (auto d : temp_ld) EXPECT_NE(d, 2.235f);
  list<string> temp_ls(5, "hello");
  ls = std::move(temp_ls);
  for (auto s : ls) EXPECT_EQ(s, "hello");
  for (auto s : temp_ls) EXPECT_NE(s, "hello");
  list<list<int>> temp_lli(5, {1, 2, 3});
  lli = std::move(temp_lli);
  for (auto li : lli) EXPECT_EQ(li, list<int>({1, 2, 3}));
  for (auto li : temp_lli) EXPECT_NE(li, list<int>({1, 2, 3}));
}

TEST_F(ListTest, BEGIN_AND_END) {
  for (auto it = li.begin(); it != li.end(); ++it) {
    *it = 5;
  }
  for (auto it = li.cbegin(); it != li.cend(); ++it) EXPECT_EQ(*it, 5);
  for (auto it = li.rbegin(); it != li.rend(); ++it) {
    EXPECT_EQ(*it, 5);
    *it = 3;
  }
  for (auto it = li.crbegin(); it != li.crend(); ++it) {
    EXPECT_EQ(*it, 3);
  }
  for (auto it = lf.begin(); it != lf.end(); ++it) {
    *it = 5.0f;
  }
  for (auto it = lf.cbegin(); it != lf.cend(); ++it) EXPECT_EQ(*it, 5.0f);
  for (auto it = lf.rbegin(); it != lf.rend(); ++it) {
    EXPECT_EQ(*it, 5.0f);
    *it = 3.0f;
  }
  for (auto it = lf.crbegin(); it != lf.crend(); ++it) {
    EXPECT_EQ(*it, 3.0f);
  }
  for (auto it = ld.begin(); it != ld.end(); ++it) {
    *it = 5.0f;
  }
  for (auto it = ld.cbegin(); it != ld.cend(); ++it) EXPECT_EQ(*it, 5.0f);
  for (auto it = ld.rbegin(); it != ld.rend(); ++it) {
    EXPECT_EQ(*it, 5.0f);
    *it = 3.0f;
  }
  for (auto it = ld.crbegin(); it != ld.crend(); ++it) {
    EXPECT_EQ(*it, 3.0f);
  }
  for (auto it = ls.begin(); it != ls.end(); ++it) {
    EXPECT_EQ(*it, "hello");
    *it = "world";
  }
  for (auto it = ls.cbegin(); it != ls.cend(); ++it) EXPECT_EQ(*it, "world");
  for (auto it = ls.rbegin(); it != ls.rend(); ++it) {
    EXPECT_EQ(*it, "world");
    *it = "hello";
  }
  for (auto it = ls.crbegin(); it != ls.crend(); ++it) {
    EXPECT_EQ(*it, "hello");
  }
  for (auto it = lli.begin(); it != lli.end(); ++it) {
    EXPECT_EQ(*it, list<int>({1, 2, 3}));
    *it = {4, 5, 6};
  }
  for (auto it = lli.cbegin(); it != lli.cend(); ++it)
    EXPECT_EQ(*it, list<int>({4, 5, 6}));
  for (auto it = lli.rbegin(); it != lli.rend(); ++it) {
    EXPECT_EQ(*it, list<int>({4, 5, 6}));
    *it = {7, 8, 9};
  }
  for (auto it = lli.crbegin(); it != lli.crend(); ++it) {
    EXPECT_EQ(*it, list<int>({7, 8, 9}));
  }
}

TEST_F(ListTest, FRONT_AND_BACK) {
  EXPECT_EQ(li.front(), 1);
  li.front() = 2;
  EXPECT_EQ(li.front(), 2);
  EXPECT_EQ(li.back(), 5);
  li.back() = 2;
  EXPECT_EQ(li.back(), 2);
  EXPECT_EQ(lf.front(), 1.0f);
  lf.front() = 2.0f;
  EXPECT_EQ(lf.front(), 2.0f);
  EXPECT_EQ(lf.back(), 5.0f);
  lf.back() = 2.0f;
  EXPECT_EQ(lf.back(), 2.0f);
  EXPECT_EQ(ld.front(), 1.0f);
  ld.front() = 2.0f;
  EXPECT_EQ(ld.front(), 2.0f);
  EXPECT_EQ(ld.back(), 5.0f);
  ld.back() = 2.0f;
  EXPECT_EQ(ld.back(), 2.0f);
  EXPECT_EQ(ls.front(), "hello");
  ls.front() = "world";
  EXPECT_EQ(ls.front(), "world");
  EXPECT_EQ(ls.back(), "hello");
  ls.back() = "world";
  EXPECT_EQ(ls.back(), "world");
  EXPECT_EQ(lli.front(), list<int>({1, 2, 3}));
  lli.front() = {4, 5, 6};
  EXPECT_EQ(lli.front(), list<int>({4, 5, 6}));
  EXPECT_EQ(lli.back(), list<int>({1, 2, 3}));
  lli.back() = {4, 5, 6};
  EXPECT_EQ(lli.back(), list<int>({4, 5, 6}));
}

TEST_F(ListTest, SIZE) {
  EXPECT_EQ(li.size(), 5);
  li.resize(3);
  EXPECT_EQ(li.size(), 3);
  li.resize(10, 5);
  EXPECT_EQ(li.size(), 10);
  auto it = li.begin();
  while (*it != 5) ++it;
  while (it != li.end()) EXPECT_EQ(*it++, 5);
}

TEST_F(ListTest, COMPARATOR) {
  list<int> temp_li = {1, 2, 3, 4, 5};
  EXPECT_TRUE(temp_li == li);
  EXPECT_FALSE(temp_li != li);
  list<float> temp_lf(lf);
  EXPECT_TRUE(temp_lf == lf);
  EXPECT_FALSE(temp_lf != lf);
  list<double> temp_ld(ld.cbegin(), ld.cend());
  EXPECT_TRUE(temp_ld == ld);
  EXPECT_FALSE(temp_ld != ld);
  list<string> temp_ls(std::move(ls));
  EXPECT_FALSE(temp_ls == ls);
  EXPECT_TRUE(temp_ls != ls);
  list<list<int>> temp_lli({{1, 2, 3}, {1, 2, 3}});
  EXPECT_TRUE(temp_lli == lli);
  EXPECT_FALSE(temp_lli != lli);
}

TEST_F(ListTest, PUSH_AND_POP) {
  li.push_back(6);
  EXPECT_EQ(li.back(), 6);
  EXPECT_EQ(li.size(), 6);
  li.pop_back();
  EXPECT_EQ(li.back(), 5);
  EXPECT_EQ(li.size(), 5);
  li.push_front(0);
  EXPECT_EQ(li.front(), 0);
  EXPECT_EQ(li.size(), 6);
  li.pop_fornt();
  EXPECT_EQ(li.front(), 1);
  EXPECT_EQ(li.size(), 5);
  lf.push_back(6.0f);
  EXPECT_EQ(lf.back(), 6.0f);
  EXPECT_EQ(lf.size(), 6);
  lf.pop_back();
  EXPECT_EQ(lf.back(), 5.0f);
  EXPECT_EQ(lf.size(), 5);
  lf.push_front(0.0f);
  EXPECT_EQ(lf.front(), 0.0f);
  EXPECT_EQ(lf.size(), 6);
  lf.pop_fornt();
  EXPECT_EQ(lf.front(), 1.0f);
  EXPECT_EQ(lf.size(), 5);
  ld.push_back(6.0f);
  EXPECT_EQ(ld.back(), 6.0f);
  EXPECT_EQ(ld.size(), 6);
  ld.pop_back();
  EXPECT_EQ(ld.back(), 5.0f);
  EXPECT_EQ(ld.size(), 5);
  ld.push_front(0.0f);
  EXPECT_EQ(ld.front(), 0);
  EXPECT_EQ(ld.size(), 6);
  ld.pop_fornt();
  EXPECT_EQ(ld.front(), 1);
  EXPECT_EQ(ld.size(), 5);
  ls.push_back("world");
  EXPECT_EQ(ls.back(), "world");
  EXPECT_EQ(ls.size(), 3);
  ls.pop_back();
  EXPECT_EQ(ls.back(), "hello");
  EXPECT_EQ(ls.size(), 2);
  ls.push_front("miemie");
  EXPECT_EQ(ls.front(), "miemie");
  EXPECT_EQ(ls.size(), 3);
  ls.pop_fornt();
  EXPECT_EQ(ls.front(), "hello");
  EXPECT_EQ(ls.size(), 2);
  lli.push_back({4, 5, 6});
  EXPECT_EQ(lli.back(), list<int>({4, 5, 6}));
  EXPECT_EQ(lli.size(), 3);
  lli.pop_back();
  EXPECT_EQ(lli.back(), list<int>({1, 2, 3}));
  EXPECT_EQ(lli.size(), 2);
  lli.push_front({0, 0, 0});
  EXPECT_EQ(lli.front(), list<int>({0, 0, 0}));
  EXPECT_EQ(lli.size(), 3);
  lli.pop_fornt();
  EXPECT_EQ(lli.front(), list<int>({1, 2, 3}));
  EXPECT_EQ(lli.size(), 2);
}

TEST_F(ListTest, ERASE) {
  li.erase(li.begin());
  EXPECT_EQ(li.front(), 2);
  EXPECT_EQ(li.size(), 4);
  li.clear();
  EXPECT_EQ(li.size(), 0);
  lf.erase(lf.begin());
  EXPECT_EQ(lf.front(), 2.0f);
  EXPECT_EQ(lf.size(), 4);
  lf.clear();
  EXPECT_EQ(lf.size(), 0);
}

TEST_F(ListTest, INSERT) {
  li.insert(li.begin());
  EXPECT_EQ(li.front(), 0);
  EXPECT_EQ(li.size(), 6);
  list<int> temp_li = {1, 2, 3};
  li.insert(li.end(), temp_li.begin(), temp_li.end());
  EXPECT_EQ(li.back(), 3);
  EXPECT_EQ(li.size(), 9);
  lf.insert(lf.begin());
  EXPECT_EQ(lf.front(), 0.0f);
  EXPECT_EQ(lf.size(), 6);
  list<float> temp_lf = {1.0, 2.0, 3.0, 4.0};
  lf.insert(lf.end(), temp_lf.begin(), temp_lf.end());
  EXPECT_EQ(lf.back(), 4.0f);
  EXPECT_EQ(lf.size(), 10);
}

TEST_F(ListTest, ASSIGN) {
  li.assign(5, 2);
  for (auto i : li) EXPECT_EQ(i, 2);
  list<int> temp_li = {1, 2};
  li.assign(temp_li.begin(), temp_li.end());
  EXPECT_EQ(li, temp_li);
  EXPECT_EQ(li.size(), 2);
  li.assign({3, 3, 3, 3, 3, 3});
  for (auto i : li) EXPECT_EQ(i, 3);
  EXPECT_EQ(li.size(), 6);
  lf.assign(5, 2.0f);
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  list<float> temp_lf = {1.0f, 2.0f};
  lf.assign(temp_lf.begin(), temp_lf.end());
  EXPECT_EQ(lf, temp_lf);
  EXPECT_EQ(lf.size(), 2);
  lf.assign({3.1f, 3.1f, 3.1f, 3.1f, 3.1f, 3.1f});
  for (auto f : lf) EXPECT_EQ(f, 3.1f);
  EXPECT_EQ(lf.size(), 6);
  ls.assign(5, "hi");
  for (auto s : ls) EXPECT_EQ(s, "hi");
  list<string> temp_ls = {"world"};
  ls.assign(temp_ls.begin(), temp_ls.end());
  EXPECT_EQ(ls, temp_ls);
  EXPECT_EQ(ls.size(), 1);
  ls.assign({"cat", "cat", "cat"});
  for (auto s : ls) EXPECT_EQ(s, "cat");
  EXPECT_EQ(ls.size(), 3);
}

TEST_F(ListTest, UNIQUE) {
  ls.unique();
  EXPECT_EQ(ls.size(), 1);
  EXPECT_EQ(ls.front(), "hello");
  lli.unique();
  EXPECT_EQ(lli.size(), 1);
  EXPECT_EQ(lli.front(), list<int>({1, 2, 3}));
}

TEST_F(ListTest, SPLICE) {
  list<int> temp_li = {6, 7, 8, 9, 10};
  li.splice(li.end(), temp_li);
  int i = 1;
  for (auto it = li.cbegin(); it != li.cend(); ++it, ++i) EXPECT_EQ(*it, i);
  EXPECT_EQ(temp_li.size(), 0);
  list<float> temp_lf = {6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  lf.splice(lf.begin(), temp_lf, temp_lf.begin());
  EXPECT_EQ(lf.size(), 6);
  EXPECT_EQ(lf.front(), 6.0f);
  lf.splice(lf.end(), temp_lf, temp_lf.begin());
  EXPECT_EQ(lf.size(), 7);
  EXPECT_EQ(lf.back(), 7.0f);
  EXPECT_EQ(temp_lf.size(), 3);
  EXPECT_EQ(temp_lf.front(), 8.0f);
  list<string> temp_ls = {"world", "world"};
  ls.splice(ls.end(), temp_ls, temp_ls.begin(), temp_ls.end());
  EXPECT_EQ(ls.size(), 4);
  EXPECT_EQ(ls.back(), "world");
  EXPECT_EQ(temp_ls.size(), 0);
}

TEST_F(ListTest, MERGE) {
  list<int> temp_li1 = {1, 3, 5, 7, 9};
  list<int> temp_li2 = {2, 4, 6, 8, 10};
  temp_li1.merge(temp_li2);
  int i = 1;
  for (auto it = temp_li1.cbegin(); it != temp_li1.cend(); ++it, ++i)
    EXPECT_EQ(*it, i);
}

TEST_F(ListTest, REVERSE) {
  list<int> temp_li(li);
  li.reverse();
  auto it = li.begin();
  auto rit = temp_li.rbegin();
  while (it != li.end()) EXPECT_EQ(*it++, *rit++);
}

TEST_F(ListTest, SORT) {
  li = {5, 3, 1, 2, 6, 7, 10, 9, 8, 4};
  li.sort();
  int i = 1;
  for (auto it = li.cbegin(); it != li.cend(); ++it, ++i) EXPECT_EQ(*it, i);
}

TEST_F(ListTest, REMOVE) {
  li.remove(1);
  EXPECT_EQ(li.size(), 4);
  EXPECT_EQ(li.front(), 2);
  li.remove(5);
  EXPECT_EQ(li.size(), 3);
  EXPECT_EQ(li.back(), 4);
  ls.remove("hello");
  EXPECT_EQ(ls.size(), 0);
}