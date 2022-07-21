#include "Algorithms/algo/stl_algo.h"
#include "Function/function.h"
#include <Vector/stl_vector.h>
#include <array>
#include <gtest/gtest.h>

// TEST CASES FROM FOLLY https://github.com/facebook/folly
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

template<class T, size_t S>
struct Functor {
  std::array<T, S> data = {{0}};

  // Two operator() with different argument types.
  // The InvokeReference tests use both
  T const &operator()(size_t index) const { return data[index]; }
  T operator()(size_t index, T const &value) {
    T oldvalue = data[index];
    data[index] = value;
    return oldvalue;
  }
};

}// namespace

TEST_F(functionTest, InvokeFunctor) {
  Functor<int, 100> func;
  static_assert(
      sizeof(func) > sizeof(function<int(size_t)>),
      "sizeof(Function) is much larger than expected");
  func(5, 123);
  function<int(size_t) const> getter = MiniSTL::move(func);

  // Function will allocate memory on the heap to store the functor object
  ASSERT_GT(getter.heapAllocatedMemory(), 0);

  ASSERT_EQ(123, getter(5));
}

TEST_F(functionTest, InvokeReference) {
  Functor<int, 10> func;
  func(5, 123);

  // Have Functions for getter and setter, both referencing the same funtor
  function<int(size_t) const> getter = std::ref(func);
  function<int(size_t, int)> setter = std::ref(func);

  ASSERT_EQ(123, getter(5));
  ASSERT_EQ(123, setter(5, 456));
  ASSERT_EQ(123, setter(5, 567));
  ASSERT_EQ(567, getter(5));
}

TEST_F(functionTest, Emptiness_T) {
  function<int(int)> f;
  ASSERT_EQ(f, nullptr);
  ASSERT_EQ(nullptr, f);
  ASSERT_FALSE(f);
  ASSERT_THROW(f(98), std::bad_function_call);

  function<int(int)> g([](int x) { return x + 1; });
  ASSERT_NE(g, nullptr);
  ASSERT_NE(nullptr, g);
  ASSERT_TRUE(bool(g));
  ASSERT_EQ(100, g(99));

  function<int(int)> h(&func_int_int_add_25);
  ASSERT_NE(h, nullptr);
  ASSERT_NE(nullptr, h);
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

  struct CastableToBool {
    bool val;
    CastableToBool(bool b) : val(b) {}
    explicit operator bool() { return val; }
  };
  // models std::function
  struct NullptrTestableInSitu {
    int res;
    explicit NullptrTestableInSitu(std::nullptr_t);
    explicit NullptrTestableInSitu(int i) : res(i) {}
    CastableToBool operator==(std::nullptr_t) const { return res % 3 != 1; }
    int operator()(int in) const { return res * in; }
  };
  struct NullptrTestableOnHeap : NullptrTestableInSitu {
    unsigned char data[1024 - sizeof(NullptrTestableInSitu)];
    using NullptrTestableInSitu::NullptrTestableInSitu;
  };
  function<int(int)> j(NullptrTestableInSitu(2));
  ASSERT_EQ(j, nullptr);
  ASSERT_EQ(nullptr, j);
  ASSERT_FALSE(j);
  ASSERT_THROW(j(107), std::bad_function_call);
  function<int(int)> k(NullptrTestableInSitu(4));
  ASSERT_NE(k, nullptr);
  ASSERT_NE(nullptr, k);
  ASSERT_TRUE(k);
  ASSERT_EQ(428, k(107));
  function<int(int)> l(NullptrTestableOnHeap(2));
  ASSERT_EQ(l, nullptr);
  ASSERT_EQ(nullptr, l);
  ASSERT_FALSE(l);
  ASSERT_THROW(l(107), std::bad_function_call);
  function<int(int)> m(NullptrTestableOnHeap(4));
  ASSERT_NE(m, nullptr);
  ASSERT_NE(nullptr, m);
  ASSERT_TRUE(m);
  ASSERT_EQ(428, m(107));
}

namespace {
template<bool UseSwapMethod>
void swap_test() {
  function<int(int)> mf1(func_int_int_add_25);
  function<int(int)> mf2(func_int_int_add_111);

  ASSERT_EQ(125, mf1(100));
  ASSERT_EQ(211, mf2(100));

  if (UseSwapMethod) {
    mf1.swap(mf2);
  } else {
    swap(mf1, mf2);
  }

  ASSERT_EQ(125, mf2(100));
  ASSERT_EQ(211, mf1(100));

  function<int(int)> mf3(nullptr);
  ASSERT_EQ(mf3, nullptr);

  if (UseSwapMethod) {
    mf1.swap(mf3);
  } else {
    swap(mf1, mf3);
  }

  ASSERT_EQ(211, mf3(100));
  ASSERT_EQ(nullptr, mf1);

  function<int(int)> mf4([](int x) { return x + 222; });
  ASSERT_EQ(322, mf4(100));

  if (UseSwapMethod) {
    mf4.swap(mf3);
  } else {
    swap(mf4, mf3);
  }
  ASSERT_EQ(211, mf4(100));
  ASSERT_EQ(322, mf3(100));

  if (UseSwapMethod) {
    mf3.swap(mf1);
  } else {
    swap(mf3, mf1);
  }
  ASSERT_EQ(nullptr, mf3);
  ASSERT_EQ(322, mf1(100));
}
}// namespace

TEST_F(functionTest, SwapMethod) {
  swap_test<true>();
}
TEST_F(functionTest, SwapFunction) {
  swap_test<false>();
}

TEST_F(functionTest, Bind) {
  function<float(float, float)> fnc = floatMult;
  auto task = std::bind(MiniSTL::move(fnc), 2.f, 4.f);
  ASSERT_THROW(fnc(0, 0), std::bad_function_call);
  ASSERT_EQ(8, task());
  auto task2(MiniSTL::move(task));
  ASSERT_THROW(task(), std::bad_function_call);
  ASSERT_EQ(8, task2());
}

TEST_F(functionTest, NonCopyableLambda) {
  auto unique_ptr_int = std::make_unique<int>(900);
  ASSERT_EQ(900, *unique_ptr_int);

  struct {
    char data[64];
  } fooData = {{0}};
  (void) fooData;

  auto functor = std::bind(
      [fooData](std::unique_ptr<int> &up) mutable {
        (void) fooData;
        return ++*up;
      },
      MiniSTL::move(unique_ptr_int));

  ASSERT_EQ(901, functor());

  function<int(void)> func = MiniSTL::move(functor);
  ASSERT_GT(func.heapAllocatedMemory(), 0);

  ASSERT_EQ(902, func());
}

TEST_F(functionTest, OverloadedFunctor) {
  struct OverloadedFunctor {
    // variant 1
    int operator()(int x) { return 100 + 1 * x; }

    // variant 2 (const-overload of v1)
    int operator()(int x) const { return 100 + 2 * x; }

    // variant 3
    int operator()(int x, int) { return 100 + 3 * x; }

    // variant 4 (const-overload of v3)
    int operator()(int x, int) const { return 100 + 4 * x; }

    // variant 5 (non-const, has no const-overload)
    int operator()(int x, char const *) { return 100 + 5 * x; }

    // variant 6 (const only)
    int operator()(int x, MiniSTL::vector<int> const &) const { return 100 + 6 * x; }
  };
  OverloadedFunctor of;

  function<int(int)> variant1 = of;
  ASSERT_EQ(100 + 1 * 15, variant1(15));

  function<int(int) const> variant2 = of;
  ASSERT_EQ(100 + 2 * 16, variant2(16));

  function<int(int, int)> variant3 = of;
  ASSERT_EQ(100 + 3 * 17, variant3(17, 0));

  function<int(int, int) const> variant4 = of;
  ASSERT_EQ(100 + 4 * 18, variant4(18, 0));

  function<int(int, char const *)> variant5 = of;
  ASSERT_EQ(100 + 5 * 19, variant5(19, "foo"));

  function<int(int, MiniSTL::vector<int> const &)> variant6 = of;
  ASSERT_EQ(100 + 6 * 20, variant6(20, {}));
  ASSERT_EQ(100 + 6 * 20, variant6(20, {1, 2, 3}));

  function<int(int, MiniSTL::vector<int> const &) const> variant6const = of;
  ASSERT_EQ(100 + 6 * 21, variant6const(21, {}));

  //  auto variant1_const = constCastFunction(MiniSTL::move(variant1));
  //  ASSERT_THROW(variant1(0), std::bad_function_call);
  //  ASSERT_EQ(100 + 1 * 22, variant1_const(22));
  //
  //  function<int(int)> variant2_nonconst = MiniSTL::move(variant2);
  //  ASSERT_THROW(variant2(0), std::bad_function_call);
  //  ASSERT_EQ(100 + 2 * 23, variant2_nonconst(23));
  //
  //  auto variant3_const = constCastFunction(MiniSTL::move(variant3));
  //  ASSERT_THROW(variant3(0, 0), std::bad_function_call);
  //  ASSERT_EQ(100 + 3 * 24, variant3_const(24, 0));
  //
  //  function<int(int, int)> variant4_nonconst = MiniSTL::move(variant4);
  //  ASSERT_THROW(variant4(0, 0), std::bad_function_call);
  //  ASSERT_EQ(100 + 4 * 25, variant4_nonconst(25, 0));
  //
  //  auto variant5_const = constCastFunction(MiniSTL::move(variant5));
  //  ASSERT_THROW(variant5(0, ""), std::bad_function_call);
  //  ASSERT_EQ(100 + 5 * 26, variant5_const(26, "foo"));
  //
  //  auto variant6_const = constCastFunction(MiniSTL::move(variant6));
  //  ASSERT_THROW(variant6(0, {}), std::bad_function_call);
  //  ASSERT_EQ(100 + 6 * 27, variant6_const(27, {}));
  //
  //  function<int(int, MiniSTL::vector<int> const &)> variant6const_nonconst =
  //      MiniSTL::move(variant6const);
  //  ASSERT_THROW(variant6const(0, {}), std::bad_function_call);
  //  ASSERT_EQ(100 + 6 * 28, variant6const_nonconst(28, {}));
}
