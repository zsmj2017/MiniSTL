#include <gtest/gtest.h>
#include <string>
#include "stl_set.h"

using namespace ::MiniSTL;
using std::string;

class SetTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // set pre data
    si = {1, 2, 3, 4, 5};
    sf = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    sd = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
    ss = {"hello", "hello"};
    ssi = {{1, 2, 3}, {1, 2, 3}};
  }
  // pre structure
  struct BAR {
    int a;
    double b;
  };
  struct BARComp {
    bool operator()(const BAR& lhs, const BAR& rhs) { return lhs.b < rhs.a; }
  };
  set<int> si;
  set<float> sf;
  set<double> sd;
  set<BAR, BARComp> sb;
  set<string> ss;
  set<set<int>> ssi;
};

TEST_F(SetTest, DEFAULT_CTOR) {
  EXPECT_NO_THROW(set<int>());
  EXPECT_NO_THROW(set<float>());
  EXPECT_NO_THROW(set<double>());
  EXPECT_NO_THROW(set<string>());
  EXPECT_NO_THROW(set<BAR>());
  EXPECT_NO_THROW(set<set<int>>());
}

TEST_F(SetTest, CTOR_WITH_CUSTOM_COMPARATOR) {
  EXPECT_NO_THROW((set<int, greater<int>>()));
  EXPECT_NO_THROW((set<float, greater<float>>()));
  EXPECT_NO_THROW((set<double, greater<double>>()));
  EXPECT_NO_THROW((set<string, greater<string>>()));
  EXPECT_NO_THROW((set<set<int>, greater<set<int>>>()));
  EXPECT_NO_THROW((set<BAR, BARComp>()));
}

TEST_F(SetTest, CTOR_WITH_INPUTITERATOR) {
  EXPECT_NO_THROW(set<int>(si.begin(), si.end()));
  EXPECT_NO_THROW(set<int>(si.cbegin(), si.cend()));
  EXPECT_NO_THROW(set<float>(sf.begin(), sf.end()));
  EXPECT_NO_THROW(set<float>(sf.cbegin(), sf.cend()));
  EXPECT_NO_THROW(set<double>(sd.begin(), sd.end()));
  EXPECT_NO_THROW(set<double>(sd.cbegin(), sd.cend()));
  EXPECT_NO_THROW((set<BAR, BARComp>(sb.begin(), sb.end())));
  EXPECT_NO_THROW((set<BAR, BARComp>(sb.cbegin(), sb.cend())));
  EXPECT_NO_THROW(set<set<int>>(ssi.begin(), ssi.end()));
  EXPECT_NO_THROW(set<set<int>>(ssi.cbegin(), ssi.cend()));
  // C API
  int ai[5] = {1, 2, 3, 4, 5};
  EXPECT_NO_THROW(set<int>(ai, ai + 5));
  double ad[5] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};
  EXPECT_NO_THROW(set<double>(ad, ad + 5));
  string as[5] = {"a", "b", "c", "d", "f"};
  EXPECT_NO_THROW(set<string>(as, as + 5));
}

TEST_F(SetTest, CTOR_WITH_INITIAL_LIST) {
  EXPECT_NO_THROW(set<int>({1, 2, 3, 4, 5}));
  EXPECT_NO_THROW(set<float>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(set<double>({1.0f, 2.0f, 3.0f, 4.0f, 5.0f}));
  EXPECT_NO_THROW(set<string>({"hello", "world"}));
  EXPECT_NO_THROW(set<set<int>>({{1, 2, 3}, {4, 5, 6}}));
}

TEST_F(SetTest, COPY_CTOR) {
  EXPECT_NO_THROW(set<int>{si});
  EXPECT_NO_THROW(set<float>{sf});
  EXPECT_NO_THROW(set<double>{sd});
  EXPECT_NO_THROW(set<string>{ss});
  EXPECT_NO_THROW((set<BAR, BARComp>{sb}));
  EXPECT_NO_THROW(set<set<int>>{ssi});
}

TEST_F(SetTest, MOVE_CTOR) {
  EXPECT_NO_THROW(set<int>(std::move(si)));
  EXPECT_NO_THROW(set<float>(std::move(sf)));
  EXPECT_NO_THROW(set<double>(std::move(sd)));
  EXPECT_NO_THROW(set<string>(std::move(ss)));
  EXPECT_NO_THROW((set<BAR, BARComp>(std::move(sb))));
  EXPECT_NO_THROW((set<set<int>>(std::move(ssi))));
}

TEST_F(SetTest, COPYASSIGN_WITH_SELF) {
  set<int> temp_si;
  temp_si.insert(5);
  temp_si.insert(2);
  si = temp_si;
  EXPECT_NE(si.find(5), si.end());
  EXPECT_NE(si.find(2), si.end());
  set<float> temp_sf;
  temp_sf.insert(5.0);
  temp_sf.insert(2.0);
  sf = temp_sf;
  EXPECT_NE(sf.find(5.0), sf.end());
  EXPECT_NE(sf.find(2.0), sf.end());
  set<double> temp_sd;
  temp_sd.insert(5.0);
  temp_sd.insert(2.0);
  sd = temp_sd;
  EXPECT_NE(sd.find(5.0), sd.end());
  EXPECT_NE(sd.find(2.0), sd.end());
  set<string> temp_ss;
  temp_ss.insert("hello");
  temp_ss.insert("world");
  ss = temp_ss;
  EXPECT_NE(ss.find("hello"), ss.end());
  EXPECT_NE(ss.find("world"), ss.end());
  set<set<int>> temp_ssi;
  temp_ssi.insert({1, 2, 3});
  temp_ssi.insert({4, 5, 6});
  ssi = temp_ssi;
  EXPECT_NE(ssi.find({1, 2, 3}), ssi.end());
  EXPECT_NE(ssi.find({4, 5, 6}), ssi.end());
}

TEST_F(SetTest, COPYASSIGN_WITH_INITIALIZER_LIST) {
  si = {5, 2};
  EXPECT_EQ(si.size(), 2);
  EXPECT_NE(si.find(5), si.end());
  EXPECT_NE(si.find(2), si.end());
  sf = {5.0f, 2.0f};
  EXPECT_EQ(sf.size(), 2);
  EXPECT_NE(sf.find(5), sf.end());
  EXPECT_NE(sf.find(2), sf.end());
  sd = {5.0, 2.0};
  EXPECT_EQ(sd.size(), 2);
  EXPECT_NE(sd.find(5.0), sd.end());
  EXPECT_NE(sd.find(2.0), sd.end());
  ss = {"hello", "world"};
  EXPECT_EQ(ss.size(), 2);
  EXPECT_NE(ss.find("hello"), ss.end());
  EXPECT_NE(ss.find("world"), ss.end());
  ssi = {{1, 2, 3}, {4, 5, 6}};
  EXPECT_EQ(ssi.size(), 2);
  EXPECT_NE(ssi.find({1, 2, 3}), ssi.end());
  EXPECT_NE(ssi.find({4, 5, 6}), ssi.end());
}

TEST_F(SetTest, MOVEASSIGN_WITH_SELF) {
  set<int> temp_si;
  temp_si.insert(5);
  temp_si.insert(2);
  si = std::move(temp_si);
  EXPECT_NE(si.find(5), si.end());
  EXPECT_NE(si.find(2), si.end());
  EXPECT_EQ(temp_si.find(5), temp_si.end());
  EXPECT_EQ(temp_si.find(2), temp_si.end());
  set<float> temp_sf;
  temp_sf.insert(5.0);
  temp_sf.insert(2.0);
  sf = std::move(temp_sf);
  EXPECT_NE(sf.find(5.0), sf.end());
  EXPECT_NE(sf.find(2.0), sf.end());
  EXPECT_EQ(temp_sf.find(5.0f), temp_sf.end());
  EXPECT_EQ(temp_sf.find(2.0f), temp_sf.end());
  set<double> temp_sd;
  temp_sd.insert(5.0);
  temp_sd.insert(2.0);
  sd = std::move(temp_sd);
  EXPECT_NE(sd.find(5.0), sd.end());
  EXPECT_NE(sd.find(2.0), sd.end());
  EXPECT_EQ(temp_sd.find(5.0), temp_sd.end());
  EXPECT_EQ(temp_sd.find(2.0), temp_sd.end());
  set<string> temp_ss;
  temp_ss.insert("hello");
  temp_ss.insert("world");
  ss = std::move(temp_ss);
  EXPECT_NE(ss.find("hello"), ss.end());
  EXPECT_NE(ss.find("world"), ss.end());
  EXPECT_EQ(temp_ss.find("hello"), temp_ss.end());
  EXPECT_EQ(temp_ss.find("world"), temp_ss.end());
  set<set<int>> temp_ssi;
  temp_ssi.insert({1, 2, 3});
  temp_ssi.insert({4, 5, 6});
  ssi = std::move(temp_ssi);
  EXPECT_NE(ssi.find({1, 2, 3}), ssi.end());
  EXPECT_NE(ssi.find({4, 5, 6}), ssi.end());
  EXPECT_EQ(temp_ssi.find({1, 2, 3}), temp_ssi.end());
  EXPECT_EQ(temp_ssi.find({4, 5, 6}), temp_ssi.end());
}

int main(int argc, char* argv[]) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
  return 0;
}