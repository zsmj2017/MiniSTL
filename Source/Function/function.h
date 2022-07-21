#pragma once
#include "Utils/type_traits.h"
#include <stdexcept>

// src code from folly

namespace MiniSTL {

template<typename functionType>
class function;

namespace detail {

namespace function {
// 可执行对象的操作类型
// move:移动可执行对象
// nuke:析构可执行对象
// heap:获取可执行对象大小
enum class Op { MOVE,
                NUKE,
                HEAP };

union Data {// store callable object
  Data() {}
  void *big;                                          // call object on heap
  std::aligned_storage<6 * sizeof(void *)>::type tiny;// call object on stack
};

// 观察T是否可以由nullptr构造得到，并且支持与nullptr做比较
template<typename T>
using functionNullptrTest =
    decltype(static_cast<bool>(static_cast<T const &>(T(nullptr)) == nullptr));

// T若不可以转换为nullptr_t类型，返回false
// 若可以，则判断t == nullptr
template<
    typename T,
    enable_if_t<!is_detected_v<functionNullptrTest, T>, int> = 0>
constexpr bool isEmptyfunction(T const &) {
  return false;
}
template<
    typename T,
    enable_if_t<is_detected_v<functionNullptrTest, T>, int> = 0>
constexpr bool isEmptyfunction(T const &t) {
  return static_cast<bool>(t == nullptr);
}

// 获取F(Args...)的返回类型
template<typename F, typename... Args>
using CallableResult = decltype(declval<F>()(declval<Args>()...));

// 判断将From转为To的转换是否存在空悬引用
template<
    typename From,
    typename To,
    typename = typename std::enable_if<
        !std::is_reference<To>::value || std::is_reference<From>::value>::type>
using IfSafeResultImpl = decltype(void(static_cast<To>(declval<From>())));

template<typename T>
using CallArg = conditional_t<is_register_pass_v<T>, T, T &&>;

template<typename functionType>
struct functionTraits;

template<typename ReturnType, typename... Args>
struct functionTraits<ReturnType(Args...)> {
  using Call = ReturnType (*)(CallArg<Args>..., Data &);
  using ConstSignature = ReturnType(Args...) const;
  using NonConstSignature = ReturnType(Args...);
  using OtherSignature = ConstSignature;

  template<typename F, typename R = CallableResult<F &, Args...>>
  using IfSafeResult = IfSafeResultImpl<R, ReturnType>;

  template<typename Fun>
  static ReturnType callSmall(CallArg<Args>... args, Data &p) {
    auto &fn = *static_cast<Fun *>(static_cast<void *>(&p.tiny));// call obj on stack
    if constexpr (std::is_void<ReturnType>::value) {
      fn(static_cast<Args &&>(args)...);
    } else {
      return fn(static_cast<Args &&>(args)...);
    }
  }

  template<typename Fun>
  static ReturnType callBig(CallArg<Args>... args, Data &p) {
    auto &fn = *static_cast<Fun *>(p.big);// call obj on heap
    if constexpr (std::is_void<ReturnType>::value) {
      fn(static_cast<Args &&>(args)...);
    } else {
      return fn(static_cast<Args &&>(args)...);
    }
  }

  static ReturnType uninitCall(CallArg<Args>..., Data &) {
    throw std::bad_function_call();
  }

  ReturnType operator()(Args... args) {
    auto &fn = *static_cast<MiniSTL::function<NonConstSignature> *>(this);
    return fn.call_(static_cast<Args &&>(args)..., fn.data_);
  }
};

template<typename ReturnType, typename... Args>
struct functionTraits<ReturnType(Args...) const> {
  using Call = ReturnType (*)(CallArg<Args>..., Data &);
  using ConstSignature = ReturnType(Args...) const;
  using NonConstSignature = ReturnType(Args...);
  using OtherSignature = NonConstSignature;

  template<typename F, typename R = CallableResult<const F &, Args...>>
  using IfSafeResult = IfSafeResultImpl<R, ReturnType>;

  template<typename Fun>
  static ReturnType callSmall(CallArg<Args>... args, Data &p) {
    auto &fn = *static_cast<const Fun *>(static_cast<void *>(&p.tiny));
    if constexpr (std::is_void<ReturnType>::value) {
      fn(static_cast<Args &&>(args)...);
    } else {
      return fn(static_cast<Args &&>(args)...);
    }
  }

  template<typename Fun>
  static ReturnType callBig(CallArg<Args>... args, Data &p) {
    auto &fn = *static_cast<const Fun *>(p.big);
    if constexpr (std::is_void<ReturnType>::value) {
      fn(static_cast<Args &&>(args)...);
    } else {
      return fn(static_cast<Args &&>(args)...);
    }
  }

  static ReturnType uninitCall(CallArg<Args>..., Data &) {
    throw std::bad_function_call();
  }

  ReturnType operator()(Args... args) const {// with const
    auto &fn = *static_cast<const MiniSTL::function<ConstSignature> *>(this);
    return fn.call_(static_cast<Args &&>(args)..., fn.data_);
  }
};

// TODO::need noexpect functionTraits

// 可执行对象的操作器，操作分为三种：
// move(移动可执行对象）
// nuke(析构可执行对象）
// heap(获取可执行对象在堆上的大小）
std::size_t exec_(Op, Data *, Data *) noexcept;
using Exec = decltype(&exec_);
static_assert(noexcept(Exec(nullptr)(Op{}, nullptr, nullptr)), "");// exec must be noexcept

// call obj 三类操作
struct DispatchSmallTrivial {
  template<typename Fun, typename Base>
  static constexpr auto call = Base::template callSmall<Fun>;

  template<std::size_t Size>
  static std::size_t exec_(Op o, Data *src, Data *dst) noexcept {
    switch (o) {
      case Op::MOVE:
        std::memcpy(static_cast<void *>(dst), static_cast<void *>(src), Size);
        break;
      case Op::NUKE:
        break;
      case Op::HEAP:
        break;
    }
    return 0U;
  }
  template<std::size_t size, std::size_t adjust = alignof(Data) - 1>
  static constexpr std::size_t size_ = (size + adjust) & ~adjust;
  template<typename Fun>
  static constexpr auto exec = exec_<size_<sizeof(Fun)>>;
};

struct DispatchSmall {
  template<typename Fun, typename Base>
  static constexpr auto call = Base::template callSmall<Fun>;

  template<typename Fun>
  static std::size_t exec(Op o, Data *src, Data *dst) noexcept {
    switch (o) {
      case Op::MOVE:
        ::new (static_cast<void *>(&dst->tiny)) Fun(static_cast<Fun &&>(
            *static_cast<Fun *>(static_cast<void *>(&src->tiny))));
        // Here is no break, because we need to move the functor.
      case Op::NUKE:
        static_cast<Fun *>(static_cast<void *>(&src->tiny))->~Fun();
        break;
      case Op::HEAP:
        break;
    }
    return 0U;
  }
};

struct DispatchBig {
  template<typename Fun, typename Base>
  static constexpr auto call = Base::template callBig<Fun>;

  template<typename Fun>
  static std::size_t exec(Op o, Data *src, Data *dst) noexcept {
    switch (o) {
      case Op::MOVE:
        dst->big = src->big;
        src->big = nullptr;
        break;
      case Op::NUKE:
        delete static_cast<Fun *>(src->big);
        break;
      case Op::HEAP:
        break;
    }
    return sizeof(Fun);
  }
};

}// namespace function
}// namespace detail

template<typename functionType>
class function final : private detail::function::functionTraits<functionType> {
 public:
  using Data = detail::function::Data;
  using Op = detail::function::Op;

  using Traits = detail::function::functionTraits<functionType>;
  using Call = typename Traits::Call;
  using Exec = detail::function::Exec;

  // sizeof(function) == 64 bytes (on 64-bit platform)
  // data:48 bytes, call_:8 bytes, exec_:8 bytes
  mutable Data data_{};// call obj
  Call call_{&Traits::uninitCall};
  Exec exec_{nullptr};// exec on call obj

  std::size_t exec(Op o, Data *src, Data *dst) const {
    if (!exec_) {
      return 0U;
    }
    return exec_(o, src, dst);
  }

  friend Traits;
  friend class function<typename Traits::OtherSignature>;

  template<typename Signature>
  function(function<Signature> &&fun) {
    using Fun = function<Signature>;
    if (fun) {
      data_.big = new Fun(static_cast<Fun &&>(fun));
      call_ = Traits::template callBig<Fun>;
      exec_ = Exec(detail::function::DispatchBig::exec<Fun>);
    }
  }

  function(function<typename Traits::OtherSignature> &&that) noexcept
      : call_(that.call_), exec_(that.exec_) {
    that.call_ = &Traits::uninitCall;
    that.exec_ = nullptr;
    exec(Op::MOVE, &that.data_, &data_);
  }

 public:
  function() = default;

  // not copyable
  function(const function &) = delete;

  function(function &&that) noexcept : call_(that.call_), exec_(that.exec_) {
    that.call_ = &Traits::uninitCall;
    that.exec_ = nullptr;
    exec(Op::MOVE, &that.data_, &data_);
  }

  function(std::nullptr_t) noexcept {}

  template<
      typename Fun,
      typename =
          enable_if_t<!is_similar_instantiation_v<function, Fun>>,
      typename = typename Traits::template IfSafeResult<Fun>,
      bool IsSmall = sizeof(Fun) <= sizeof(Data::tiny) &&noexcept(Fun(declval<Fun>()))>
  function(Fun fun) noexcept(
      IsSmall &&noexcept(Fun(static_cast<Fun &&>(fun)))) {
    using Dispatch = conditional_t<
        IsSmall && std::is_trivially_copyable_v<Fun>,
        detail::function::DispatchSmallTrivial,
        conditional_t<
            IsSmall,
            detail::function::DispatchSmall,
            detail::function::DispatchBig>>;
    if (detail::function::isEmptyfunction(fun)) {
      return;
    }
    if constexpr (IsSmall) {
      ::new (&data_.tiny) Fun(static_cast<Fun &&>(fun));
    } else {
      data_.big = new Fun(static_cast<Fun &&>(fun));
    }
    call_ = Dispatch::template call<Fun, Traits>;
    exec_ = Exec(Dispatch::template exec<Fun>);
  }

  template<
      typename Signature,
      typename Fun = function<Signature>,
      typename = enable_if_t<!is_same<function, Fun>::value>,
      typename = typename Traits::template IfSafeResult<Fun>>
  function(function<Signature> &&that) noexcept(
      noexcept(function(move(that))))
      : function(std::move(that)) {}
  template<
      typename Member,
      typename Class,
      typename = decltype(function(std::mem_fn((Member Class::*) nullptr)))>
  function(Member Class::*ptr) noexcept {
    if (ptr) {
      *this = std::mem_fn(ptr);
    }
  }

  ~function() { exec(Op::NUKE, &data_, nullptr); }

  function &operator=(const function &) = delete;

  function &operator=(function &&that) noexcept {
    exec(Op::NUKE, &data_, nullptr);
    that.exec(Op::MOVE, &that.data_, &data_);
    exec_ = that.exec_;
    call_ = that.call_;
    that.exec_ = nullptr;
    that.call_ = &Traits::uninitCall;
    return *this;
  }

  template<
      typename Fun,
      typename...,
      bool Nx = noexcept(function(declval<Fun>()))>
  function &operator=(Fun fun) noexcept(Nx) {
    if (Nx) {// 如果可能抛出异常，则析构this，重新构造
      this->~function();
      ::new (this) function(static_cast<Fun &&>(fun));
    } else {
      // Construct a temporary and (nothrow) swap.
      function(static_cast<Fun &&>(fun)).swap(*this);
    }
    return *this;
  }

  template<
      typename Signature,
      typename...,
      typename = typename Traits::template IfSafeResult<function<Signature>>>
  function &operator=(function<Signature> &&that) noexcept(
      noexcept(function(std::move(that)))) {
    return (*this = function(std::move(that)));
  }

  function &operator=(std::nullptr_t) noexcept { return (*this = function()); }

  template<typename Member, typename Class>
  auto operator=(Member Class::*ptr) noexcept
      -> decltype(operator=(std::mem_fn(ptr))) {
    return ptr ? (*this = std::mem_fn(ptr)) : (*this = function());
  }

  // operator()函数
  using Traits::operator();

  void swap(function &that) noexcept { std::swap(*this, that); }

  // 根据exec_是否为空判断是否为空函数
  explicit operator bool() const noexcept { return exec_ != nullptr; }

  std::size_t heapAllocatedMemory() const noexcept {
    return exec(Op::HEAP, nullptr, nullptr);
  }
};

template<typename functionType>
void swap(function<functionType> &lhs, function<functionType> &rhs) noexcept {
  lhs.swap(rhs);
}

template<typename functionType>
bool operator==(const function<functionType> &fn, std::nullptr_t) {
  return !fn;
}

template<typename functionType>
bool operator==(std::nullptr_t, const function<functionType> &fn) {
  return !fn;
}

template<typename functionType>
bool operator!=(const function<functionType> &fn, std::nullptr_t) {
  return !(fn == nullptr);
}

template<typename functionType>
bool operator!=(std::nullptr_t, const function<functionType> &fn) {
  return !(nullptr == fn);
}

}// namespace MiniSTL