#pragma once
#include <cstdio>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <type_traits.h>

namespace MiniSTL {

template<typename functionType>
class function;

namespace detail {

template<typename T>
using FunctionNullptrTest =
    decltype(static_cast<bool>(static_cast<T const &>(T(nullptr)) == nullptr));

template<
    typename T,
    enable_if_t<!is_detected_v<FunctionNullptrTest, T>, int> = 0>
constexpr bool isEmptyFunction(T const &) {
  return false;
}
template<
    typename T,
    enable_if_t<is_detected_v<FunctionNullptrTest, T>, int> = 0>
constexpr bool isEmptyFunction(T const &t) {
  return static_cast<bool>(t == nullptr);
}

enum class Op { MOVE,
                NUKE,
                HEAP };

// a callable object
struct Data {
  void *big;
};

size_t exec_(Op, Data *, Data *) noexcept;
using Exec = decltype(&exec_);

template<typename Fun>
static size_t default_exec(Op o, Data *src, Data *dst) noexcept {
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

template<typename ReturnType, typename... Args>
static ReturnType uninit_func(Args...) {
  throw std::bad_function_call();
}

}// namespace detail

template<typename ReturnType, typename... Args>
class function<ReturnType(Args...)> {
  using Data = detail::Data;
  using Op = detail::Op;
  using Exec = detail::Exec;
  using Call = ReturnType (*)(Args..., Data &);

  // mutable for const_cast
  mutable Data data_{};
  Exec exec_{nullptr};

  Call call_{&detail::uninit_func<ReturnType, Args...>};

  size_t exec(Op o, Data *src, Data *dst) const {
    if (!exec_) {
      return 0U;
    }
    return exec_(o, src, dst);
  }

  template<typename Fun>
  static ReturnType call(Args... args, Data &p) noexcept {
    auto &fn = *static_cast<const Fun *>(static_cast<void *>(p.big));
    return static_cast<ReturnType>(fn(static_cast<Args &&>(args)...));
  }

  template<typename Signature>
  function(function<Signature> &&fun) {
    using Fun = function<Signature>;
    if (fun) {
      data_.big = new Fun(static_cast<Fun &&>(fun));
      exec_ = Exec(detail::default_exec<Fun>);
      call_ = &call<Fun>;
    }
  }

 public:// ctor
  // for creating a empty function
  function() = default;
  function(std::nullptr_t) noexcept {}
  // not copyable
  function(const function &) = delete;
  function(function &&that) noexcept : exec_(that.exec_) {
    that.exec_ = nullptr;
    exec(Op::MOVE, &that.data_, &data_);
    call_ = that.call_;
  }

  template<
      typename Fun,
      typename =
          enable_if_t<!is_similar_instantiation_v<function, Fun>>>
  function(Fun fun) noexcept {
    if (detail::isEmptyFunction(fun)) {
      return;
    }
    data_.big = new Fun(static_cast<Fun &&>(fun));
    exec_ = Exec(detail::default_exec<Fun>);
    call_ = &call<Fun>;
  }

  template<
      typename Signature,
      typename Fun = function<Signature>,
      typename = enable_if_t<!is_same_v<function, Fun>>>
  function(function<Signature> &&that) noexcept(
      noexcept(function(move(that))))
      : function(move(that)) {}

  // ctor for creating a function from a member function
  template<
      typename Member,
      typename Class,
      typename = decltype(function(std::mem_fn((Member Class::*) 0)))>
  function(Member Class::*ptr) noexcept {
    if (ptr) {
      *this = std::mem_fn(ptr);
    }
  }

  ~function() { exec(Op::NUKE, &data_, nullptr); }

  function &operator=(const function &) = delete;

 public:// move assignment
  function &operator=(function &&that) noexcept {
    exec(Op::NUKE, &data_, nullptr);
    that.exec(Op::MOVE, &that.data_, &data_);
    exec_ = that.exec_;
    that.exec_ = nullptr;
    return *this;
  }

  template<typename Fun>
  function &operator=(Fun fun) noexcept {
    function(static_cast<Fun &&>(fun)).swap(*this);
    return *this;
  }

  template<typename Signature>
  function &operator=(function<Signature> &&that) noexcept(
      noexcept(function(move(that)))) {
    *this = function(move(that));
    return *this;
  }

  function &operator=(std::nullptr_t) noexcept {
    *this = function();
    return *this;
  }

  template<typename Member, typename Class>
  function &operator=(Member Class::*ptr) noexcept {
    if (ptr) {
      *this = std::mem_fn(ptr);
    } else {
      *this = function();
    }
    return *this;
  }

  auto operator()(Args... args) -> ReturnType {
    return call_(static_cast<Args &&>(args)..., data_);
  }

  void swap(function &that) noexcept { std::swap(*this, that); }

  explicit operator bool() const noexcept { return exec_ != nullptr; }
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
