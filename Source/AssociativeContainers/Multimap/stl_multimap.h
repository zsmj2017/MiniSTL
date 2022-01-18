#pragma once

#include "AssociativeContainers/RB-Tree/rb_tree.h"
#include "Function/stl_function.h"

namespace MiniSTL {

// Forward declarations of operators == and <, needed for friend declarations.
template<class Key, class T, class Compare, class Alloc>
class multimap;

template<class Key, class T, class Compare, class Alloc>
inline bool operator==(const multimap<Key, T, Compare, Alloc> &lhs,
                       const multimap<Key, T, Compare, Alloc> &rhs);

template<class Key, class T, class Compare, class Alloc>
inline bool operator<(const multimap<Key, T, Compare, Alloc> &lhs,
                      const multimap<Key, T, Compare, Alloc> &rhs);

template<class Key, class T, class Compare = less<Key>,
         class Alloc = simpleAlloc<T>>
class multimap {
  // friend declarations
  template<class _Key, class _T, class _Compare, class _Alloc>
  friend bool operator==(const multimap<_Key, _T, _Compare, _Alloc> &lhs,
                         const multimap<_Key, _T, _Compare, _Alloc> &rhs);
  template<class _Key, class _T, class _Compare, class _Alloc>
  friend bool operator<(const multimap<_Key, _T, _Compare, _Alloc> &lhs,
                        const multimap<_Key, _T, _Compare, _Alloc> &rhs);

 public:// value comparator
  using key_type = Key;
  using data_type = T;
  using value_type = pair<const Key, T>;
  using key_compare = Compare;
  class value_compare : public binary_function<value_type, value_type, bool> {
    friend class multimap<Key, T, Compare, Alloc>;

   protected:
    Compare comp;
    value_compare(Compare c) : comp(c) {}

   public:
    bool operator()(const value_type &x, const value_type &y) const {
      return comp(x.first, y.first);
    }
  };

 private:// data member
  using rep_type = rb_tree<key_type, value_type, select1st<value_type>,
                           key_compare, Alloc>;
  rep_type t;

 public:// alias declarations
  using pointer = typename rep_type::pointer;
  using const_pointer = typename rep_type::const_pointer;
  using reference = typename rep_type::reference;
  using const_reference = typename rep_type::const_reference;
  using iterator = typename rep_type::iterator;
  using const_iterator = typename rep_type::const_iterator;
  using reverse_iterator = typename rep_type::reverse_iterator;
  using const_reverse_iterator = typename rep_type::const_reverse_iterator;
  using size_type = typename rep_type::size_type;
  using difference_type = typename rep_type::difference_type;

 public:// ctor
  multimap() : t(key_compare()) {}
  explicit multimap(const key_compare &comp) : t(comp) {}
  template<class InputIterator>
  multimap(InputIterator first, InputIterator last) : t(key_compare()) {
    t.insert_equal(first, last);
  }
  template<class InputIterator>
  multimap(InputIterator first, InputIterator last, const key_compare &comp)
      : t(comp) {
    t.insert_equal(first, last);
  }
  multimap(const multimap<Key, T, Compare, Alloc> &rhs) : t(rhs.t) {}

 public:// copy operations
  multimap<Key, T, Compare, Alloc> &operator=(
      const multimap<Key, T, Compare, Alloc> &rhs) {
    t = rhs.t;
    return *this;
  }

 public:// getter
  key_compare key_comp() const noexcept { return t.key_comp(); }
  value_compare value_comp() const noexcept {
    return value_compare(t.key_comp());
  }
  const_iterator begin() const noexcept { return t.begin(); }
  const_iterator end() const noexcept { return t.end(); }
  const_iterator cbegin() const noexcept { return t.cbegin(); }
  const_iterator cend() const noexcept { return t.cend(); }
  const_reverse_iterator rbegin() const noexcept { return t.rbegin(); }
  const_reverse_iterator rend() const noexcept { return t.rend(); }
  const_reverse_iterator crbegin() const noexcept { return t.rbegin(); }
  const_reverse_iterator crend() const noexcept { return t.rend(); }
  bool empty() const noexcept { return t.empty(); }
  size_type size() const noexcept { return t.size(); }
  size_type max_size() const { return t.max_size(); }

 public:// setter
  iterator begin() { return t.begin(); }
  iterator end() { return t.end(); }
  reverse_iterator rbegin() { return t.rbegin(); }
  reverse_iterator rend() { return t.rend(); }

 public:// swap
  void swap(multimap<Key, T, Compare, Alloc> &x) noexcept { t.swap(x.t); }

 public:// insert && erase
  iterator insert(const value_type &x) { return t.insert_equal(x); }
  iterator insert(iterator position, const value_type &x) {
    return t.insert_equal(position, x);
  }
  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_equal(first, last);
  }
  void erase(iterator position) { t.erase(position); }
  size_type erase(const key_type &x) { return t.erase(x); }
  void erase(iterator first, iterator last) { t.erase(first, last); }
  void clear() { t.clear(); }

 public:// find
  iterator find(const key_type &x) noexcept { return t.find(x); }
  const_iterator find(const key_type &x) const noexcept { return t.find(x); }
  size_type count(const key_type &x) const noexcept { return t.count(x); }
  iterator lower_bound(const key_type &x) noexcept {
    return t.lower_bound(x);
  }
  const_iterator lower_bound(const key_type &x) const noexcept {
    return t.lower_bound(x);
  }
  iterator upper_bound(const key_type &x) noexcept {
    return t.upper_bound(x);
  }
  const_iterator upper_bound(const key_type &x) const noexcept {
    return t.upper_bound(x);
  }
  pair<iterator, iterator> equal_range(const key_type &x) noexcept {
    return t.equal_range(x);
  }
  pair<const_iterator, const_iterator> equal_range(const key_type &x) const
      noexcept {
    return t.equal_range(x);
  }
};

template<class Key, class T, class Compare, class Alloc>
inline bool operator==(const multimap<Key, T, Compare, Alloc> &lhs,
                       const multimap<Key, T, Compare, Alloc> &rhs) {
  return lhs.t == rhs.t;
}

template<class Key, class Tp, class Compare, class Alloc>
inline bool operator!=(const multimap<Key, Tp, Compare, Alloc> &lhs,
                       const multimap<Key, Tp, Compare, Alloc> &rhs) {
  return !(lhs.t == rhs.t);
}

template<class Key, class T, class Compare, class Alloc>
inline bool operator<(const multimap<Key, T, Compare, Alloc> &lhs,
                      const multimap<Key, T, Compare, Alloc> &rhs) {
  return lhs.t < rhs.t;
}

template<class Key, class Tp, class Compare, class Alloc>
inline bool operator>(const multimap<Key, Tp, Compare, Alloc> &lhs,
                      const multimap<Key, Tp, Compare, Alloc> &rhs) {
  return rhs < lhs;
}

template<class Key, class Tp, class Compare, class Alloc>
inline bool operator<=(const multimap<Key, Tp, Compare, Alloc> &lhs,
                       const multimap<Key, Tp, Compare, Alloc> &rhs) {
  return !(rhs < lhs);
}

template<class Key, class Tp, class Compare, class Alloc>
inline bool operator>=(const multimap<Key, Tp, Compare, Alloc> &lhs,
                       const multimap<Key, Tp, Compare, Alloc> &rhs) {
  return !(lhs < rhs);
}

template<class Key, class Tp, class Compare, class Alloc>
inline void swap(multimap<Key, Tp, Compare, Alloc> &lhs,
                 multimap<Key, Tp, Compare, Alloc> &rhs) {
  lhs.swap(rhs);
}

}// namespace MiniSTL