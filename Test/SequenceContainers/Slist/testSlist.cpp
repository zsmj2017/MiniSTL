#include "SequenceContainers/Slist/slist.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class SlistTest : public testing::Test {
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
  slist<int> li;
  slist<float> lf;
  slist<double> ld;
  slist<BAR> lb;
  slist<string> ls;
  slist<slist<int>> lli;
};

TEST_F(SlistTest, DEFAULT_CTOR) {
  EXPECT_NO_THROW(slist<int>());
  EXPECT_NO_THROW(slist<float>());
  EXPECT_NO_THROW(slist<double>());
  EXPECT_NO_THROW(slist<string>());
  EXPECT_NO_THROW(slist<BAR>());
  EXPECT_NO_THROW(slist<slist<int>>());
}

TEST_F(SlistTest, CTOR_WITH_SIZE) {
  EXPECT_NO_THROW(slist<int>(5));
  EXPECT_NO_THROW(slist<float>(5));
  EXPECT_NO_THROW(slist<double>(5));
  EXPECT_NO_THROW(slist<string>(5));
  EXPECT_NO_THROW(slist<BAR>(5));
  EXPECT_NO_THROW(slist<slist<int>>(5));
}

TEST_F(SlistTest, CTOR_WITH_VALUE) {
  EXPECT_NO_THROW(slist<int>(5, 5));
  EXPECT_NO_THROW(slist<float>(5, 5.0f));
  EXPECT_NO_THROW(slist<double>(5, 5.0f));
  EXPECT_NO_THROW(slist<string>(5, "hello"));
  EXPECT_NO_THROW(slist<BAR>(5, BAR()));
  EXPECT_NO_THROW(slist<slist<int>>(5, {1, 2, 3}));
}

TEST_F(SlistTest, CTOR_WITH_INPUTITERATOR) {
  EXPECT_NO_THROW(slist<int>(li.begin(), li.end()));
  EXPECT_NO_THROW(slist<int>(li.cbegin(), li.cend()));
  EXPECT_NO_THROW(slist<float>(lf.begin(), lf.end()));
  EXPECT_NO_THROW(slist<float>(lf.cbegin(), lf.cend()));
  EXPECT_NO_THROW(slist<double>(ld.begin(), ld.end()));
  EXPECT_NO_THROW(slist<double>(ld.cbegin(), ld.cend()));
  EXPECT_NO_THROW(slist<BAR>(lb.begin(), lb.end()));
  EXPECT_NO_THROW(slist<BAR>(lb.cbegin(), lb.cend()));
  EXPECT_NO_THROW(slist<slist<int>>(lli.begin(), lli.end()));
  EXPECT_NO_THROW(slist<slist<int>>(lli.cbegin(), lli.cend()));
  // C API
  int ai[5] = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW(slist<int>(ai, ai + 5));
  double ad[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  EXPECT_NO_THROW(slist<double>(ad, ad + 5));
  string as[5] = {"a", "b", "c", "d", "f"};
  EXPECT_NO_THROW(slist<string>(as, as + 5));
}

TEST_F(SlistTest, CTOR_WITH_INITIAL_LIST) {
  EXPECT_NO_THROW(slist<int>({1, 2, 3, 4, 5}));
  EXPECT_NO_THROW(slist<float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(slist<double>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(slist<string>({"hello", "world"}));
  EXPECT_NO_THROW(slist<slist<int>>({{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(SlistTest, COPY_CTOR) {
  EXPECT_NO_THROW(slist<int>(li));
  EXPECT_NO_THROW(slist<float>(lf));
  EXPECT_NO_THROW(slist<double>(ld));
  EXPECT_NO_THROW(slist<string>(ls));
  EXPECT_NO_THROW(slist<BAR>(lb));
  EXPECT_NO_THROW(slist<slist<int>>(lli));
}

TEST_F(SlistTest, MOVE_CTOR) {
  EXPECT_NO_THROW(slist<int>(std::move(li)));
  EXPECT_NO_THROW(slist<float>(std::move(lf)));
  EXPECT_NO_THROW(slist<double>(std::move(ld)));
  EXPECT_NO_THROW(slist<string>(std::move(ls)));
  EXPECT_NO_THROW(slist<BAR>(std::move(lb)));
  EXPECT_NO_THROW(slist<slist<int>>(std::move(lli)));
}

TEST_F(SlistTest, COPYASSIGN_WITH_SELF) {
  slist<int> temp_li(5, 2);
  li = temp_li;
  for (auto i : li) EXPECT_EQ(i, 2);
  slist<float> temp_lf(5, 2.0f);
  lf = temp_lf;
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  slist<double> temp_ld(5, 2.235f);
  ld = temp_ld;
  for (auto d : ld) EXPECT_EQ(d, 2.235f);
  slist<string> temp_ls(5, "hello");
  ls = temp_ls;
  for (auto s : ls) EXPECT_EQ(s, "hello");
  slist<slist<int>> temp_lli(5, {1, 2, 3});
  lli = temp_lli;
  for (auto li : lli) EXPECT_EQ(li, slist<int>({1, 2, 3}));
}

TEST_F(SlistTest, COPYASSIGN_WITH_INITIAL_LIST) {
  li = {2, 2, 2, 2, 2};
  for (auto i : li) EXPECT_EQ(i, 2);
  lf = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  ld = {2.235f};
  for (auto d : ld) EXPECT_EQ(d, 2.235f);
  ls = {"hello", "hello", "hello"};
  for (auto s : ls) EXPECT_EQ(s, "hello");
  lli = {slist<int>({1, 2, 3}), slist<int>({1, 2, 3})};
  for (auto li : lli) EXPECT_EQ(li, slist<int>({1, 2, 3}));
}

TEST_F(SlistTest, MOVEASSIGN_WITH_SELF) {
  slist<int> temp_li(5, 2);
  li = std::move(temp_li);
  for (auto i : li) EXPECT_EQ(i, 2);
  for (auto i : temp_li) EXPECT_NE(i, 2);
  slist<float> temp_lf(5, 2.0f);
  lf = std::move(temp_lf);
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  for (auto f : temp_lf) EXPECT_NE(f, 2.0f);
  slist<double> temp_ld(5, 2.235f);
  ld = std::move(temp_ld);
  for (auto d : ld) EXPECT_EQ(d, 2.235f);
  for (auto d : temp_ld) EXPECT_NE(d, 2.235f);
  slist<string> temp_ls(5, "hello");
  ls = std::move(temp_ls);
  for (auto s : ls) EXPECT_EQ(s, "hello");
  for (auto s : temp_ls) EXPECT_NE(s, "hello");
  slist<slist<int>> temp_lli(5, {1, 2, 3});
  lli = std::move(temp_lli);
  for (auto li : lli) EXPECT_EQ(li, slist<int>({1, 2, 3}));
  for (auto li : temp_lli) EXPECT_NE(li, slist<int>({1, 2, 3}));
}

TEST_F(SlistTest, BEGIN_AND_END) {
  for (auto it = li.begin(); it != li.end(); ++it) {
    *it = 5;
  }
  for (auto it = li.cbegin(); it != li.cend(); ++it) EXPECT_EQ(*it, 5);
  for (auto it = lf.begin(); it != lf.end(); ++it) {
    *it = 5.0f;
  }
  for (auto it = lf.cbegin(); it != lf.cend(); ++it) EXPECT_EQ(*it, 5.0f);
  for (auto it = ld.begin(); it != ld.end(); ++it) {
    *it = 5.0f;
  }
  for (auto it = ld.cbegin(); it != ld.cend(); ++it) EXPECT_EQ(*it, 5.0f);
  for (auto it = ls.begin(); it != ls.end(); ++it) {
    EXPECT_EQ(*it, "hello");
    *it = "world";
  }
  for (auto it = ls.cbegin(); it != ls.cend(); ++it) EXPECT_EQ(*it, "world");
  for (auto it = lli.begin(); it != lli.end(); ++it) {
    EXPECT_EQ(*it, slist<int>({1, 2, 3}));
    *it = {4, 5, 6};
  }
  for (auto it = lli.cbegin(); it != lli.cend(); ++it)
    EXPECT_EQ(*it, slist<int>({4, 5, 6}));
}

TEST_F(SlistTest, FRONT_AND_BACK) {
  EXPECT_EQ(li.front(), 1);
  li.front() = 2;
  EXPECT_EQ(li.front(), 2);
  EXPECT_EQ(lf.front(), 1.0f);
  lf.front() = 2.0f;
  EXPECT_EQ(lf.front(), 2.0f);
  EXPECT_EQ(ld.front(), 1.0f);
  ld.front() = 2.0f;
  EXPECT_EQ(ld.front(), 2.0f);
  EXPECT_EQ(ls.front(), "hello");
  ls.front() = "world";
  EXPECT_EQ(ls.front(), "world");
  lli.front() = {4, 5, 6};
}

TEST_F(SlistTest, PREVIOUS) {
  EXPECT_EQ(li.before_begin(), li.previous(li.begin()));
  EXPECT_EQ(lf.before_begin(), lf.previous(lf.begin()));
  EXPECT_EQ(ld.before_begin(), ld.previous(ld.begin()));
  EXPECT_EQ(ls.before_begin(), ls.previous(ls.begin()));
  EXPECT_EQ(lli.before_begin(), lli.previous(lli.begin()));
}

TEST_F(SlistTest, SIZE) {
  EXPECT_EQ(li.size(), 5);
  li.resize(3);
  EXPECT_EQ(li.size(), 3);
  li.resize(10, 5);
  EXPECT_EQ(li.size(), 10);
  auto it = li.begin();
  while (*it != 5) ++it;
  while (it != li.end()) EXPECT_EQ(*it++, 5);
}

TEST_F(SlistTest, COMPARATOR) {
  slist<int> temp_li = {1, 2, 3, 4, 5};
  EXPECT_TRUE(temp_li == li);
  EXPECT_FALSE(temp_li != li);
  slist<float> temp_lf(lf);
  EXPECT_TRUE(temp_lf == lf);
  EXPECT_FALSE(temp_lf != lf);
  slist<double> temp_ld(ld.cbegin(), ld.cend());
  EXPECT_TRUE(temp_ld == ld);
  EXPECT_FALSE(temp_ld != ld);
  slist<string> temp_ls(std::move(ls));
  EXPECT_FALSE(temp_ls == ls);
  EXPECT_TRUE(temp_ls != ls);
  slist<slist<int>> temp_lli({{1, 2, 3}, {1, 2, 3}});
  EXPECT_TRUE(temp_lli == lli);
  EXPECT_FALSE(temp_lli != lli);
}

TEST_F(SlistTest, PUSH_AND_POP) {
  li.push_front(0);
  EXPECT_EQ(li.front(), 0);
  EXPECT_EQ(li.size(), 6);
  li.pop_front();
  EXPECT_EQ(li.front(), 1);
  EXPECT_EQ(li.size(), 5);
  lf.push_front(0.0f);
  EXPECT_EQ(lf.front(), 0.0f);
  EXPECT_EQ(lf.size(), 6);
  lf.pop_front();
  EXPECT_EQ(lf.front(), 1.0f);
  EXPECT_EQ(lf.size(), 5);
  ld.push_front(0.0f);
  EXPECT_EQ(ld.front(), 0);
  EXPECT_EQ(ld.size(), 6);
  ld.pop_front();
  EXPECT_EQ(ld.front(), 1);
  EXPECT_EQ(ld.size(), 5);
  ls.push_front("miemie");
  EXPECT_EQ(ls.front(), "miemie");
  EXPECT_EQ(ls.size(), 3);
  ls.pop_front();
  EXPECT_EQ(ls.front(), "hello");
  EXPECT_EQ(ls.size(), 2);
  EXPECT_EQ(lli.size(), 2);
  lli.push_front({0, 0, 0});
  EXPECT_EQ(lli.front(), slist<int>({0, 0, 0}));
  EXPECT_EQ(lli.size(), 3);
}

TEST_F(SlistTest, ERASE) {
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
  ld.erase(ld.begin(), ld.end());
  EXPECT_EQ(ld.size(), 0);
}

TEST_F(SlistTest, INSERT) {
  li.insert(li.begin());
  EXPECT_EQ(li.front(), 0);
  EXPECT_EQ(li.size(), 6);
  slist<int> temp_li = {1, 2, 3};
  li.insert(li.end(), temp_li.begin(), temp_li.end());
  EXPECT_EQ(li.size(), 9);
  lf.insert(lf.begin());
  EXPECT_EQ(lf.front(), 0.0f);
  EXPECT_EQ(lf.size(), 6);
  slist<float> temp_lf = {1.0, 2.0, 3.0, 4.0};
  lf.insert(lf.end(), temp_lf.begin(), temp_lf.end());
  EXPECT_EQ(lf.size(), 10);
}

TEST_F(SlistTest, ASSIGN) {
  li.assign(5, 2);
  for (auto i : li) EXPECT_EQ(i, 2);
  slist<int> temp_li = {1, 2};
  li.assign(temp_li.begin(), temp_li.end());
  EXPECT_EQ(li, temp_li);
  EXPECT_EQ(li.size(), 2);
  li.assign({3, 3, 3, 3, 3, 3});
  for (auto i : li) EXPECT_EQ(i, 3);
  EXPECT_EQ(li.size(), 6);
  lf.assign(5, 2.0f);
  for (auto f : lf) EXPECT_EQ(f, 2.0f);
  slist<float> temp_lf = {1.0f, 2.0f};
  lf.assign(temp_lf.begin(), temp_lf.end());
  EXPECT_EQ(lf, temp_lf);
  EXPECT_EQ(lf.size(), 2);
  lf.assign({3.1f, 3.1f, 3.1f, 3.1f, 3.1f, 3.1f});
  for (auto f : lf) EXPECT_EQ(f, 3.1f);
  EXPECT_EQ(lf.size(), 6);
  ls.assign(5, "hi");
  for (auto s : ls) EXPECT_EQ(s, "hi");
  slist<string> temp_ls = {"world"};
  ls.assign(temp_ls.begin(), temp_ls.end());
  EXPECT_EQ(ls, temp_ls);
  EXPECT_EQ(ls.size(), 1);
  ls.assign({"cat", "cat", "cat"});
  for (auto s : ls) EXPECT_EQ(s, "cat");
  EXPECT_EQ(ls.size(), 3);
}

TEST_F(SlistTest, UNIQUE) {
  ls.unique();
  EXPECT_EQ(ls.size(), 1);
  EXPECT_EQ(ls.front(), "hello");
  lli.unique();
  EXPECT_EQ(lli.size(), 1);
  EXPECT_EQ(lli.front(), slist<int>({1, 2, 3}));
}

TEST_F(SlistTest, SPLICE) {
  slist<int> temp_li = {6, 7, 8, 9, 10};
  li.splice(li.end(), temp_li);
  int i = 1;
  for (auto it = li.cbegin(); it != li.cend(); ++it, ++i) EXPECT_EQ(*it, i);
  EXPECT_EQ(temp_li.size(), 0);
  slist<float> temp_lf = {6.0f, 7.0f, 8.0f, 9.0f, 10.0f};
  lf.splice(lf.begin(), temp_lf, temp_lf.begin());
  EXPECT_EQ(lf.size(), 6);
  EXPECT_EQ(lf.front(), 6.0f);
  lf.splice(lf.end(), temp_lf, temp_lf.begin());
  EXPECT_EQ(lf.size(), 7);
  EXPECT_EQ(temp_lf.size(), 3);
  EXPECT_EQ(temp_lf.front(), 8.0f);
  slist<string> temp_ls = {"world", "world"};
  ls.splice(ls.end(), temp_ls, temp_ls.begin(), temp_ls.end());
  EXPECT_EQ(ls.size(), 4);
  EXPECT_EQ(temp_ls.size(), 0);
}

TEST_F(SlistTest, MERGE) {
  slist<int> temp_li1 = {1, 3, 5, 7, 9};
  slist<int> temp_li2 = {2, 4, 6, 8, 10};
  temp_li1.merge(temp_li2);
  int i = 1;
  for (auto it = temp_li1.cbegin(); it != temp_li1.cend(); ++it, ++i)
    EXPECT_EQ(*it, i);
}

TEST_F(SlistTest, REVERSE) {
  slist<int> temp_li = {5, 4, 3, 2, 1};
  li.reverse();
  EXPECT_EQ(li, temp_li);
}

TEST_F(SlistTest, SORT) {
  li = {5, 3, 1, 2, 6, 7, 10, 9, 8, 4};
  li.sort();
  int i = 1;
  for (auto it = li.cbegin(); it != li.cend(); ++it, ++i) EXPECT_EQ(*it, i);
}

TEST_F(SlistTest, REMOVE) {
  li.remove(1);
  EXPECT_EQ(li.size(), 4);
  EXPECT_EQ(li.front(), 2);
  li.remove(5);
  EXPECT_EQ(li.size(), 3);
  ls.remove("hello");
  EXPECT_EQ(ls.size(), 0);
}