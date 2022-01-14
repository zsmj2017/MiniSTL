#pragma once

#include "hashtable.h"

namespace MiniSTL {

template<class Value, class HashFcn, class EqualKey, class Alloc>
class hash_set;

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc> &,
                       const hash_set<Value, HashFcn, EqualKey, Alloc> &);

template<class Value, class HashFcn = hash<Value>,
         class EqualKey = equal_to<Value>, class Alloc = simpleAlloc<Value>>
class hash_set {
  // friend declarations
  template<class _Value, class _HashFcn, class _EqualKey, class _Alloc>
  friend bool operator==(
      const hash_set<_Value, _HashFcn, _EqualKey, _Alloc> &,
      const hash_set<_Value, _HashFcn, _EqualKey, _Alloc> &);

 private:// data member
  using ht =
      hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc>;
  ht rep;

 public:// alias declarations
  using key_type = typename ht::key_type;
  using value_type = typename ht::value_type;
  using hasher = typename ht::hasher;
  using key_equal = typename ht::key_equal;

  using iterator = typename ht::const_iterator;
  using const_iterator = typename ht::const_iterator;

  using size_type = typename ht::size_type;

 public:// getter
  hasher hash_funct() const noexcept { return rep.hash_funct(); }
  key_equal key_eq() const noexcept { return rep.key_eq(); }
  size_type bucket_count() const noexcept { return rep.bucket_count(); }
  size_type max_bucket_count() const noexcept {
    return rep.max_bucket_count();
  }
  size_type elems_in_bucket(size_type n) const noexcept {
    return rep.elems_in_bucket(n);
  }
  size_type size() const noexcept { return rep.size(); }
  size_type max_size() const noexcept { return rep.max_size(); }
  bool empty() const noexcept { return rep.empty(); }
  iterator begin() const noexcept { return rep.cbegin(); }
  iterator end() const noexcept { return rep.cend(); }
  const_iterator cbegin() const noexcept { return rep.cbegin(); }
  const_iterator cend() const noexcept { return rep.cend(); }

 public:// setter
  void resize(size_type n) { rep.resize(n); }

 public:// ctor
  hash_set() : rep(100, hasher(), key_equal()) {}
  explicit hash_set(size_type n) : rep(n, hasher(), key_equal()) {}
  hash_set(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
  template<class InputIterator>
  hash_set(InputIterator first, InputIterator last)
      : rep(100, hasher(), key_equal()) {
    rep.insert_unique(first, last);
  }
  template<class InputIterator>
  hash_set(InputIterator first, InputIterator last, size_type n)
      : rep(n, hasher(), key_equal()) {
    rep.insert_unique(first, last);
  }
  template<class InputIterator>
  hash_set(InputIterator first, InputIterator last, size_type n,
           const hasher &hf)
      : rep(n, hf, key_equal()) {
    rep.insert_unique(first, last);
  }

 public:// swap
  void swap(hash_set &rhs) noexcept { rep.swap(rhs.rep); }

 public:// insert
  pair<iterator, bool> insert(const value_type &obj) {
    pair<typename ht::iterator, bool> p = rep.insert_unique(obj);
    return pair<iterator, bool>(p.first, p.second);
  }
  template<class InputIterator>
  void insert(InputIterator first, InputIterator last) {
    rep.insert_unique(first, last);
  }

 public:// erase
  size_type erase(const key_type &key) { return rep.erase(key); }
  void erase(iterator it) { rep.erase(it); }
  void erase(iterator first, iterator last) { rep.erase(first, last); }
  void clear() { rep.clear(); }

 public:// find
  iterator find(const key_type &key) const { return rep.find(key); }
  size_type count(const key_type &key) const { return rep.count(key); }
  pair<iterator, iterator> equal_range(const key_type &key) const {
    return rep.equal_range(key);
  }
};

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_set<Value, HashFcn, EqualKey, Alloc> &lhs,
                       const hash_set<Value, HashFcn, EqualKey, Alloc> &rhs) {
  return lhs.rep == rhs.rep;
}

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator!=(const hash_set<Value, HashFcn, EqualKey, Alloc> &lhs,
                       const hash_set<Value, HashFcn, EqualKey, Alloc> &rhs) {
  return !(lhs == rhs);
}

template<class Value, class HashFcn, class EqualKey, class Alloc>
inline void swap(const hash_set<Value, HashFcn, EqualKey, Alloc> &lhs,
                 const hash_set<Value, HashFcn, EqualKey, Alloc> &rhs) {
  lhs.swap(rhs);
}

}// namespace MiniSTL
