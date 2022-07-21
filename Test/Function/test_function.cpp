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
      "sizeof(function) is much larger than expected");
  func(5, 123);
  function<int(size_t) const> getter = MiniSTL::move(func);

  // function will allocate memory on the heap to store the functor object
  ASSERT_GT(getter.heapAllocatedMemory(), 0);

  ASSERT_EQ(123, getter(5));
}

TEST_F(functionTest, InvokeReference) {
  Functor<int, 10> func;
  func(5, 123);

  // Have functions for getter and setter, both referencing the same funtor
  function<int(size_t) const> getter = std::ref(func);
  function<int(size_t, int)> setter = std::ref(func);

  ASSERT_EQ(123, getter(5));
  ASSERT_EQ(123, setter(5, 456));
  ASSERT_EQ(456, setter(5, 567));
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
}

TEST_F(functionTest, Lambda) {
  // Non-mutable lambdas: can be stored in a non-const...
  function<int(int)> func = [](int x) { return 1000 + x; };
  ASSERT_EQ(1001, func(1));

  // ...as well as in a const function
  function<int(int) const> func_const = [](int x) { return 2000 + x; };
  ASSERT_EQ(2001, func_const(1));

  // Mutable lambda: can only be stored in a const function:
  int number = 3000;
  function<int()> func_mutable = [number]() mutable { return ++number; };
  ASSERT_EQ(3001, func_mutable());
  ASSERT_EQ(3002, func_mutable());

  function<int(int)> func_const_made_nonconst = std::move(func_const);
  ASSERT_EQ(2002, func_const_made_nonconst(2));
  ASSERT_THROW(func_const(0), std::bad_function_call);
}

// DataMember & MemberFunction

struct MemberFunc {
  int x;
  int getX() const { return x; }
  void setX(int xx) { x = xx; }
};

TEST_F(functionTest, DataMember) {
  MemberFunc mf;
  MemberFunc const &cmf = mf;
  mf.x = 123;

  function<int(MemberFunc const *)> data_getter1 = &MemberFunc::x;
  ASSERT_EQ(123, data_getter1(&cmf));
  function<int(MemberFunc *)> data_getter2 = &MemberFunc::x;
  ASSERT_EQ(123, data_getter2(&mf));
  function<int(MemberFunc const &)> data_getter3 = &MemberFunc::x;
  ASSERT_EQ(123, data_getter3(cmf));
  function<int(MemberFunc &)> data_getter4 = &MemberFunc::x;
  ASSERT_EQ(123, data_getter4(mf));
}

TEST_F(functionTest, MemberFunction) {
  MemberFunc mf;
  MemberFunc const &cmf = mf;
  mf.x = 123;

  function<int(MemberFunc const *)> getter1 = &MemberFunc::getX;
  ASSERT_EQ(123, getter1(&cmf));
  function<int(MemberFunc *)> getter2 = &MemberFunc::getX;
  ASSERT_EQ(123, getter2(&mf));
  function<int(MemberFunc const &)> getter3 = &MemberFunc::getX;
  ASSERT_EQ(123, getter3(cmf));
  function<int(MemberFunc &)> getter4 = &MemberFunc::getX;
  ASSERT_EQ(123, getter4(mf));

  function<void(MemberFunc *, int)> setter1 = &MemberFunc::setX;
  setter1(&mf, 234);
  ASSERT_EQ(234, mf.x);

  function<void(MemberFunc &, int)> setter2 = &MemberFunc::setX;
  setter2(mf, 345);
  ASSERT_EQ(345, mf.x);
}

// CaptureCopyMoveCount & ParameterCopyMoveCount
class CopyMoveTracker {
 public:
  struct ConstructorTag {};

  CopyMoveTracker() = delete;
  explicit CopyMoveTracker(ConstructorTag)
      : data_(std::make_shared<std::pair<size_t, size_t>>(0, 0)) {}

  CopyMoveTracker(CopyMoveTracker const &o) noexcept : data_(o.data_) {
    ++data_->first;
  }
  CopyMoveTracker &operator=(CopyMoveTracker const &o) noexcept {
    data_ = o.data_;
    ++data_->first;
    return *this;
  }

  CopyMoveTracker(CopyMoveTracker &&o) noexcept : data_(o.data_) {
    ++data_->second;
  }
  CopyMoveTracker &operator=(CopyMoveTracker &&o) noexcept {
    data_ = o.data_;
    ++data_->second;
    return *this;
  }

  size_t copyCount() const { return data_->first; }
  size_t moveCount() const { return data_->second; }
  size_t refCount() const { return data_.use_count(); }
  void resetCounters() { data_->first = data_->second = 0; }

 private:
  // copy, move
  std::shared_ptr<std::pair<size_t, size_t>> data_;
};

TEST_F(functionTest, CaptureCopyMoveCount) {
  // This test checks that no unnecessary copies/moves are made.

  CopyMoveTracker cmt(CopyMoveTracker::ConstructorTag{});
  ASSERT_EQ(0, cmt.copyCount());
  ASSERT_EQ(0, cmt.moveCount());
  ASSERT_EQ(1, cmt.refCount());

  // Move into lambda, move lambda into function
  auto lambda1 = [cmt = std::move(cmt)]() { return cmt.moveCount(); };
  function<size_t(void)> uf1 = std::move(lambda1);

  // Max copies: 0. Max copy+moves: 2.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 3);
  ASSERT_LE(cmt.copyCount(), 0);

  cmt.resetCounters();

  // Move into lambda, copy lambda into function
  auto lambda2 = [cmt = std::move(cmt)]() { return cmt.moveCount(); };
  function<size_t(void)> uf2 = lambda2;

  // Max copies: 1. Max copy+moves: 2.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 3);
  ASSERT_LE(cmt.copyCount(), 1);

  // Invoking function must not make copies/moves of the callable
  cmt.resetCounters();
  uf1();
  uf2();
  ASSERT_EQ(0, cmt.copyCount());
  ASSERT_EQ(0, cmt.moveCount());
}

TEST_F(functionTest, ParameterCopyMoveCount) {
  // This test checks that no unnecessary copies/moves are made.

  CopyMoveTracker cmt(CopyMoveTracker::ConstructorTag{});
  ASSERT_EQ(0, cmt.copyCount());
  ASSERT_EQ(0, cmt.moveCount());
  ASSERT_EQ(1, cmt.refCount());

  // pass by value
  function<size_t(CopyMoveTracker)> uf1 = [](CopyMoveTracker c) {
    return c.moveCount();
  };

  cmt.resetCounters();
  uf1(cmt);
  // Max copies: 1. Max copy+moves: 2.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 2);
  ASSERT_LE(cmt.copyCount(), 1);

  cmt.resetCounters();
  uf1(std::move(cmt));
  // Max copies: 1. Max copy+moves: 2.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 2);
  ASSERT_LE(cmt.copyCount(), 0);

  // pass by reference
  function<size_t(CopyMoveTracker &)> uf2 = [](CopyMoveTracker &c) {
    return c.moveCount();
  };

  cmt.resetCounters();
  uf2(cmt);
  // Max copies: 0. Max copy+moves: 0.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 0);
  ASSERT_LE(cmt.copyCount(), 0);

  // pass by const reference
  function<size_t(CopyMoveTracker const &)> uf3 = [](CopyMoveTracker const &c) {
    return c.moveCount();
  };

  cmt.resetCounters();
  uf3(cmt);
  // Max copies: 0. Max copy+moves: 0.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 0);
  ASSERT_LE(cmt.copyCount(), 0);

  // pass by rvalue reference
  function<size_t(CopyMoveTracker &&)> uf4 = [](CopyMoveTracker &&c) {
    return c.moveCount();
  };

  cmt.resetCounters();
  uf4(std::move(cmt));
  // Max copies: 0. Max copy+moves: 0.
  ASSERT_LE(cmt.moveCount() + cmt.copyCount(), 0);
  ASSERT_LE(cmt.copyCount(), 0);
}

// VariadicTemplate & VariadicArguments
struct VariadicTemplateSum {
  int operator()() const { return 0; }
  template<class... Args>
  int operator()(int x, Args... args) const {
    return x + (*this)(args...);
  }
};

TEST_F(functionTest, VariadicTemplate) {
  function<int(int)> uf1 = VariadicTemplateSum();
  function<int(int, int)> uf2 = VariadicTemplateSum();
  function<int(int, int, int)> uf3 = VariadicTemplateSum();

  ASSERT_EQ(66, uf1(66));
  ASSERT_EQ(99, uf2(55, 44));
  ASSERT_EQ(66, uf3(33, 22, 11));
}

struct VariadicArgumentsSum {
  int operator()(int count, ...) const {
    int result = 0;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
      result += va_arg(args, int);
    }
    va_end(args);
    return result;
  }
};

TEST_F(functionTest, VariadicArguments) {
  function<int(int)> uf1 = VariadicArgumentsSum();
  function<int(int, int)> uf2 = VariadicArgumentsSum();
  function<int(int, int, int)> uf3 = VariadicArgumentsSum();

  ASSERT_EQ(0, uf1(0));
  ASSERT_EQ(66, uf2(1, 66));
  ASSERT_EQ(99, uf3(2, 55, 44));
}

// SafeCaptureByReference

template<typename T>
void for_each(
    T const &range,
    function<void(typename T::value_type const &) const> const &func) {
  for (auto const &elem : range) {
    func(elem);
  }
}

TEST_F(functionTest, SafeCaptureByReference) {
  std::vector<int> const vec = {20, 30, 40, 2, 3, 4, 200, 300, 400};
  int sum = 0;
  for_each(vec, [&sum](int x) { sum += x; });
  ASSERT_EQ(999, sum);
}

// IgnoreReturnValue
TEST_F(functionTest, IgnoreReturnValue) {
  int x = 95;
  // Assign a lambda that return int to a function that returns void.
  function<void()> f = [&]() -> int { return ++x; };
  ASSERT_EQ(95, x);
  f();
  ASSERT_EQ(96, x);

  function<int()> g = [&]() -> int { return ++x; };
  function<void()> cg = std::move(g);

  ASSERT_EQ(96, x);
  cg();
  ASSERT_EQ(97, x);
}

// TEST =====================================================================
// ReturnConvertible, ConvertReturnType

TEST_F(functionTest, ReturnConvertible) {
  struct CBase {
    int x;
  };
  struct CDerived : CBase {};

  function<double()> f1 = []() -> int { return 5; };
  ASSERT_EQ(5.0, f1());

  struct Convertible {
    double value;
    /* implicit */ Convertible(double v) noexcept : value{v} {}
    /* implicit */ operator int() const noexcept { return int(value); }
  };
  function<int()> f2 = []() -> Convertible { return 5.2; };
  ASSERT_EQ(5, f2());

  CDerived derived;
  derived.x = 55;

  function<CBase const &()> f3 = [&]() -> CDerived const & { return derived; };
  ASSERT_EQ(55, f3().x);

  function<CBase const &()> f4 = [&]() -> CDerived & { return derived; };
  ASSERT_EQ(55, f4().x);

  function<CBase &()> f5 = [&]() -> CDerived & { return derived; };
  ASSERT_EQ(55, f5().x);

  function<CBase const *()> f6 = [&]() -> CDerived const * { return &derived; };
  ASSERT_EQ(f6()->x, 55);

  function<CBase const *()> f7 = [&]() -> CDerived * { return &derived; };
  ASSERT_EQ(55, f7()->x);

  function<CBase *()> f8 = [&]() -> CDerived * { return &derived; };
  ASSERT_EQ(55, f8()->x);

  function<CBase()> f9 = [&]() -> CDerived {
    auto d = derived;
    d.x = 66;
    return d;
  };
  ASSERT_EQ(66, f9().x);
}

TEST_F(functionTest, ConvertReturnType) {
  struct CBase {
    int x;
  };
  struct CDerived : CBase {};

  struct Convertible {
    double value;
    /* implicit */ Convertible(double v) noexcept : value{v} {}
    /* implicit */ operator int() const noexcept { return int(value); }
  };

  function<int()> f1 = []() -> int { return 5; };
  function<double()> cf1 = std::move(f1);
  ASSERT_EQ(5.0, cf1());
  function<Convertible()> ccf1 = std::move(cf1);
  ASSERT_EQ(5, ccf1());

  function<double()> f2 = []() -> double { return 5.2; };
  function<Convertible()> cf2 = std::move(f2);
  ASSERT_EQ(5, cf2());
  function<double()> ccf2 = std::move(cf2);
  ASSERT_EQ(5.0, ccf2());

  CDerived derived;
  derived.x = 55;

  function<CDerived const &()> f3 = [&]() -> CDerived const & { return derived; };
  function<CBase const &()> cf3 = std::move(f3);
  ASSERT_EQ(55, cf3().x);

  function<CDerived &()> f4 = [&]() -> CDerived & { return derived; };
  function<CBase const &()> cf4 = std::move(f4);
  ASSERT_EQ(55, cf4().x);

  function<CDerived &()> f5 = [&]() -> CDerived & { return derived; };
  function<CBase &()> cf5 = std::move(f5);
  ASSERT_EQ(55, cf5().x);

  function<CDerived const *()> f6 = [&]() -> CDerived const * {
    return &derived;
  };
  function<CBase const *()> cf6 = std::move(f6);
  ASSERT_EQ(55, cf6()->x);

  function<CDerived const *()> f7 = [&]() -> CDerived * { return &derived; };
  function<CBase const *()> cf7 = std::move(f7);
  ASSERT_EQ(55, cf7()->x);

  function<CDerived *()> f8 = [&]() -> CDerived * { return &derived; };
  function<CBase *()> cf8 = std::move(f8);
  ASSERT_EQ(55, cf8()->x);

  function<CDerived()> f9 = [&]() -> CDerived {
    auto d = derived;
    d.x = 66;
    return d;
  };
  function<CBase()> cf9 = std::move(f9);
  ASSERT_EQ(66, cf9().x);
}

TEST_F(functionTest, NoAllocatedMemoryAfterMove) {
  Functor<int, 100> foo;

  function<int(size_t)> func = foo;
  ASSERT_GT(func.heapAllocatedMemory(), 0);

  function<int(size_t)> func2 = std::move(func);
  ASSERT_GT(func2.heapAllocatedMemory(), 0);
  ASSERT_EQ(func.heapAllocatedMemory(), 0);
}

TEST_F(functionTest, ConstCastEmbedded) {
  int x = 0;
  auto functor = [&x]() { ++x; };

  function<void() const> func(functor);
  ASSERT_EQ(func.heapAllocatedMemory(), 0);

  function<void()> func2(std::move(func));
  ASSERT_EQ(func2.heapAllocatedMemory(), 0);
}

TEST_F(functionTest, EmptyAfterConstCast) {
  function<int(size_t)> func;
  ASSERT_FALSE(func);
}

TEST_F(functionTest, SelfStdSwap) {
  function<int()> f = [] { return 42; };
  f.swap(f);
  ASSERT_TRUE(bool(f));
  ASSERT_EQ(42, f());
  std::swap(f, f);
  ASSERT_TRUE(bool(f));
  ASSERT_EQ(42, f());
  swap(f, f);
  ASSERT_TRUE(bool(f));
  ASSERT_EQ(42, f());
}

TEST_F(functionTest, SelfMove) {
  function<int()> f = [] { return 42; };
  function<int()> &g = f;
  f = std::move(g);// shouldn't crash!
  (void) bool(f);  // valid but unspecified state
  f = [] { return 43; };
  ASSERT_TRUE(bool(f));
  ASSERT_EQ(43, f());
}

TEST_F(functionTest, SelfMove2) {
  int alive{0};
  struct arg {
    int *ptr_;
    explicit arg(int *ptr) noexcept : ptr_(ptr) { ++*ptr_; }
    arg(arg &&o) noexcept : ptr_(o.ptr_) { ++*ptr_; }
    arg &operator=(arg &&) = delete;
    ~arg() { --*ptr_; }
  };
  ASSERT_EQ(0, alive);
  function<int()> f = [myarg = arg{&alive}] { return 42; };
  ASSERT_EQ(1, alive);
  function<int()> &g = f;
  f = std::move(g);
  ASSERT_FALSE(bool(f)) << "self-assign is self-destruct";
  ASSERT_EQ(0, alive) << "self-asign is self-destruct";
  f = [] { return 43; };
  ASSERT_EQ(0, alive) << "sanity check against double-destruction";
  ASSERT_TRUE(bool(f));
  ASSERT_EQ(43, f());
}

TEST_F(functionTest, CtorWithCopy) {
  struct X {
    X() {}
    X(X const &)
    noexcept(true) {}
    X &operator=(X const &) = default;
  };
  struct Y {
    Y() {}
    Y(Y const &)
    noexcept(false) {}
    Y(Y &&)
    noexcept(true) {}
    Y &operator=(Y &&) = default;
    Y &operator=(Y const &) = default;
  };
  auto lx = [x = X()] {};
  auto ly = [y = Y()] {};
  ASSERT_TRUE(noexcept(function<void()>(lx)));
  ASSERT_FALSE(noexcept(function<void()>(ly)));
}

TEST_F(functionTest, AllocatedSize) {
  function<void(int)> defaultConstructed;
  ASSERT_EQ(defaultConstructed.heapAllocatedMemory(), 0U)
      << "Default constructed function should have zero allocations";

  // On any platform this has to allocate heap storage, because the captures are
  // larger than the inline size of the function object:
  constexpr size_t kCaptureBytes = sizeof(function<void(int)>) + 1;
  function<void(int)> fromLambda{
      [x = std::array<char, kCaptureBytes>()](int) { (void) x; }};
  // I can't assert much about the size because it's permitted to vary from
  // platform to platform or as optimization levels change, but we can be sure
  // that the lambda must be at least as large as its captures
  ASSERT_GE(fromLambda.heapAllocatedMemory(), kCaptureBytes)
      << "Lambda-derived function's allocated size is smaller than the "
         "lambda's capture size";
}

TEST_F(functionTest, TrivialSmallBig) {
  auto tl = [] { return 7; };
  static_assert(std::is_trivially_copyable_v<decltype(tl)>);

  struct move_nx {
    move_nx() {}
    ~move_nx() {}
    move_nx(move_nx &&) noexcept {}
    void operator=(move_nx &&) = delete;
  };
  auto sl = [o = move_nx{}] { return 7; };
  static_assert(!std::is_trivially_copyable_v<decltype(sl)>);
  static_assert(std::is_nothrow_move_constructible_v<decltype(sl)>);

  struct move_x {
    move_x() {}
    ~move_x() {}
    move_x(move_x &&) noexcept(false) {}
    void operator=(move_x &&) = delete;
  };
  auto hl = [o = move_x{}] { return 7; };
  static_assert(!std::is_trivially_copyable_v<decltype(hl)>);
  static_assert(!std::is_nothrow_move_constructible_v<decltype(hl)>);

  function<int()> t{std::move(tl)};
  function<int()> s{std::move(sl)};
  function<int()> h{std::move(hl)};

  ASSERT_EQ(7, t());
  ASSERT_EQ(7, s());
  ASSERT_EQ(7, h());

  auto t2 = std::move(t);
  auto s2 = std::move(s);
  auto h2 = std::move(h);

  ASSERT_EQ(7, t2());
  ASSERT_EQ(7, s2());
  ASSERT_EQ(7, h2());
}