#include "Algorithms/algo/stl_algo.h"
#include "Function/function.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class functionTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

namespace {
int func_int_int_add_25(int x) {
  return x + 25;
}
int func_int_int_add_111(int x) {
  return x + 111;
}
float floatMult(float a, float b) {
  return a * b;
}

}// namespace

TEST_F(functionTest, Emptiness_T) {
  function<int(int)> f;
  ASSERT_EQ(f, nullptr);
  ASSERT_EQ(nullptr, f);
  ASSERT_FALSE(f);
  ASSERT_THROW(f(98), std::bad_function_call);

  function<int(int)> g([](int x) { return x + 1; });
  EXPECT_NE(g, nullptr);
  EXPECT_NE(nullptr, g);
  ASSERT_TRUE(bool(g));
  ASSERT_EQ(100, g(99));

  function<int(int)> h(&func_int_int_add_25);
  EXPECT_NE(h, nullptr);
  EXPECT_NE(nullptr, h);
  ASSERT_TRUE(bool(h));
  ASSERT_EQ(125, h(100));

  h = {};
  ASSERT_EQ(h, nullptr);
  ASSERT_EQ(nullptr, h);
  ASSERT_FALSE(h);
  ASSERT_THROW(h(101), std::bad_function_call);

  function<int(int)> i{function<int(int)>{}};
  ASSERT_EQ(i, nullptr);
  ASSERT_EQ(nullptr, i);
  ASSERT_FALSE(i);
  ASSERT_THROW(i(107), std::bad_function_call);
}
