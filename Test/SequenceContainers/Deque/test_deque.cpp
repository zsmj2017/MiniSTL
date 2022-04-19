#include "SequenceContainers/Deque/stl_deque.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class DequeTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // set pre data
    di = {1, 2, 3, 4, 5};
    df = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    dd = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    db.resize(5);
    ds = {"hello", "hello"};
    ddi = {{1, 2, 3}, {1, 2, 3}};
  }
  // pre structure
  struct BAR {
    int a;
    double b;
  };
  deque<int> di;
  deque<float> df;
  deque<double> dd;
  deque<BAR> db;
  deque<string> ds;
  deque<deque<int>> ddi;
};

TEST_F(DequeTest, DEFAULT_CTOR) {
  EXPECT_NO_THROW(deque<int>());
  EXPECT_NO_THROW(deque<float>());
  EXPECT_NO_THROW(deque<double>());
  EXPECT_NO_THROW(deque<string>());
  EXPECT_NO_THROW(deque<BAR>());
  EXPECT_NO_THROW(deque<deque<int>>());
}

TEST_F(DequeTest, CTOR_WITH_SIZE) {
  EXPECT_NO_THROW(deque<int>(5));
  EXPECT_NO_THROW(deque<float>(5));
  EXPECT_NO_THROW(deque<double>(5));
  EXPECT_NO_THROW(deque<string>(5));
  EXPECT_NO_THROW(deque<BAR>(5));
  EXPECT_NO_THROW(deque<deque<int>>(5));
}

TEST_F(DequeTest, CTOR_WITH_VALUE) {
  EXPECT_NO_THROW(deque<int>(5, 5));
  EXPECT_NO_THROW(deque<float>(5, 5.0f));
  EXPECT_NO_THROW(deque<double>(5, 5.0f));
  EXPECT_NO_THROW(deque<string>(5, "hello"));
  EXPECT_NO_THROW(deque<BAR>(5, BAR()));
  EXPECT_NO_THROW(deque<deque<int>>(5, {1, 2, 3}));
}

TEST_F(DequeTest, CTOR_WITH_INPUTITERATOR) {
  EXPECT_NO_THROW(deque<int>(di.begin(), di.end()));
  EXPECT_NO_THROW(deque<int>(di.cbegin(), di.cend()));
  EXPECT_NO_THROW(deque<float>(df.begin(), df.end()));
  EXPECT_NO_THROW(deque<float>(df.cbegin(), df.cend()));
  EXPECT_NO_THROW(deque<double>(dd.begin(), dd.end()));
  EXPECT_NO_THROW(deque<double>(dd.cbegin(), dd.cend()));
  EXPECT_NO_THROW(deque<BAR>(db.begin(), db.end()));
  EXPECT_NO_THROW(deque<BAR>(db.cbegin(), db.cend()));
  EXPECT_NO_THROW(deque<deque<int>>(ddi.begin(), ddi.end()));
  EXPECT_NO_THROW(deque<deque<int>>(ddi.cbegin(), ddi.cend()));
  // C API
  int ai[5] = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW(deque<int>(ai, ai + 5));
  double ad[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  EXPECT_NO_THROW(deque<double>(ad, ad + 5));
  string as[5] = {"a", "b", "c", "d", "f"};
  EXPECT_NO_THROW(deque<string>(as, as + 5));
}

TEST_F(DequeTest, CTOR_WITH_INITIAL_LIST) {
  EXPECT_NO_THROW(deque<int>({1, 2, 3, 4, 5}));
  EXPECT_NO_THROW(deque<float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(deque<double>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(deque<string>({"hello", "world"}));
  EXPECT_NO_THROW(deque<deque<int>>({{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(DequeTest, COPY_CTOR) {
  EXPECT_NO_THROW(deque<int>{di});
  EXPECT_NO_THROW(deque<float>{df});
  EXPECT_NO_THROW(deque<double>{dd});
  EXPECT_NO_THROW(deque<string>{ds});
  EXPECT_NO_THROW(deque<BAR>{db});
  EXPECT_NO_THROW(deque<deque<int>>{ddi});
}

TEST_F(DequeTest, MOVE_CTOR) {
  EXPECT_NO_THROW(deque<int>(MiniSTL::move(di)));
  EXPECT_NO_THROW(deque<float>(MiniSTL::move(df)));
  EXPECT_NO_THROW(deque<double>(MiniSTL::move(dd)));
  EXPECT_NO_THROW(deque<string>(MiniSTL::move(ds)));
  EXPECT_NO_THROW(deque<BAR>(MiniSTL::move(db)));
  EXPECT_NO_THROW(deque<deque<int>>(MiniSTL::move(ddi)));
}

TEST_F(DequeTest, DTOR) {
  deque<int> *p = new deque<int>(5, 1);
  int *p2i = &((*p)[0]);
  EXPECT_EQ(*p2i, 1);
  delete p;
  EXPECT_NE(*p2i, 1);
}

TEST_F(DequeTest, COPYASSIGN_WITH_SELF) {
  deque<int> temp_di(5, 2);
  di = temp_di;
  for (auto i : di) EXPECT_EQ(i, 2);
  deque<float> temp_df(5, 2.0f);
  df = temp_df;
  for (auto f : df) EXPECT_EQ(f, 2.0f);
  deque<double> temp_dd(5, 2.235f);
  dd = temp_dd;
  for (auto d : dd) EXPECT_EQ(d, 2.235f);
  deque<string> temp_ds(5, "hello");
  ds = temp_ds;
  for (auto s : ds) EXPECT_EQ(s, "hello");
  deque<deque<int>> temp_ddi(5, {1, 2, 3});
  ddi = temp_ddi;
  for (auto di : ddi) EXPECT_EQ(di, deque<int>({1, 2, 3}));
}

TEST_F(DequeTest, COPYASSIGN_WITH_INITIAL_LIST) {
  di = {2, 2, 2, 2, 2};
  for (auto i : di) EXPECT_EQ(i, 2);
  df = {2.0f, 2.0f, 2.0f, 2.0f, 2.0f};
  for (auto f : df) EXPECT_EQ(f, 2.0f);
  dd = {2.235f};
  for (auto d : dd) EXPECT_EQ(d, 2.235f);
  ds = {"hello", "hello", "hello"};
  for (auto s : ds) EXPECT_EQ(s, "hello");
  ddi = {deque<int>({1, 2, 3}), deque<int>({1, 2, 3})};
  for (auto di : ddi) EXPECT_EQ(di, deque<int>({1, 2, 3}));
}

TEST_F(DequeTest, MOVEASSIGN_WITH_SELF) {
  deque<int> temp_di(5, 2);
  di = MiniSTL::move(temp_di);
  for (auto i : di) EXPECT_EQ(i, 2);
  for (auto i : temp_di) EXPECT_NE(i, 2);
  deque<float> temp_df(5, 2.0f);
  df = MiniSTL::move(temp_df);
  for (auto f : df) EXPECT_EQ(f, 2.0f);
  for (auto f : temp_df) EXPECT_NE(f, 2.0f);
  deque<double> temp_dd(5, 2.235f);
  dd = MiniSTL::move(temp_dd);
  for (auto d : dd) EXPECT_EQ(d, 2.235f);
  for (auto d : temp_dd) EXPECT_NE(d, 2.235f);
  deque<string> temp_ds(5, "hello");
  ds = MiniSTL::move(temp_ds);
  for (auto s : ds) EXPECT_EQ(s, "hello");
  for (auto s : temp_ds) EXPECT_NE(s, "hello");
  deque<deque<int>> temp_ddi(5, {1, 2, 3});
  ddi = MiniSTL::move(temp_ddi);
  for (auto di : ddi) EXPECT_EQ(di, deque<int>({1, 2, 3}));
  for (auto di : temp_ddi) EXPECT_NE(di, deque<int>({1, 2, 3}));
}

TEST_F(DequeTest, BEGIN_AND_END) {
  for (auto it = di.begin(); it != di.end(); ++it) {
    EXPECT_EQ(*it, it - di.begin() + 1);
    *it = 5;
  }
  for (auto it = di.cbegin(); it != di.cend(); ++it) EXPECT_EQ(*it, 5);
  for (auto it = di.rbegin(); it != di.rend(); ++it) {
    EXPECT_EQ(*it, 5);
    *it = 3;
  }
  for (auto it = di.crbegin(); it != di.crend(); ++it) {
    EXPECT_EQ(*it, 3);
  }
  for (auto it = df.begin(); it != df.end(); ++it) {
    EXPECT_EQ(*it, it - df.begin() + 1.0f);
    *it = 5.0f;
  }
  for (auto it = df.cbegin(); it != df.cend(); ++it) EXPECT_EQ(*it, 5.0f);
  for (auto it = df.rbegin(); it != df.rend(); ++it) {
    EXPECT_EQ(*it, 5.0f);
    *it = 3.0f;
  }
  for (auto it = df.crbegin(); it != df.crend(); ++it) {
    EXPECT_EQ(*it, 3.0f);
  }
  for (auto it = dd.begin(); it != dd.end(); ++it) {
    EXPECT_EQ(*it, it - dd.begin() + 1.0f);
    *it = 5.0f;
  }
  for (auto it = dd.cbegin(); it != dd.cend(); ++it) EXPECT_EQ(*it, 5.0f);
  for (auto it = dd.rbegin(); it != dd.rend(); ++it) {
    EXPECT_EQ(*it, 5.0f);
    *it = 3.0f;
  }
  for (auto it = dd.crbegin(); it != dd.crend(); ++it) {
    EXPECT_EQ(*it, 3.0f);
  }
  for (auto it = ds.begin(); it != ds.end(); ++it) {
    EXPECT_EQ(*it, "hello");
    *it = "world";
  }
  for (auto it = ds.cbegin(); it != ds.cend(); ++it) EXPECT_EQ(*it, "world");
  for (auto it = ds.rbegin(); it != ds.rend(); ++it) {
    EXPECT_EQ(*it, "world");
    *it = "hello";
  }
  for (auto it = ds.crbegin(); it != ds.crend(); ++it) {
    EXPECT_EQ(*it, "hello");
  }
  for (auto it = ddi.begin(); it != ddi.end(); ++it) {
    EXPECT_EQ(*it, deque<int>({1, 2, 3}));
    *it = {4, 5, 6};
  }
  for (auto it = ddi.cbegin(); it != ddi.cend(); ++it)
    EXPECT_EQ(*it, deque<int>({4, 5, 6}));
  for (auto it = ddi.rbegin(); it != ddi.rend(); ++it) {
    EXPECT_EQ(*it, deque<int>({4, 5, 6}));
    *it = {7, 8, 9};
  }
  for (auto it = ddi.crbegin(); it != ddi.crend(); ++it) {
    EXPECT_EQ(*it, deque<int>({7, 8, 9}));
  }
}

TEST_F(DequeTest, FRONT_AND_BACK) {
  EXPECT_EQ(di.front(), 1);
  di.front() = 2;
  EXPECT_EQ(di.front(), 2);
  EXPECT_EQ(di.back(), 5);
  di.back() = 2;
  EXPECT_EQ(di.back(), 2);
  EXPECT_EQ(df.front(), 1.0f);
  df.front() = 2.0f;
  EXPECT_EQ(df.front(), 2.0f);
  EXPECT_EQ(df.back(), 5.0f);
  df.back() = 2.0f;
  EXPECT_EQ(df.back(), 2.0f);
  EXPECT_EQ(dd.front(), 1.0f);
  dd.front() = 2.0f;
  EXPECT_EQ(dd.front(), 2.0f);
  EXPECT_EQ(dd.back(), 5.0f);
  dd.back() = 2.0f;
  EXPECT_EQ(dd.back(), 2.0f);
  EXPECT_EQ(ds.front(), "hello");
  ds.front() = "world";
  EXPECT_EQ(ds.front(), "world");
  EXPECT_EQ(ds.back(), "hello");
  ds.back() = "world";
  EXPECT_EQ(ds.back(), "world");
  EXPECT_EQ(ddi.front(), deque<int>({1, 2, 3}));
  ddi.front() = {4, 5, 6};
  EXPECT_EQ(ddi.front(), deque<int>({4, 5, 6}));
  EXPECT_EQ(ddi.back(), deque<int>({1, 2, 3}));
  ddi.back() = {4, 5, 6};
  EXPECT_EQ(ddi.back(), deque<int>({4, 5, 6}));
}

TEST_F(DequeTest, RANDOMACCESS) {
  for (size_t i = 0; i != di.size(); ++i) EXPECT_EQ(di[i], i + 1);
  for (size_t i = 0; i != df.size(); ++i) EXPECT_EQ(df[i], i + 1.0f);
  for (size_t i = 0; i != dd.size(); ++i) EXPECT_EQ(dd[i], i + 1.0f);
  for (size_t i = 0; i != ds.size(); ++i) EXPECT_EQ(ds[i], "hello");
  for (size_t i = 0; i != ddi.size(); ++i)
    EXPECT_EQ(ddi[i], deque<int>({1, 2, 3}));
}

TEST_F(DequeTest, SIZE_AND_CAPACITY) {
  EXPECT_EQ(di.size(), 5);
  di.resize(3);
  EXPECT_EQ(di.size(), 3);
  di.resize(10, 5);
  for (size_t i = 3; i != 10; ++i) EXPECT_EQ(di[i], 5);
  EXPECT_EQ(di.size(), 10);
}

TEST_F(DequeTest, COMPARATOR) {
  deque<int> temp_di = {1, 2, 3, 4, 5};
  EXPECT_TRUE(temp_di == di);
  EXPECT_FALSE(temp_di != di);
  deque<float> temp_df(df);
  EXPECT_TRUE(temp_df == df);
  EXPECT_FALSE(temp_df != df);
  deque<double> temp_dd(dd.cbegin(), dd.cend());
  EXPECT_TRUE(temp_dd == dd);
  EXPECT_FALSE(temp_dd != dd);
  deque<string> temp_ds(MiniSTL::move(ds));
  EXPECT_FALSE(temp_ds == ds);
  EXPECT_TRUE(temp_ds != ds);
  deque<deque<int>> temp_ddi({{1, 2, 3}, {1, 2, 3}});
  EXPECT_TRUE(temp_ddi == ddi);
  EXPECT_FALSE(temp_ddi != ddi);
}

TEST_F(DequeTest, PUSH_AND_POP) {
  di.push_back(6);
  EXPECT_EQ(di.back(), 6);
  EXPECT_EQ(di.size(), 6);
  di.pop_back();
  EXPECT_EQ(di.back(), 5);
  EXPECT_EQ(di.size(), 5);
  df.push_back(6.0f);
  EXPECT_EQ(df.back(), 6.0f);
  EXPECT_EQ(df.size(), 6);
  df.pop_back();
  EXPECT_EQ(df.back(), 5.0f);
  EXPECT_EQ(df.size(), 5);
  dd.push_back(6.0f);
  EXPECT_EQ(dd.back(), 6.0f);
  EXPECT_EQ(dd.size(), 6);
  dd.pop_back();
  EXPECT_EQ(dd.back(), 5.0f);
  EXPECT_EQ(dd.size(), 5);
  ds.push_back("world");
  EXPECT_EQ(ds.back(), "world");
  EXPECT_EQ(ds.size(), 3);
  ds.pop_back();
  EXPECT_EQ(ds.back(), "hello");
  EXPECT_EQ(ds.size(), 2);
  ddi.push_back({4, 5, 6});
  EXPECT_EQ(ddi.back(), deque<int>({4, 5, 6}));
  EXPECT_EQ(ddi.size(), 3);
  ddi.pop_back();
  EXPECT_EQ(ddi.back(), deque<int>({1, 2, 3}));
  EXPECT_EQ(ddi.size(), 2);
}

TEST_F(DequeTest, ERASE) {
  di.erase(di.begin());
  EXPECT_EQ(di.front(), 2);
  EXPECT_EQ(di.size(), 4);
  di.erase(di.end() - 1, di.end());
  EXPECT_EQ(di.back(), 4);
  EXPECT_EQ(di.size(), 3);
  di.clear();
  EXPECT_EQ(di.size(), 0);
  df.erase(df.begin());
  EXPECT_EQ(df.front(), 2.0f);
  EXPECT_EQ(df.size(), 4);
  df.erase(df.end() - 1, df.end());
  EXPECT_EQ(df.back(), 4.0f);
  EXPECT_EQ(df.size(), 3);
  df.clear();
  EXPECT_EQ(df.size(), 0);
  dd.erase(dd.begin() + 1, dd.begin() + 2);
  EXPECT_EQ(dd[1], 3.0f);
  EXPECT_EQ(dd.size(), 4);
  ds.erase(ds.begin() + 1, ds.begin() + 2);
  EXPECT_EQ(ds.size(), 1);
  ddi.erase(ddi.begin() + 1, ddi.begin() + 2);
  EXPECT_EQ(ddi[1], deque<int>({1, 2, 3}));
  EXPECT_EQ(ddi.size(), 1);
}

TEST_F(DequeTest, INSERT) {
  di.insert(di.begin());
  EXPECT_EQ(di.front(), 0);
  EXPECT_EQ(di.size(), 6);
  di.insert(di.begin() + 1, 5);
  EXPECT_EQ(di[1], 5);
  EXPECT_EQ(di.size(), 7);
  deque<int> temp_di = {1, 2, 3};
  di.insert(di.end(), temp_di.begin(), temp_di.end());
  EXPECT_EQ(di.back(), 3);
  EXPECT_EQ(di.size(), 10);
  df.insert(df.begin());
  EXPECT_EQ(df.front(), 0.0f);
  EXPECT_EQ(df.size(), 6);
  df.insert(df.begin() + 1, 5.0f);
  EXPECT_EQ(df[1], 5.0f);
  EXPECT_EQ(df.size(), 7);
  deque<float> temp_df = {1.0, 2.0, 3.0, 4.0};
  df.insert(df.end(), temp_df.begin(), temp_df.end());
  EXPECT_EQ(df.back(), 4.0f);
  EXPECT_EQ(df.size(), 11);
}

TEST_F(DequeTest, ASSIGN) {
  di.assign(5, 2);
  for (auto i : di) EXPECT_EQ(i, 2);
  deque<int> temp_di = {1, 2};
  di.assign(temp_di.begin(), temp_di.end());
  EXPECT_EQ(di, temp_di);
  EXPECT_EQ(di.size(), 2);
  di.assign({3, 3, 3, 3, 3, 3});
  for (auto i : di) EXPECT_EQ(i, 3);
  EXPECT_EQ(di.size(), 6);
  df.assign(5, 2.0f);
  for (auto f : df) EXPECT_EQ(f, 2.0f);
  deque<float> temp_df = {1.0f, 2.0f};
  df.assign(temp_df.begin(), temp_df.end());
  EXPECT_EQ(df, temp_df);
  EXPECT_EQ(df.size(), 2);
  df.assign({3.1f, 3.1f, 3.1f, 3.1f, 3.1f, 3.1f});
  for (auto f : df) EXPECT_EQ(f, 3.1f);
  EXPECT_EQ(df.size(), 6);
  ds.assign(5, "hi");
  for (auto s : ds) EXPECT_EQ(s, "hi");
  deque<string> temp_ds = {"world"};
  ds.assign(temp_ds.begin(), temp_ds.end());
  EXPECT_EQ(ds, temp_ds);
  EXPECT_EQ(ds.size(), 1);
  ds.assign({"cat", "cat", "cat"});
  for (auto s : ds) EXPECT_EQ(s, "cat");
  EXPECT_EQ(ds.size(), 3);
}

// test cases from https://sourceforge.net/projects/stlport/
TEST_F(DequeTest, deque1) {
  deque<int> d;
  d.push_back(4);
  d.push_back(9);
  d.push_back(16);
  d.push_front(1);

  ASSERT_TRUE(d[0] == 1);
  ASSERT_TRUE(d[1] == 4);
  ASSERT_TRUE(d[2] == 9);
  ASSERT_TRUE(d[3] == 16);

  d.pop_front();
  d[2] = 25;

  ASSERT_TRUE(d[0] == 4);
  ASSERT_TRUE(d[1] == 9);
  ASSERT_TRUE(d[2] == 25);

  //Some compile time tests:
  deque<int>::iterator dit = d.begin();
  deque<int>::const_iterator cdit(d.begin());
  ASSERT_TRUE((dit - cdit) == 0);
  ASSERT_TRUE((cdit - dit) == 0);
  ASSERT_TRUE((dit - dit) == 0);
  ASSERT_TRUE((cdit - cdit) == 0);
  ASSERT_TRUE(!((dit < cdit) || (dit > cdit) || (dit != cdit) || !(dit <= cdit) || !(dit >= cdit)));
}

TEST_F(DequeTest, insert) {
  deque<int> d;
  d.push_back(0);
  d.push_back(1);
  d.push_back(2);
  ASSERT_TRUE(d.size() == 3);

  deque<int>::iterator dit;

  //Insertion before begin:
  dit = d.insert(d.begin(), 3);
  ASSERT_TRUE(dit != d.end());
  ASSERT_TRUE(*dit == 3);
  ASSERT_TRUE(d.size() == 4);
  ASSERT_TRUE(d[0] == 3);

  //Insertion after begin:
  dit = d.insert(d.begin() + 1, 4);
  ASSERT_TRUE(dit != d.end());
  ASSERT_TRUE(*dit == 4);
  ASSERT_TRUE(d.size() == 5);
  ASSERT_TRUE(d[1] == 4);

  //Insertion at end:
  dit = d.insert(d.end(), 5);
  ASSERT_TRUE(dit != d.end());
  ASSERT_TRUE(*dit == 5);
  ASSERT_TRUE(d.size() == 6);
  ASSERT_TRUE(d[5] == 5);

  //Insertion before last element:
  dit = d.insert(d.end() - 1, 6);
  ASSERT_TRUE(dit != d.end());
  ASSERT_TRUE(*dit == 6);
  ASSERT_TRUE(d.size() == 7);
  ASSERT_TRUE(d[5] == 6);

  //Insertion of several elements before begin
  d.insert(d.begin(), 2, 7);
  ASSERT_TRUE(d.size() == 9);
  ASSERT_TRUE(d[0] == 7);
  ASSERT_TRUE(d[1] == 7);

  //Insertion of several elements after begin
  //There is more elements to insert than elements before insertion position
  d.insert(d.begin() + 1, 2, 8);
  ASSERT_TRUE(d.size() == 11);
  ASSERT_TRUE(d[1] == 8);
  ASSERT_TRUE(d[2] == 8);

  //There is less elements to insert than elements before insertion position
  d.insert(d.begin() + 3, 2, 9);
  ASSERT_TRUE(d.size() == 13);
  ASSERT_TRUE(d[3] == 9);
  ASSERT_TRUE(d[4] == 9);

  //Insertion of several elements at end:
  d.insert(d.end(), 2, 10);
  ASSERT_TRUE(d.size() == 15);
  ASSERT_TRUE(d[14] == 10);
  ASSERT_TRUE(d[13] == 10);

  //Insertion of several elements before last:
  //There is more elements to insert than elements after insertion position
  d.insert(d.end() - 1, 2, 11);
  ASSERT_TRUE(d.size() == 17);
  ASSERT_TRUE(d[15] == 11);
  ASSERT_TRUE(d[14] == 11);

  //There is less elements to insert than elements after insertion position
  d.insert(d.end() - 3, 2, 12);
  ASSERT_TRUE(d.size() == 19);
  ASSERT_TRUE(d[15] == 12);
  ASSERT_TRUE(d[14] == 12);
}

// TODO::need impl at()
#if 0
TEST_F(DequeTest, at) {
  deque<int> d;
  deque<int> const &cd = d;

  d.push_back(10);
  ASSERT_TRUE(d.at(0) == 10);
  d.at(0) = 20;
  ASSERT_TRUE(cd.at(0) == 20);

  for (;;) {
    try {
      d.at(1) = 20;
      ASSERT_TRUE(false);
    } catch (out_of_range const &) {
      return;
    } catch (...) {
      ASSERT_TRUE(false);
    }
  }
}
#endif

TEST_F(DequeTest, auto_ref) {
  int i;
  deque<int> ref;
  for (i = 0; i < 5; ++i) {
    ref.push_back(i);
  }

  deque<deque<int>> d_d_int(1, ref);
  d_d_int.push_back(d_d_int[0]);
  d_d_int.push_back(ref);
  d_d_int.push_back(d_d_int[0]);
  d_d_int.push_back(d_d_int[0]);
  d_d_int.push_back(ref);

  for (i = 0; i < 5; ++i) {
    ASSERT_TRUE(d_d_int[i] == ref);
  }
}

//This test check that deque implementation do not over optimize
//operation as PointEx copy constructor is trivial
TEST_F(DequeTest, optimizations_check) {
  struct Point {
    int x, y;
  };

  struct PointEx : public Point {
    PointEx() : Point(), builtFromBase(false) {}
    PointEx(const Point &) : builtFromBase(true) {}

    bool builtFromBase;
  };
  deque<Point> d1(1);
  ASSERT_TRUE(d1.size() == 1);

  deque<PointEx> d2(d1.begin(), d1.end());
  ASSERT_TRUE(d2.size() == 1);
  ASSERT_TRUE(d2[0].builtFromBase == true);

  d2.insert(d2.end(), d1.begin(), d1.end());
  ASSERT_TRUE(d2.size() == 2);
  ASSERT_TRUE(d2[1].builtFromBase == true);
}

TEST_F(DequeTest, erase) {
  deque<int> dint;
  dint.push_back(3);
  dint.push_front(2);
  dint.push_back(4);
  dint.push_front(1);
  dint.push_back(5);
  dint.push_front(0);
  dint.push_back(6);

  deque<int>::iterator it(dint.begin() + 1);
  ASSERT_TRUE(*it == 1);

  dint.erase(dint.begin());
  ASSERT_TRUE(*it == 1);

  it = dint.end() - 2;
  ASSERT_TRUE(*it == 5);

  dint.erase(dint.end() - 1);
  ASSERT_TRUE(*it == 5);

  dint.push_back(6);
  dint.push_front(0);

  it = dint.begin() + 2;
  ASSERT_TRUE(*it == 2);

  dint.erase(dint.begin(), dint.begin() + 2);
  ASSERT_TRUE(*it == 2);

  it = dint.end() - 3;
  ASSERT_TRUE(*it == 4);

  dint.erase(dint.end() - 2, dint.end());
  ASSERT_TRUE(*it == 4);
}