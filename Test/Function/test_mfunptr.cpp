#include "Algorithms/algo/stl_algo.h"
#include "Function/function_adapter.h"
#include "SequenceContainers/Vector/stl_vector.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
class MemFunPtrTest : public testing::Test {
 protected:
  void SetUp() override {
  }
};

struct S1 {
} s1;
struct S2 {
} s2;

int f1(S1 &);
int f2(S1 &, S2 &);
int f1c(const S1 &);
int f2c(const S1 &, const S2 &);

void vf1(S1 &);
void vf2(S1 &, S2 &);
void vf1c(const S1 &);
void vf2c(const S1 &, const S2 &);

class Class {
 public:
  int f0();
  int f1(const S1 &);

  void vf0();
  void vf1(const S1 &);

  int f0c() const;
  int f1c(const S1 &) const;

  void vf0c() const;
  void vf1c(const S1 &) const;
};

TEST_F(MemFunPtrTest, mem_ptr_fun) {
  Class obj;
  const Class &objc = obj;

  // ptr_fun

  ptr_fun(f1)(s1);
  ptr_fun(f2)(s1, s2);

  ptr_fun(f1c)(s1);
  ptr_fun(f2c)(s1, s2);

  ptr_fun(vf1)(s1);
  ptr_fun(vf2)(s1, s2);

  ptr_fun(vf1c)(s1);
  ptr_fun(vf2c)(s1, s2);

  // mem_fun

  mem_fun (&Class::f0)(&obj);
  mem_fun (&Class::f1)(&obj, s1);

  mem_fun (&Class::vf0)(&obj);
  mem_fun (&Class::vf1)(&obj, s1);

  // mem_fun (const)

  mem_fun (&Class::f0c)(&objc);
  mem_fun (&Class::f1c)(&objc, s1);

  mem_fun (&Class::vf0c)(&objc);
  mem_fun (&Class::vf1c)(&objc, s1);

  // mem_fun_ref

  mem_fun_ref (&Class::f0)(obj);
  mem_fun_ref (&Class::f1)(obj, s1);

  mem_fun_ref (&Class::vf0)(obj);
  mem_fun_ref (&Class::vf1)(obj, s1);

  // mem_fun_ref (const)
  mem_fun_ref (&Class::f0c)(objc);
  mem_fun_ref (&Class::f1c)(objc, s1);

  mem_fun_ref (&Class::vf0c)(objc);
  mem_fun_ref (&Class::vf1c)(objc, s1);
}
int f1(S1 &) { return 1; }

int f2(S1 &, S2 &) { return 2; }

int f1c(const S1 &) { return 1; }

int f2c(const S1 &, const S2 &) { return 2; }

void vf1(S1 &) {}

void vf2(S1 &, S2 &) {}

void vf1c(const S1 &) {}

void vf2c(const S1 &, const S2 &) {}

int Class::f0() { return 0; }

int Class::f1(const S1 &) { return 1; }

void Class::vf0() {}

void Class::vf1(const S1 &) {}

int Class::f0c() const { return 0; }

int Class::f1c(const S1 &) const { return 1; }

void Class::vf0c() const {}

void Class::vf1c(const S1 &) const {}

struct V {
 public:
  V(int _v) : v(_v) {}

  bool f(int _v) const { return (v == _v); }

  int v;
#if defined(__DMC__)
  V() {}
#endif
};

TEST_F(MemFunPtrTest, find) {
  vector<V> v;

  v.push_back(V(1));
  v.push_back(V(2));
  v.push_back(V(3));

  // step-by-step complication of work for compiler:

  // step 1:
  const_mem_fun1_ref_t<bool, V, int> pmf = mem_fun_ref(&V::f);
  binder2nd<const_mem_fun1_ref_t<bool, V, int>> b(pmf, 2);
  vector<V>::iterator i = find_if(v.begin(), v.end(), b);
  ASSERT_TRUE(i != v.end());
  ASSERT_TRUE(i->v == 2);

  // step 2, just check that compiler understand what pass to bind2nd:
  binder2nd<const_mem_fun1_ref_t<bool, V, int>> b2 = bind2nd(pmf, 2);

  // step 3, the same as step 1, but more intellect from compiler required:
  binder2nd<const_mem_fun1_ref_t<bool, V, int>> b3 = bind2nd(mem_fun_ref(&V::f), 2);

  vector<V>::iterator j = find_if(v.begin(), v.end(), b3);
  ASSERT_TRUE(j != v.end());
  ASSERT_TRUE(j->v == 2);

  // step 4, more brief, more complex:
  vector<V>::iterator k = find_if(v.begin(), v.end(), bind2nd(mem_fun_ref(&V::f), 2));
  ASSERT_TRUE(k != v.end());
  ASSERT_TRUE(k->v == 2);
}
