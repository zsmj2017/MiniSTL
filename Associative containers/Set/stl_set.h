#pragma once

#include "rb_tree.h"
#include "stl_function.h"

namespace MiniSTL{
// Forward declarations of operators == and <, needed for friend declarations.
template <class Key, class Compare, class Alloc>
class set;

template <class Key, class Compare, class Alloc>
inline bool operator== (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

template <class Key, class Compare, class Alloc>
inline bool operator< (const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

template <class Key, class Compare = less<Key>, class Alloc = simpleAlloc<Key>>
class set {
	// friend declarations
	template <class _Key, class _Compare, class _Alloc>
	friend bool operator==(const set<_Key, _Compare, _Alloc>& lhs, const set<_Key, _Compare, _Alloc>& rhs);
	template <class _Key, class _Compare, class _Alloc>
	friend bool operator<(const set<_Key, _Compare, _Alloc>& lhs, const set<_Key, _Compare, _Alloc>& rhs);
public:// Set的key与value是同一个
	using key_type = Key;
	using value_type = Key;
	using key_compare = Compare;
	using value_compare = Compare;

private:
	template <class T>
	struct identity :public unary_function<T,T>{
		const T& operator()(const T& x) const { return x; }
	};
	using rep_type = rb_tree <key_type, value_type, identity<value_type>, Compare, Alloc>;
	rep_type t;//底层红黑树

public:
	// set禁止修改键值，因此其迭代器与指针均为const iterator或const_ptr，引用亦为const
	// set迭代器无法执行任何写入操作
	using pointer = typename rep_type::const_pointer;
	using const_pointer = typename rep_type::const_pointer;
	using reference = typename rep_type::const_reference;
	using const_reference = typename rep_type:: const_reference;
	using iterator = typename rep_type::const_iterator;
	using const_iterator = typename rep_type::const_iterator;
	// TODO:
	//using reverse_iterator = typename rep_type::const_reverse_iterator;
	//using const_reverse_iterator = typename rep_type::const_reverse_iterator;

	using size_type = typename rep_type::size_type;
	using difference_type = typename rep_type::difference_type;

public:// ctor
	// set只能使用insert-unique()
	set():t(Compare()) {}
	explicit set(const Compare& comp):t(comp) {}
	template <class InputIterator>
	set (InputIterator first, InputIterator last)
		: t(Compare()) {
		t.insert_unique(first, last);
	}
	template <class InputIterator>
	set(InputIterator first, InputIterator last,const key_compare& comp)
		: t(comp) {
		t.insert_unique(first, last);
	}

public:// copy operations
	set(const set<Key, Compare, Alloc>& rhs) : t(rhs.t) {}
	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc> rhs){
		t = rhs.t;
		return *this;
	}

public:// getter
	key_compare key_comp() const noexcept { return t.key_comp(); }
	value_compare value_comp() const noexcept { return t.key_comp(); }//set的value_comp即为rb-tree的key_comp
	bool empty() const noexcept { return t.empty(); }
	size_type size() const noexcept { return t.size(); }
	size_type max_size() const noexcept { return t.max_size(); }
	// set的迭代器具有只读属性
	iterator begin() const noexcept{ return t.cbegin(); }
	iterator end() const noexcept { return t.cend(); }
	iterator cbegin() const noexcept{ return t.cbegin(); }
	iterator cend() const noexcept { return t.cend(); }
	// TODO:
	//reverse_iterator rbegin() const noexcept { return t.rbegin(); }
	//reverse_iterator rend() const noexcept { return t.rend(); }

public:// swap
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }//此处调用的是rb-tree成员函数，而非stl swap

public:// inset && erase
	pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
		return pair<iterator, bool>(p.first, p.second);
	}

	iterator insert(iterator pos, const value_type& x) {
		using rep_iterator = typename rep_type::iterator;
		return t.insert_unique(reinterpret_cast<rep_iterator&>(pos),x);
	}

	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	void erase(iterator pos) {
		using rep_iterator = typename rep_type::iterator;
		t.erase(reinterpret_cast<rep_iterator&>(pos));
	}

	size_type erase(const key_type& x) {
		return t.erase(x);
	}

	void erase(iterator first, iterator last) {
		using rep_iterator = typename rep_type::iterator;
		t.erase(reinterpret_cast<rep_iterator&>(first), reinterpret_cast<rep_iterator&>(last));
	}

	void clear() { t.clear(); }

public:// find
	iterator find(const key_type& x) const noexcept { return t.find(x); }
	size_type count(const key_type& x) const noexcept { return t.find(x) == t.end() ? 0 : 1; }
	iterator lower_bound(const key_type& x) const noexcept { return t.lower_bound(x); }
	iterator upper_bound(const key_type& x) const noexcept { return t.upper_bound(x); }
	pair<iterator, iterator> equal_range(const key_type& x) const { return t.equal_range(x);}
};

template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return lhs.t == rhs.t;
}

template <class Key, class Compare, class Alloc>
inline bool operator!=(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return !(lhs.t == rhs.t);
}

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs,const set<Key, Compare, Alloc>& rhs) {
	return lhs.t < rhs.t;
}

template <class Key, class Compare, class Alloc>
inline bool operator>(const set<Key, Compare, Alloc>& lhs,const set<Key, Compare, Alloc>& rhs) {
	return rhs < lhs;
}

template <class Key, class Compare, class Alloc>
inline bool operator<=(const set<Key, Compare, Alloc>& lhs,const set<Key, Compare, Alloc>& rhs) {
	return !(rhs < lhs);
}

template <class Key, class Compare, class Alloc>
inline bool operator>=(const set<Key, Compare, Alloc>& lhs,const set<Key, Compare, Alloc>& rhs) {
	return !(lhs < rhs);
}

}// end namespace::MiniSTL
