﻿#pragma once
#include "Algorithms/algobase/stl_algobase.h"
#include "Function/function_adapter.h"
#include "SequenceContainers/Deque/stl_deque.h"

namespace MiniSTL {

template<class T, class Sequence = deque<T>>
class queue;

template<class T, class Sequence>
bool operator==(const queue<T, Sequence> &, const queue<T, Sequence> &);

template<class T, class Sequence>
bool operator<(const queue<T, Sequence> &, const queue<T, Sequence> &);

template<class T, class Sequence>
class queue {
  // friend declarations
  template<class _T, class _Sequence>
  friend bool operator==(const queue<_T, _Sequence> &,
                         const queue<_T, _Sequence> &);
  template<class _T, class _Sequence>
  friend bool operator<(const queue<_T, _Sequence> &,
                        const queue<_T, _Sequence> &);

 public:// alias declarations
  using value_type = typename Sequence::value_type;
  using size_type = typename Sequence::size_type;
  using reference = typename Sequence::reference;
  using const_reference = typename Sequence::const_reference;

 private:// data member
  Sequence c;

 public:// ctor
  queue() = default;
  explicit queue(const Sequence &rhs) : c(rhs) {}

 public:// copy operations
  queue(const queue &rhs) : c(rhs.c) {}
  queue &operator=(const queue &rhs) {
    c.operator=(rhs.c);
    return *this;
  }

 public:// move operations
  queue(queue &&rhs) noexcept : c(MiniSTL::move(rhs.c)) {}
  queue &operator=(queue &&rhs) noexcept {
    c.operator=(MiniSTL::move(rhs.c));
    return *this;
  }

 public:// getter
  bool empty() const noexcept { return c.empty(); }
  size_type size() const noexcept { return c.size(); }
  const_reference front() const { return c.front(); }
  const_reference back() const { return c.back(); }

 public:// setter
  reference front() { return c.front(); }
  reference back() { return c.back(); }

 public:// push && pop
  void push(const value_type &value) { c.push_back(value); }
  void pop() { c.pop_front(); }

 public:// swap
  void swap(queue &rhs) noexcept { MiniSTL::swap(c, rhs.c); }
};

template<class T, class Sequence>
inline bool operator==(const queue<T, Sequence> &lhs,
                       const queue<T, Sequence> &rhs) {
  return lhs.c == rhs.c;
}

template<class T, class Sequence>
inline bool operator<(const queue<T, Sequence> &lhs,
                      const queue<T, Sequence> &rhs) {
  return lhs.c < rhs.c;
}

template<class T, class Sequence>
inline bool operator!=(const queue<T, Sequence> &lhs,
                       const queue<T, Sequence> &rhs) {
  return !(lhs == rhs);
}

template<class T, class Sequence>
inline bool operator>(const queue<T, Sequence> &lhs,
                      const queue<T, Sequence> &rhs) {
  return rhs < lhs;
}

template<class T, class Sequence>
inline bool operator<=(const queue<T, Sequence> &lhs,
                       const queue<T, Sequence> &rhs) {
  return !(rhs < lhs);
}

template<class T, class Sequence>
inline bool operator>=(const queue<T, Sequence> &lhs,
                       const queue<T, Sequence> &rhs) {
  return !(lhs < rhs);
}

template<class T, class Sequence>
inline void swap(queue<T, Sequence> &lhs, queue<T, Sequence> &rhs) noexcept {
  lhs.swap(rhs);
}

}// namespace MiniSTL