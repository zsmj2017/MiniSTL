#pragma once

#include "rb_tree.h"

namespace MiniSTL {
// Forward declarations of operators == and <, needed for friend declarations.
template <class Key, class Compare, class Alloc>
class set;

template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs);

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs);

template <class Key, class Compare = less<Key>, class Alloc = simpleAlloc<Key> >
class set {
  // friend declarations
  template <class _Key, class _Compare, class _Alloc>
  friend bool operator==(const set<_Key, _Compare, _Alloc>& lhs,
                         const set<_Key, _Compare, _Alloc>& rhs);
  template <class _Key, class _Compare, class _Alloc>
  friend bool operator<(const set<_Key, _Compare, _Alloc>& lhs,
                        const set<_Key, _Compare, _Alloc>& rhs);

 public:  // key_type is value_type
  using key_type = Key;
  using value_type = Key;
  using key_compare = Compare;
  using value_compare = Compare;

 private:  // data member
  using rep_type =
      rb_tree<key_type, value_type, identity<value_type>, Compare, Alloc>;
  rep_type t;

 public:
  // set禁止修改键值，因此其迭代器与指针均为const
  using pointer = typename rep_type::const_pointer;
  using const_pointer = typename rep_type::const_pointer;
  using reference = typename rep_type::const_reference;
  using const_reference = typename rep_type::const_reference;
  using iterator = typename rep_type::const_iterator;
  using const_iterator = typename rep_type::const_iterator;
  using reverse_iterator = typename rep_type::const_reverse_iterator;
  using const_reverse_iterator = typename rep_type::const_reverse_iterator;
  using size_type = typename rep_type::size_type;
  using difference_type = typename rep_type::difference_type;

 public:  // ctor
  // use insert_unique
  set() : t(key_compare()) {}
  explicit set(const key_compare& comp) : t(comp) {}
  template <class InputIterator>
  set(InputIterator first, InputIterator last,
      const key_compare& comp = Compare())
      : t(comp) {
    t.insert_unique(first, last);
  }
  set(std::initializer_list<value_type> ils,
      const key_compare& comp = Compare())
      : t(comp) {
    t.insert_unique(ils.begin(), ils.end());
  }

 public:  // copy operations
  set(const set& rhs) : t(rhs.t) {}
  set& operator=(const set& rhs) {
    t = rhs.t;
    return *this;
  }

 public:  // move operation
  set(set&& rhs) noexcept : t(std::move(rhs.t)) {}
  set& operator=(set&& rhs) noexcept {
    t = std::move(rhs.t);
    return *this;
  }

 public:  // getter
  key_compare key_comp() const noexcept { return t.key_comp(); }
  value_compare value_comp() const noexcept { return t.key_comp(); }
  bool empty() const noexcept { return t.empty(); }
  size_type size() const noexcept { return t.size(); }
  // read only
  iterator begin() const noexcept { return t.cbegin(); }
  iterator end() const noexcept { return t.cend(); }
  const_iterator cbegin() const noexcept { return t.cbegin(); }
  const_iterator cend() const noexcept { return t.cend(); }
  reverse_iterator rbegin() const noexcept { return t.rbegin(); }
  reverse_iterator rend() const noexcept { return t.rend(); }

 public:  // swap
  void swap(set<Key, Compare, Alloc>& rhs) noexcept { t.swap(rhs.t); }

 public:  // insert
  pair<iterator, bool> insert(const value_type& val) {
    pair<typename rep_type::iterator, bool> p = t.insert_unique(val);
    return pair<iterator, bool>(p.first, p.second);
  }
  // hint:iterator to the position before which the new element will be inserted
  iterator insert(const_iterator hint, const value_type& val) {
    using rep_iterator = typename rep_type::iterator;
    return t.insert_unique(reinterpret_cast<rep_iterator&>(hint), val);
  }
  template <class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    t.insert_unique(first, last);
  }
  void insert(std::initializer_list<value_type> ils) {
    insert(ils.begin(), ils.end());
  }

 public:  // erase
  void erase(iterator pos) {
    using rep_iterator = typename rep_type::iterator;
    t.erase(reinterpret_cast<rep_iterator>(pos));
  }
  size_type erase(const key_type& val) { return t.erase(val); }
  void erase(iterator first, iterator last) {
    using rep_iterator = typename rep_type::iterator;
    t.erase(reinterpret_cast<rep_iterator>(first),
            reinterpret_cast<rep_iterator>(last));
  }
  void clear() noexcept { t.clear(); }

 public:  // find
  iterator find(const key_type& key) noexcept { return t.find(key); }
  const_iterator find(const key_type& key) const noexcept {
    return t.find(key);
  }
  size_type count(const key_type& key) const noexcept {
    return t.find(key) == t.end() ? 0 : 1;
  }
  iterator lower_bound(const key_type& key) noexcept {
    return t.lower_bound(key);
  }
  const_iterator lower_bound(const key_type& key) const noexcept {
    return t.lower_bound(key);
  }
  iterator upper_bound(const key_type& key) noexcept {
    return t.upper_bound(key);
  }
  const_iterator upper_bound(const key_type& key) const noexcept {
    return t.upper_bound(key);
  }
  pair<iterator, iterator> equal_range(const key_type& key) {
    return t.equal_range(key);
  }
  pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
    return t.equal_range(key);
  }
};

template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return lhs.t == rhs.t;
}

template <class Key, class Compare, class Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return !(lhs.t == rhs.t);
}

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs) {
  return lhs.t < rhs.t;
}

template <class Key, class Compare, class Alloc>
inline bool operator>(const set<Key, Compare, Alloc>& lhs,
                      const set<Key, Compare, Alloc>& rhs) {
  return rhs < lhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator<=(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return !(rhs < lhs);
}

template <class Key, class Compare, class Alloc>
inline bool operator>=(const set<Key, Compare, Alloc>& lhs,
                       const set<Key, Compare, Alloc>& rhs) {
  return !(lhs < rhs);
}

template <class Key, class Compare, class Alloc>
inline void swap(const set<Key, Compare, Alloc>& lhs,
                 const set<Key, Compare, Alloc>& rhs) {
  lhs.swap(rhs);
}

}  // namespace MiniSTL
