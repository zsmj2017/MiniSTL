#pragma once

#include "hashtable.h"

namespace MiniSTL {

    template <class Key, class Value, class HashFcn, class EqualKey, class Alloc> class hash_map;

    template <class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &,
                           const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &);

    template <class Key, class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>,
              class Alloc = simpleAlloc<Value>>
    class hash_map {
        // friend declaration
        template <class _Key, class _Value, class _HashFcn, class _EqualKey, class _Alloc>
        friend bool operator==(const hash_map<_Key, _Value, _HashFcn, _EqualKey, _Alloc> &,
                               const hash_map<_Key, _Value, _HashFcn, _EqualKey, _Alloc> &);

    private: // data member
        using ht = hashtable<pair<const Key, Value>, Key, HashFcn, select1st<pair<const Key, Value>>, EqualKey, Alloc>;
        ht rep; //底层数据结构

    public: // alias declarations
        using key_type = typename ht::key_type;
        using data_type = Value;
        using mapped_type = Value;
        using value_type = typename ht::value_type;
        using hasher = typename ht::hasher;
        using key_equal = typename ht::key_equal;

        using iterator = typename ht::iterator;
        using const_iterator = typename ht::const_iterator;

        using size_type = typename ht::size_type;

    public: // getter
        hasher hash_funct() const noexcept {
            return rep.hash_funct();
        }
        key_equal key_eq() const noexcept {
            return rep.key_eq();
        }
        size_type size() const noexcept {
            return rep.size();
        }
        size_type max_size() const noexcept {
            return rep.max_size();
        }
        bool empty() const noexcept {
            return rep.empty();
        }

        size_type bucket_count() const noexcept {
            return rep.bucket_count();
        }
        size_type max_bucket_count() const noexcept {
            return rep.max_bucket_count();
        }
        size_type elems_in_bucket(size_type n) const noexcept {
            return rep.elems_in_bucket(n);
        }

        const_iterator cbgein() const noexcept {
            return rep.cbegin();
        }
        const_iterator cend() const noexcept {
            return rep.cend();
        }

    public: // ctor
        hash_map() : rep(100, hasher(), key_equal()) {}
        explicit hash_map(size_type n) : rep(n, hasher(), key_equal()) {}
        hash_map(size_type n, const hasher &hf) : rep(n, hf, key_equal()) {}
        hash_map(size_type n, const hasher &hf, const key_equal &eql) : rep(n, hf, eql) {}
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last) : rep(100, hasher(), key_equal()) {
            rep.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n) : rep(n, hasher(), key_equal()) {
            rep.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n, const hasher &hf) : rep(n, hf, key_equal()) {
            rep.insert_unique(first, last);
        }
        template <class InputIterator>
        hash_map(InputIterator first, InputIterator last, size_type n, const hasher &hf, const key_equal &eql)
            : rep(n, hf, eql) {
            rep.insert_unique(first, last);
        }

    public: // swap
        void swap(hash_map &rhs) noexcept {
            rep.swap(rhs.rep);
        }

    public: // setter
        iterator begin() {
            return rep.begin();
        }
        iterator end() {
            return rep.end();
        }
        void resize(size_type n) {
            rep.resize(n);
        }

    public: // insert
        pair<iterator, bool> insert(const value_type &obj) {
            return rep.insert_unique(obj);
        }
        template <class InputIterator> void insert(InputIterator first, InputIterator last) {
            rep.insert_unique(first, last);
        }
        data_type &operator[](const key_type &key) {
            return rep.find_or_insert(value_type(key, data_type())).second; // 见Effective STL Item24
        }

    public: // erase
        size_type erase(const key_type &key) {
            return rep.erase(key);
        }
        void erase(iterator it) {
            rep.erase(it);
        }
        void erase(iterator f, iterator l) {
            rep.erase(f, l);
        }
        void clear() {
            rep.clear();
        }

    public: // find
        iterator find(const key_type &key) {
            return rep.find(key);
        }
        const_iterator find(const key_type &key) const {
            return rep.find(key);
        }
        size_type count(const key_type &key) const {
            return rep.count(key);
        }
        pair<iterator, iterator> equal_range(const key_type &key) {
            return rep.equal_range(key);
        }
        pair<const_iterator, const_iterator> equal_range(const key_type &key) const {
            return rep.equal_range(key);
        }
    };

    template <class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator==(const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &lhs,
                           const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &rhs) {
        return lhs.rep == rhs.rep;
    }

    template <class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    inline bool operator!=(const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &lhs,
                           const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &rhs) {
        return !(lhs == rhs);
    }

    template <class Key, class Value, class HashFcn, class EqualKey, class Alloc>
    inline void swap(const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &lhs,
                     const hash_map<Key, Value, HashFcn, EqualKey, Alloc> &rhs) {
        lhs.swap(rhs);
    }

} // namespace MiniSTL