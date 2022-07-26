#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class BindTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

class pre_increment : public binary_function<int, int, int> {
 public:
  int operator()(const int incr, int &val) const { return val += incr; }
};

class post_increment : public binary_function<int, int, int> {
 public:
  int operator()(int &val, const int incr) const { return val += incr; }
};

TEST_F(BindTest, bind1st1) {
  int array[3] = {1, 2, 3};
  int *p = remove_if((int *) array, (int *) array + 3, bind1st(less<int>(), 2));

  ASSERT_TRUE(p == &array[2]);
  ASSERT_TRUE(array[0] == 1);
  ASSERT_TRUE(array[1] == 2);

  for_each((int *) array, (int *) array + 3, bind1st(pre_increment(), 1));
  ASSERT_TRUE(array[0] == 2);
  ASSERT_TRUE(array[1] == 3);
  ASSERT_TRUE(array[2] == 4);

  for_each((int *) array, (int *) array + 3, bind2nd(post_increment(), 1));
  ASSERT_TRUE(array[0] == 3);
  ASSERT_TRUE(array[1] == 4);
  ASSERT_TRUE(array[2] == 5);
}

TEST_F(BindTest, bind2nd1) {
  int array[3] = {1, 2, 3};
  replace_if(array, array + 3, binder2nd<greater<int>>(greater<int>(), 2), 4);

  ASSERT_TRUE(array[0] == 1);
  ASSERT_TRUE(array[1] == 2);
  ASSERT_TRUE(array[2] == 4);
}
TEST_F(BindTest, bind2nd2) {
  int array[3] = {1, 2, 3};
  replace_if(array, array + 3, bind2nd(greater<int>(), 2), 4);
  ASSERT_TRUE(array[0] == 1);
  ASSERT_TRUE(array[1] == 2);
  ASSERT_TRUE(array[2] == 4);
}

int test_func1(const int &param1, const int &param2) {
  return param1 + param2;
}

int test_func2(int &param1, int param2) {
  param1 += param2;
  return param1 + param2;
}

TEST_F(BindTest, bind2nd3) {
  int array[3] = {1, 2, 3};
  transform(array, array + 3, array, bind2nd(ptr_fun(test_func1), 1));
  transform(array, array + 3, array, bind1st(ptr_fun(test_func1), -1));
  ASSERT_TRUE(array[0] == 1);
  ASSERT_TRUE(array[1] == 2);
  ASSERT_TRUE(array[2] == 3);

  transform(array, array + 3, array, bind2nd(ptr_fun(test_func2), 10));
  ASSERT_TRUE(array[0] == 21);
  ASSERT_TRUE(array[1] == 22);
  ASSERT_TRUE(array[2] == 23);
}

TEST_F(BindTest, bind_memfn) {
  class A {
   public:
    A() : m_n(0) {}
    void f(int n) const {
      m_n = n;
    }
    int v() const { return m_n; }

   private:
    mutable int m_n;
  };
  A array[3];
  for_each(array, array + 3, bind2nd(mem_fun_ref(&A::f), 12));
  ASSERT_TRUE(array[0].v() == 12);
}
