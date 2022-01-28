#include "Algorithms/algo/stl_algo.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;

class SwapTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

TEST_F(SwapTest, swap1) {
  int a = 42;
  int b = 19;
  swap(a, b);

  ASSERT_TRUE(a == 19);
  ASSERT_TRUE(b == 42);
}

TEST_F(SwapTest, swprnge1) {
  char word1[] = "World";
  char word2[] = "Hello";
  swap_ranges((char *) word1, (char *) word1 + ::strlen(word1), (char *) word2);
  ASSERT_TRUE(!strcmp(word1, "Hello"));
  ASSERT_TRUE(!strcmp(word2, "World"));
}

TEST_F(SwapTest, swap_container_spec) {
  class Obj {
   public:
    Obj() : v(0) {}
    Obj(const Obj &) : v(1) {}

    Obj &operator=(const Obj &) {
      v = 2;
      return *this;
    }

    int v;
  };

  vector<Obj> v1;
  vector<Obj> v2;

  v1.push_back(Obj());
  v1.push_back(Obj());

  v1[0].v = -1;
  v1[1].v = -2;

  v2.push_back(Obj());
  v2.push_back(Obj());
  v2.push_back(Obj());

  v2[0].v = 10;
  v2[1].v = 11;
  v2[2].v = 12;

  ASSERT_TRUE(v1.size() == 2);
  ASSERT_TRUE(v2.size() == 3);

  swap(v1, v2);// this should has effect v1.swap( v2 )

  ASSERT_TRUE(v1.size() == 3);
  ASSERT_TRUE(v2.size() == 2);

  ASSERT_TRUE(v1[0].v == 10);
  ASSERT_TRUE(v1[1].v == 11);
  ASSERT_TRUE(v1[2].v == 12);

  ASSERT_TRUE(v2[0].v == -1);
  ASSERT_TRUE(v2[1].v == -2);
}
