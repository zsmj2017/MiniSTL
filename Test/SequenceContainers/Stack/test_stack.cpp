#include "SequenceContainers/Stack/stl_stack.h"
#include <gtest/gtest.h>
#include <string>

using namespace ::MiniSTL;
using std::string;

class StackTest : public testing::Test {
 protected:
  virtual void SetUp() {
    // set pre data
    si.push(1);
    sf.push(1.0f);
    sd.push(1.0f);
    sb.push(BAR());
    ss.push("hello");
    ss.push("world");
    ssi.push(stack<int>());
  }
  // pre structure
  struct BAR {
    int a;
    double b;
  };
  stack<int> si;
  stack<float> sf;
  stack<double> sd;
  stack<BAR> sb;
  stack<string> ss;
  stack<stack<int>> ssi;
};

TEST_F(StackTest, DEFAULT_CTOR) {
  EXPECT_NO_THROW(stack<int>());
  EXPECT_NO_THROW(stack<float>());
  EXPECT_NO_THROW(stack<double>());
  EXPECT_NO_THROW(stack<string>());
  EXPECT_NO_THROW(stack<BAR>());
  EXPECT_NO_THROW(stack<stack<int>>());
}

TEST_F(StackTest, COPY_CTOR) {
  EXPECT_NO_THROW(stack<int>{si});
  EXPECT_NO_THROW(stack<float>{sf});
  EXPECT_NO_THROW(stack<double>{sd});
  EXPECT_NO_THROW(stack<string>{ss});
  EXPECT_NO_THROW(stack<BAR>{sb});
  EXPECT_NO_THROW(stack<stack<int>>{ssi});
}

TEST_F(StackTest, MOVE_CTOR) {
  EXPECT_NO_THROW(stack<int>(MiniSTL::move(si)));
  EXPECT_NO_THROW(stack<float>(MiniSTL::move(sf)));
  EXPECT_NO_THROW(stack<double>(MiniSTL::move(sd)));
  EXPECT_NO_THROW(stack<string>(MiniSTL::move(ss)));
  EXPECT_NO_THROW(stack<BAR>(MiniSTL::move(sb)));
  EXPECT_NO_THROW(stack<stack<int>>(MiniSTL::move(ssi)));
}

TEST_F(StackTest, DTOR) {
  stack<int> *p = new stack<int>;
  p->push(1);
  int *p2i = &(p->top());
  EXPECT_EQ(*p2i, 1);
  delete p;
  EXPECT_NE(*p2i, 1);
}

TEST_F(StackTest, COPYASSIGN_WITH_SELF) {
  stack<int> temp_si;
  temp_si.push(2);
  si = temp_si;
  EXPECT_EQ(si.top(), 2);
  stack<float> temp_sf;
  temp_sf.push(2.0f);
  sf = temp_sf;
  EXPECT_EQ(sf.top(), 2.0f);
  stack<double> temp_sd;
  temp_sd.push(2.235f);
  sd = temp_sd;
  EXPECT_EQ(sd.top(), 2.235f);
  stack<string> temp_ss;
  temp_ss.push("cat");
  ss = temp_ss;
  EXPECT_EQ(ss.top(), "cat");
  stack<stack<int>> temp_ssi;
  temp_ssi.push(stack<int>());
  temp_ssi.top().push(1);
  ssi = temp_ssi;
  EXPECT_EQ(ssi.top().top(), 1);
}

TEST_F(StackTest, MOVEASSIGN_WITH_SELF) {
  stack<int> temp_si;
  temp_si.push(2);
  si = MiniSTL::move(temp_si);
  EXPECT_EQ(si.top(), 2);
  stack<float> temp_sf;
  temp_sf.push(2.0f);
  sf = temp_sf;
  EXPECT_EQ(sf.top(), 2.0f);
  stack<double> temp_sd;
  temp_sd.push(2.235f);
  sd = temp_sd;
  EXPECT_EQ(sd.top(), 2.235f);
  stack<string> temp_ss;
  temp_ss.push("cat");
  ss = temp_ss;
  EXPECT_EQ(ss.top(), "cat");
  stack<stack<int>> temp_ssi;
  temp_ssi.push(stack<int>());
  temp_ssi.top().push(1);
  ssi = temp_ssi;
  EXPECT_EQ(ssi.top().top(), 1);
}

TEST_F(StackTest, TOP) {
  si.push(2);
  EXPECT_EQ(si.top(), 2);
  si.push(3);
  EXPECT_EQ(si.top(), 3);
  sf.push(2.0f);
  EXPECT_EQ(sf.top(), 2.0f);
  sf.push(3.0f);
  EXPECT_EQ(sf.top(), 3.0f);
  sd.push(2.0f);
  EXPECT_EQ(sd.top(), 2.0f);
  sd.push(3.0f);
  EXPECT_EQ(sd.top(), 3.0f);
  ss.push("cat");
  EXPECT_EQ(ss.top(), "cat");
  ss.push("dog");
  EXPECT_EQ(ss.top(), "dog");
  ssi.push(stack<int>());
  EXPECT_EQ(ssi.top(), stack<int>());
}

TEST_F(StackTest, SIZE) {
  si.push(2);
  EXPECT_EQ(si.size(), 2);
  si.push(3);
  EXPECT_EQ(si.size(), 3);
  sf.push(2.0f);
  EXPECT_EQ(sf.size(), 2);
  sf.push(3.0f);
  EXPECT_EQ(sf.size(), 3);
  sd.push(2.0f);
  EXPECT_EQ(sd.size(), 2);
  sd.push(3.0f);
  EXPECT_EQ(sd.size(), 3);
  ss.push("cat");
  EXPECT_EQ(ss.size(), 3);
  ss.push("dog");
  EXPECT_EQ(ss.size(), 4);
  ssi.push(stack<int>());
  EXPECT_EQ(ssi.size(), 2);
}

TEST_F(StackTest, COMPARATOR) {
  stack<int> temp_si;
  temp_si.push(1);
  EXPECT_TRUE(si == temp_si);
  stack<float> temp_sf;
  temp_sf.push(2.0f);
  EXPECT_TRUE(sf < temp_sf);
  stack<double> temp_sd;
  temp_sd.push(2.235f);
  EXPECT_TRUE(sd <= temp_sd);
  stack<string> temp_ss;
  temp_ss.push("cat");
  EXPECT_TRUE(ss > temp_ss);
  stack<stack<int>> temp_ssi;
  temp_ssi.push(stack<int>());
  temp_ssi.top().push(1);
  EXPECT_TRUE(ssi != temp_ssi);
}

TEST_F(StackTest, PUSH_AND_POP) {
  si.push(2);
  EXPECT_EQ(si.size(), 2);
  EXPECT_EQ(si.top(), 2);
  si.pop();
  EXPECT_EQ(si.size(), 1);
  EXPECT_EQ(si.top(), 1);
  sf.push(2);
  EXPECT_EQ(sf.size(), 2);
  EXPECT_EQ(sf.top(), 2.0f);
  sf.pop();
  EXPECT_EQ(sf.size(), 1);
  EXPECT_EQ(sf.top(), 1);
  sd.push(2);
  EXPECT_EQ(sd.size(), 2);
  EXPECT_EQ(sd.top(), 2.0f);
  sd.pop();
  EXPECT_EQ(sd.size(), 1);
  EXPECT_EQ(sd.top(), 1.0f);
  ss.push("cat");
  EXPECT_EQ(ss.size(), 3);
  EXPECT_EQ(ss.top(), "cat");
  ss.pop();
  EXPECT_EQ(ss.size(), 2);
  EXPECT_EQ(ss.top(), "world");
  ss.pop();
  EXPECT_EQ(ss.size(), 1);
  EXPECT_EQ(ss.top(), "hello");
  ssi.push(stack<int>());
  EXPECT_EQ(ssi.size(), 2);
  EXPECT_EQ(ssi.top(), stack<int>());
  ssi.pop();
  EXPECT_EQ(ssi.size(), 1);
  EXPECT_EQ(ssi.top(), stack<int>());
}

TEST_F(StackTest, SWAP) {
  stack<int> temp_si;
  temp_si.push(2);
  swap(si, temp_si);
  EXPECT_EQ(si.top(), 2);
  EXPECT_EQ(temp_si.top(), 1);
  stack<float> temp_sf;
  temp_sf.push(2.0f);
  swap(sf, temp_sf);
  EXPECT_EQ(sf.top(), 2.0f);
  EXPECT_EQ(temp_sf.top(), 1.0f);
  stack<double> temp_sd;
  temp_sd.push(2.235f);
  swap(sd, temp_sd);
  EXPECT_EQ(sd.top(), 2.235f);
  EXPECT_EQ(temp_sd.top(), 1.0f);
  stack<string> temp_ss;
  temp_ss.push("cat");
  swap(ss, temp_ss);
  EXPECT_EQ(ss.top(), "cat");
  EXPECT_EQ(temp_ss.top(), "world");
  stack<stack<int>> temp_ssi;
  temp_ssi.push(stack<int>());
  temp_ssi.top().push(1);
  swap(ssi, temp_ssi);
  EXPECT_EQ(ssi.top().top(), 1);
  EXPECT_EQ(temp_ssi.top(), stack<int>());
}