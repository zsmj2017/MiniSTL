#pragma once
#include "stl_deque.h"

namespace MiniSTL {

template <class T, class Sequence = deque<T>>
class stack;

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>&, const stack<T, Sequence>&);

template <class T, class Sequence>
bool operator<(const stack<T, Sequence>&, const stack<T, Sequence>&);

template <class T, class Sequence>
class stack {
  // friend declarations
  template <class _T, class _Sequence>
  friend bool operator==(const stack<_T, _Sequence>&,
                         const stack<_T, _Sequence>&);
  template <class _T, class _Sequence>
  friend bool operator<(const stack<_T, _Sequence>&,
                        const stack<_T, _Sequence>&);

 public:
  using value_type = typename Sequence::value_type;
  using size_type = typename Sequence::size_type;
  using reference = typename Sequence::reference;
  using const_reference = typename Sequence::const_reference;

 private:  // only data member
  Sequence c;

 public:  // ctor
  stack() = default;
  explicit stack(const Sequence& rhs) : c(rhs) {}

 public:  // copy operations
  stack(const stack& rhs) : c(rhs.c) {}
  stack& operator=(const stack& rhs) { c.operator=(rhs.c); }

 public:  // move operations
  stack(stack&& rhs) noexcept : c(std::move(rhs.c)) {}
  stack& operator=(stack&& rhs) noexcept { c.operator=(std::move(rhs.c)); }

 public:  // getter
  bool empty() const noexcept { return c.empty(); }
  size_type size() const noexcept { return c.size(); }
  const_reference top() const noexcept { return c.back(); }

 public:  // setter
  reference top() { return c.back(); }

 public:  // push && pop
  void push(const value_type& value) { c.push_back(value); }
  void pop() { c.pop_back(); }

 public:  // swap
  void swap(stack& rhs) noexcept { MiniSTL::swap(c, rhs.c); }
};

template <class T, class Sequence>
inline bool operator==(const stack<T, Sequence>& lhs,
                       const stack<T, Sequence>& rhs) {
  return lhs.c == rhs.c;
}

template <class T, class Sequence>
inline bool operator<(const stack<T, Sequence>& lhs,
                      const stack<T, Sequence>& rhs) {
  return lhs.c < rhs.c;
}

template <class T, class Sequence>
inline bool operator!=(const stack<T, Sequence>& lhs,
                       const stack<T, Sequence>& rhs) {
  return !(lhs == rhs);
}

template <class T, class Sequence>
inline bool operator>(const stack<T, Sequence>& lhs,
                      const stack<T, Sequence>& rhs) {
  return rhs < lhs;
}

template <class T, class Sequence>
inline bool operator<=(const stack<T, Sequence>& lhs,
                       const stack<T, Sequence>& rhs) {
  return !(rhs < lhs);
}

template <class T, class Sequence>
inline bool operator>=(const stack<T, Sequence>& lhs,
                       const stack<T, Sequence>& rhs) {
  return !(lhs < rhs);
}

template <class T, class Sequence>
inline void swap(stack<T, Sequence>& lhs, stack<T, Sequence>& rhs) noexcept {
  lhs.swap(rhs);
}

}  // namespace MiniSTL