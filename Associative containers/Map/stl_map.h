#pragma once

#include "rb_tree.h"
#include "stl_function.h"

namespace MiniSTL{

//Forward declarations of operators == and <, needed for friend declarations.

template <class Key, class T, class Compare, class Alloc>
class map;

template <class Key, class T, class Compare, class Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs);

template <class Key, class T, class Compare, class Alloc>
inline bool operator==(const map<Key, T, Compare, Alloc>& lhs, const map<Key, T, Compare, Alloc>& rhs);

template <class Key, class T,class Compare = std::less<Key>,class Alloc = simpleAlloc<T> >
class map{
	// friend declarations
	template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator==(const map<_Key, _T, _Compare, _Alloc>& lhs, const map<_Key, _T, _Compare, _Alloc>& rhs);
	template <class _Key, class _T, class _Compare, class _Alloc>
	friend bool operator< (const map<_Key, _T, _Compare, _Alloc>& lhs, const map<_Key, _T, _Compare, _Alloc>& rhs);
public:// value comparator
	using key_type = Key;
	using data_type = T;
	using value_type = std::pair<const Key,T>;
	using key_compare = Compare;

	class value_compare : public binary_function<value_type, value_type, bool> {
		friend class map<Key, T, Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c) : comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y) const {
			return comp(x.first, y.first);
		}
	};

private:// data member
	//select1st为SGI扩充。是一个函数对象模板类，用以返回pair的第一个元素的引用
	using rep_type = rb_tree<key_type, value_type, select1st<value_type>, key_compare, Alloc>;
	rep_type t;  // 底层红黑树

public:// Alias declarations
	using pointer = typename rep_type::pointer;
	using const_pointer = typename rep_type::const_pointer;
	using reference = typename rep_type::reference;
	using const_reference = typename rep_type:: const_reference;
	using iterator = typename rep_type::iterator;
	using const_iterator = typename rep_type::const_iterator;
	// TODO::reverse
	//using reverse_iterator = typename rep_type::reverse_iterator;
	//using const_reverse_iterator = typename rep_type::const_reverse_iterator;
	using size_type = typename rep_type::size_type;
	using difference_type = typename rep_type::difference_type;

public:// ctor
	map():t(key_compare()) {}
	explicit map(const key_compare& comp):t(comp) {}
	template <class InputIterator>
	map(InputIterator first, InputIterator last) : t(key_compare()) { t.insert_unique(first, last); }
	template <class InputIterator>
	map(InputIterator first, InputIterator last,const key_compare& comp) : t(comp) { t.insert_unique(first, last); }
	map(const map<Key,T,Compare,Alloc>& rhs):t(rhs.t){}

public:// copy operation
	map& operator=(const map& rhs) {
		t = rhs.t;
		return *this;
	}
	
public:// getter
	key_compare key_comp() const noexcept { return t.key_comp(); }
	value_compare value_comp() const noexcept { return value_compare(t.key_comp()); }
	const_iterator cbegin() const noexcept{ return t.begin(); }
	const_iterator cend() const noexcept { return t.end(); }
	// TODO::reverse
	//const_reverse_iterator crbegin() const noexcept { return t.rbegin(); }
	//const_reverse_iterator crend() const noexcept { return t.rend(); }
	bool empty() const noexcept { return t.empty(); }
	size_type size() const noexcept { return t.size(); }
	size_type max_size() const noexcept { return t.max_size(); }

public:// setter
	iterator begin() noexcept { return t.begin(); }
	iterator end() noexcept { return t.end(); }
	// TODO::reverse
	//reverse_iterator rbegin() noexcept { return t.rbegin(); }
	//reverse_iterator rend() noexcept { return t.rend(); }
	//map的operator[]具备插入功能
	value_type& operator[](const key_type& k) {
		return (*(insert(value_type(k,data_type())).first)).second;
	}

public:// swap
	//调用rb-tree接口
	void swap(map<Key, T, Compare, Alloc>& x) noexcept { t.swap(x.t); }

public:// insert && erase
	std::pair<iterator, bool> insert(const value_type& x){
		return t.insert_unique(x);
	}
	iterator insert(iterator pos, const value_type& x){
		return t.insert_unique(pos, x);
	}
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}
	void erase(iterator pos) { t.erase(pos); }
	size_type erase(const key_type& x) { return t.erase(x); }
	void erase(iterator first, iterator last){
		t.erase(first, last);
	}
	void clear() { t.clear(); }

public:// find
	iterator find(const key_type& x) noexcept { return t.find(x); }
	const_iterator find(const key_type& x) const noexcept { return t.find(x); }
	size_type count(const key_type& x) const noexcept {
		return t.find(x) == t.end() ? 0 : 1;
	}
	iterator lower_bound(const key_type& x) noexcept { return t.lower_bound(x); }
	const_iterator lower_bound(const key_type& x) const noexcept {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x) noexcept { return t.upper_bound(x); }
	const_iterator upper_bound(const key_type& x) const noexcept {
		return t.upper_bound(x);
	}
	std::pair<iterator, iterator> equal_range(const key_type& x) noexcept {
		return t.equal_range(x);
	}
	std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const noexcept {
		return t.equal_range(x);
	}
};

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator==(const map<Key, Tp, Compare, Alloc>& lhs, const map<Key, Tp, Compare, Alloc>& rhs) {
	return lhs.t == rhs.t;
}

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator!=(const map<Key, Tp, Compare, Alloc>& lhs, const map<Key, Tp, Compare, Alloc>& rhs) {
	return !(lhs.t == rhs.t);
}

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator<(const map<Key, Tp, Compare, Alloc>& lhs,const map<Key, Tp, Compare, Alloc>& rhs) {
	return lhs.t < rhs.t;
}

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator>=(const map<Key, Tp, Compare, Alloc>& lhs,const map<Key, Tp, Compare, Alloc>& rhs) {
	return !(lhs.t < rhs.t);
}

}// end namespace::MiniSTL