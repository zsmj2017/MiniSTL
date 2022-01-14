#pragma once

#include "stl_iterator.h"
#include <cstddef>

namespace MiniSTL {

// 缓冲区大小设定函数（在预设情况下传回可容纳的元素个数）
// 若n不为0，则传回n，表示由用户自定
// 若n为0则采用预设值 预设值根据sz（元素大小）而定
inline size_t __deque_buf_size(size_t sz) {
  return sz < 512 ? size_t(512 / sz) : size_t(1);
}

template<class T, class Ref, class Ptr>
struct __deque_iterator {
  // alias declarartions
  using iterator = __deque_iterator<T, T &, T *>;
  using const_iterator = __deque_iterator<T, const T &, const T *>;
  using self = __deque_iterator;

  using iterator_category = random_access_iterator_tag;
  using value_type = T;
  using pointer = Ptr;
  using reference = Ref;
  using size_type = size_t;
  using difference_type = ptrdiff_t;
  using map_pointer = T **;

  // data member
  value_type *cur;  // 当前缓冲区的当前元素
  value_type *first;// 当前缓冲区头
  value_type *last; // 当前缓冲区尾(含备用空间）
  map_pointer node; // 指向管控中心

  static size_t buffer_size() { return __deque_buf_size(sizeof(value_type)); }

  // ctor
  __deque_iterator()
      : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
  __deque_iterator(pointer x, map_pointer y)
      : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
  __deque_iterator(const iterator &rhs)
      : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}

  void set_node(map_pointer new_node) {
    node = new_node;
    first = *new_node;
    last = first + static_cast<difference_type>(buffer_size());
  }

  // dereference
  reference operator*() const { return *cur; }
  pointer operator->() const { return &(operator*()); }

  self &operator++() {
    ++cur;
    if (cur == last) {// 若已抵达尾端
      set_node(node + 1);
      cur = first;
    }
    return *this;
  }

  self operator++(int) {
    self temp = *this;
    ++*this;
    return temp;
  }

  self &operator--() {
    if (cur == first) {
      set_node(node - 1);
      cur = last;
    }
    --cur;
    return *this;
  }

  self operator--(int) {
    self temp = *this;
    --*this;
    return temp;
  }

  // random access
  self &operator+=(difference_type n) {
    difference_type offset = n + (cur - first);
    if (offset >= 0 && offset < static_cast<difference_type>(buffer_size())) {
      // 不需要跳转
      cur += n;
    } else {
      // off_set小于0则必然需要跳转
      difference_type node_offset =
          offset > 0
          ? offset / static_cast<difference_type>(buffer_size())
          : -static_cast<difference_type>((-offset - 1) / buffer_size()) - 1;
      set_node(node + node_offset);
      cur = first + (offset - node_offset * static_cast<difference_type>(buffer_size()));
    }
    return *this;
  }

  self operator+(difference_type n) {
    self temp = *this;
    return temp += n;
  }

  self &operator-=(difference_type n) { return *this += -n; }

  self operator-(difference_type n) {
    self temp = *this;
    return temp -= n;
  }

  reference operator[](difference_type n) { return *(*this + n); }
};

template<class T, class Ref, class Ptr>
inline bool operator==(const __deque_iterator<T, Ref, Ptr> &lhs,
                       const __deque_iterator<T, Ref, Ptr> &rhs) {
  return lhs.cur == rhs.cur;
}

// compare with const
template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline bool operator==(const __deque_iterator<T, RefL, PtrL> &lhs,
                       const __deque_iterator<T, RefR, PtrR> &rhs) {
  return lhs.cur == rhs.cur;
}

template<class T, class Ref, class Ptr>
inline bool operator!=(const __deque_iterator<T, Ref, Ptr> &lhs,
                       const __deque_iterator<T, Ref, Ptr> &rhs) {
  return !(lhs == rhs);
}

template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline bool operator!=(const __deque_iterator<T, RefL, PtrL> &lhs,
                       const __deque_iterator<T, RefR, PtrR> &rhs) {
  return !(lhs == rhs);
}

template<class T, class Ref, class Ptr>
inline bool operator<(const __deque_iterator<T, Ref, Ptr> &lhs,
                      const __deque_iterator<T, Ref, Ptr> &rhs) {
  return (lhs.node == rhs.node) ? (lhs.cur < rhs.cur) : (lhs.node < rhs.node);
}

template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline bool operator<(const __deque_iterator<T, RefL, PtrL> &lhs,
                      const __deque_iterator<T, RefR, PtrR> &rhs) {
  return (lhs.node == rhs.node) ? (lhs.cur < rhs.cur) : (lhs.node < rhs.node);
}

template<class T, class Ref, class Ptr>
inline bool operator>(const __deque_iterator<T, Ref, Ptr> &lhs,
                      const __deque_iterator<T, Ref, Ptr> &rhs) {
  return rhs < lhs;
}

template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline bool operator>(const __deque_iterator<T, RefL, PtrL> &lhs,
                      const __deque_iterator<T, RefR, PtrR> &rhs) {
  return rhs < lhs;
}

template<class T, class Ref, class Ptr>
inline bool operator<=(const __deque_iterator<T, Ref, Ptr> &lhs,
                       const __deque_iterator<T, Ref, Ptr> &rhs) {
  return !(rhs < lhs);
}

template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline bool operator<=(const __deque_iterator<T, RefL, PtrL> &lhs,
                       const __deque_iterator<T, RefR, PtrR> &rhs) {
  return !(rhs < lhs);
}

template<class T, class Ref, class Ptr>
inline bool operator>=(const __deque_iterator<T, Ref, Ptr> &lhs,
                       const __deque_iterator<T, Ref, Ptr> &rhs) {
  return !(lhs < rhs);
}

template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline bool operator>=(const __deque_iterator<T, RefL, PtrL> &lhs,
                       const __deque_iterator<T, RefR, PtrR> &rhs) {
  return !(lhs < rhs);
}

template<class T, class Ref, class Ptr>
inline typename __deque_iterator<T, Ref, Ptr>::difference_type operator-(
    const __deque_iterator<T, Ref, Ptr> &lhs,
    const __deque_iterator<T, Ref, Ptr> &rhs) {
  return typename __deque_iterator<T, Ref, Ptr>::difference_type(
      __deque_iterator<T, Ref, Ptr>::buffer_size() * (lhs.node - rhs.node - 1) + (lhs.cur - lhs.first) + (rhs.last - rhs.cur));
}

template<class T, class RefL, class PtrL, class RefR, class PtrR>
inline typename __deque_iterator<T, RefL, PtrL>::difference_type operator-(
    const __deque_iterator<T, RefL, PtrL> &lhs,
    const __deque_iterator<T, RefR, PtrR> &rhs) {
  return typename __deque_iterator<T, RefL, PtrL>::difference_type(
      __deque_iterator<T, RefL, PtrL>::buffer_size() * (lhs.node - rhs.node - 1) + (lhs.cur - lhs.first) + (rhs.last - rhs.cur));
}

template<class T, class Ref, class Ptr>
inline __deque_iterator<T, Ref, Ptr> operator+(
    ptrdiff_t n, const __deque_iterator<T, Ref, Ptr> &x) {
  return x + n;
}

}// namespace MiniSTL