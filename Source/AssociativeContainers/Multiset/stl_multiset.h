#pragma once

#include "AssociativeContainers/RB-Tree/rb_tree.h"
#include "Function/stl_function.h"

namespace MiniSTL {

// Forward declarations of operators == and <, needed for friend declarations.
template<class Key, class Compare, class Alloc>
class multiset;

template<class Key, class Compare, class Alloc>
inline bool operator==(const multiset<Key, Compare, Alloc> &lhs,
                       const multiset<Key, Compare, Alloc> &rhs);

template<class Key, class Compare, class Alloc>
inline bool operator<(const multiset<Key, Compare, Alloc> &lhs,
                      const multiset<Key, Compare, Alloc> &rhs);

template<class Key, class Compare = less<Key>, class Alloc = simpleAlloc<Key>>
class multiset {
  // friend declarations
  template<class _Key, class _Compare, class _Alloc>
  friend bool operator==(const multiset<_Key, _Compare, _Alloc> &lhs,
                         const multiset<_Key, _Compare, _Alloc> &rhs);
  template<class _Key, class _Compare, class _Alloc>
  friend bool operator<(const multiset<_Key, _Compare, _Alloc> &lhs,
                        const multiset<_Key, _Compare, _Alloc> &rhs);

 public:
  using key_type = Key;
  using value_type = Key;
  using key_compare = Compare;
  using value_compare = Compare;

 private:// data member
  using rep_type =
      rb_tree<key_type, value_type, identity<value_type>, key_compare, Alloc>;
  rep_type t;//底层红黑树

 public:
  using pointer = typename rep_type::const_pointer;
  using const_pointer = typename rep_type::const_pointer;
  using reference = typename rep_type::const_reference;
  using const_reference = typename rep_type::const_reference;
  using iterator = typename rep_type::const_iterator;
  using const_iterator = typename rep_type::const_iterator;
  // TODO:
  // using reverse_iterator = typename rep_type::const_reverse_iterator;
  // using const_reverse_iterator = typename rep_type::const_reverse_iterator;
  using size_type = typename rep_type::size_type;
  using difference_type = typename rep_type::difference_type;

 public:// ctor
  multiset() : t(key_compare()) {}
  explicit multiset(const key_compare &comp) : t(comp) {}
  template<class InputIterator>
  multiset(InputIterator first, InputIterator last) : t(key_compare()) {
    t.insert_equal(first, last);
  }
  template<class InputIterator>
  multiset(InputIterator first, InputIterator last, const key_compare &comp)
      : t(comp) {
    t.insert_equal(first, last);
  }

 public:// copy operations
  multiset(const multiset<Key, Compare, Alloc> &rhs) : t(rhs.t) {}
  multiset<Key, Compare, Alloc> &operator=(
      const multiset<Key, Compare, Alloc> &rhs) {
    t = rhs.t;
    return *this;
  }

 public:// getter
  key_compare key_comp() const noexcept { return t.key_comp(); }
  value_compare value_comp() const noexcept { return t.key_comp(); }
  iterator begin() const noexcept { return t.cbegin(); }
  iterator end() const noexcept { return t.cend(); }
  const_iterator cbegin() const noexcept { return t.cbegin(); }
  const_iterator cend() const noexcept { return t.cend(); }
  // TODO:
  // reverse_iterator rbegin() const { return t.rbegin(); }
  // reverse_iterator rend() const { return t.rend(); }
  bool empty() const noexcept { return t.empty(); }
  size_type size() const noexcept { return t.size(); }
  size_type max_size() const noexcept { return t.max_size(); }

 public:// swap
  void swap(multiset<Key, Compare, Alloc> &x) noexcept { t.swap(x.t); }

 public:// insert && erase
  iterator insert(const value_type &x) { return t.insert_equal(x); }

  iterator insert(iterator pos, const value_type &x) {
    using rep_iterator = typename rep_type::iterator;
    return t.insert_equal(reinterpret_cast<rep_iterator &>(pos), x);
  }

  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_equal(first, last);
  }

  void erase(iterator pos) {
    using rep_iterator = typename rep_type::iterator;
    t.erase(reinterpret_cast<rep_iterator &>(pos));
  }

  size_type erase(const key_type &x) { return t.erase(x); }

  void erase(iterator first, iterator last) {
    using rep_iterator = typename rep_type::iterator;
    t.erase(reinterpret_cast<rep_iterator &>(first),
            reinterpret_cast<rep_iterator &>(last));
  }

  void clear() { t.clear(); }

 public:// find && search
  iterator find(const key_type &x) const noexcept { return t.find(x); }
  size_type count(const key_type &x) const noexcept { return t.count(x); }
  iterator lower_bound(const key_type &x) const noexcept {
    return t.lower_bound(x);
  }
  iterator upper_bound(const key_type &x) const noexcept {
    return t.upper_bound(x);
  }
  pair<iterator, iterator> equal_range(const key_type &x) const noexcept {
    return t.equal_range(x);
  }
};

template<class Key, class Compare, class Alloc>
inline bool operator==(const multiset<Key, Compare, Alloc> &lhs,
                       const multiset<Key, Compare, Alloc> &rhs) {
  return lhs.t == rhs.t;
}

template<class Key, class Compare, class Alloc>
inline bool operator!=(const multiset<Key, Compare, Alloc> &lhs,
                       const multiset<Key, Compare, Alloc> &rhs) {
  return !(lhs.t == rhs.t);
}

template<class Key, class Compare, class Alloc>
inline bool operator<(const multiset<Key, Compare, Alloc> &lhs,
                      const multiset<Key, Compare, Alloc> &rhs) {
  return lhs.t < rhs.t;
}

template<class Key, class Compare, class Alloc>
inline bool operator>(const multiset<Key, Compare, Alloc> &lhs,
                      const multiset<Key, Compare, Alloc> &rhs) {
  return rhs < lhs;
}

template<class Key, class Compare, class Alloc>
inline bool operator<=(const multiset<Key, Compare, Alloc> &lhs,
                       const multiset<Key, Compare, Alloc> &rhs) {
  return !(rhs < lhs);
}

template<class Key, class Compare, class Alloc>
inline bool operator>=(const multiset<Key, Compare, Alloc> &lhs,
                       const multiset<Key, Compare, Alloc> &rhs) {
  return !(lhs < rhs);
}

template<class Key, class Compare, class Alloc>
inline void swap(multiset<Key, Compare, Alloc> &lhs,
                 multiset<Key, Compare, Alloc> &rhs) noexcept {
  lhs.swap(rhs);
}

}// namespace MiniSTL
