#pragma once

#include <functional>

//Forward declarations of operators == and <, needed for friend declarations.
template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class multimap;

template <class Key, class Compare, class Alloc>
inline bool operator==(const multimap<Key, Compare, Alloc>& lhs, const multimap<Key, Compare, Alloc>& rhs);

template <class Key, class Compare, class Alloc>
inline bool operator<(const multimap<Key, Compare, Alloc>& lhs, const multimap<Key, Compare, Alloc>& rhs);

template <class Key, class T, class Compare = less<Key>, class Alloc = alloc>
class multimap {
public:
	using key_type = Key;
	using data_type = T;
	using value_type = std::pair<const Key, T>;
	using key_compare = Compare;

	class value_compare : public std::binary_function<value_type, value_type, bool> {
		friend class multimap<Key, T, Compare, Alloc>;
	protected:
		Compare comp;
		value_compare(Compare c) : comp(c) {}
	public:
		bool operator()(const value_type& x, const value_type& y) const 
			return comp(x.first, y.first);	
	};

private:
	//select1st是SGI扩充的一个仿函数模板类，返回pair的第一个元素
	using rep_type = rb_tree<key_type, value_type, std::select1st<value_type>, key_compare, _Alloc>;
	rep_type t;

public:
	using pointer = typename rep_type::pointer;
	using const_pointer = typename rep_type::const_pointer;
	using reference = typename rep_type::reference;
	using const_reference = typename rep_type:: const_reference;
	using iterator = typename rep_type::iterator;
	using const_iterator = typename rep_type::const_iterator;
	using reverse_iterator = typename rep_type::reverse_iterator;
	using const_reverse_iterator = typename rep_type::const_reverse_iterator;


	using size_type = typename rep_type::size_type;
	using difference_type = typename rep_type::difference_type;
	using allocator_type = typename rep_type::allocator_type;


	multimap() :t(key_compare()) {}
	explicit multimap(const key_compare& comp) :t(comp) {}
	template <class InputIterator>
	multimap(InputIterator first, InputIterator last) : t(key_compare()) { t.insert_equal(first, last); }
	template <class InputIterator>
	multimap(InputIterator first, InputIterator last, const key_compare()& comp) : t(comp) { t.insert_equal(first, last); }
	multimap(const multimap<Key, T, Compare, Alloc>&rhs) :t(x.t) {}
	multimap<Key, T, Compare, Alloc>& operator=(const multimap<Key, T, Compare, Alloc>&rhs) {
		t = x.t;
		return *this;
	}

	//accessors

	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return value_compare(t.key_comp()); }
	iterator begin() { return t.begin(); }
	const_iterator begin() const { return t.begin(); }
	iterator end() { return t.end(); }
	const_iterator end() const { return t.end(); }
	reverse_iterator rbegin() { return t.rbegin(); }
	const_reverse_iterator rbegin() const { return t.rbegin(); }
	reverse_iterator rend() { return t.rend(); }
	const_reverse_iterator rend() const { return t.rend(); }
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	//multimapµÄoperator[]¾ß±¸²åÈë¹¦ÄÜ
	value_type& operator[](const key_type& k) {
		return (*(insert(value_type(k, data_type())).first)).second
	}


	void swap(multimap<Key, T, Compare, Alloc>& x) { t.swap(x.t); }

	// insert/erase

	std::pair<iterator, bool> insert(const value_type& x) {
		return t.insert_equal(x);
	}
	iterator insert(iterator position, const value_type& x) {
		return t.insert_equal(position, x);
	}
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_equal(first, last);
	}
	void erase(iterator position) { t.erase(position); }
	size_type erase(const key_type& x) { return t.erase(x); }
	void erase(iterator first, iterator last) {
		t.erase(first, last);
	}
	void clear() { t.clear(); }

	//multimap operations

	iterator find(const key_type& x) { return t.find(x); }
	const_iterator find(const key_type& x) const { return t.find(x); }
	size_type count(const key_type& x) const {
		return t.find(x) == t.end() ? 0 : 1;
	}
	iterator lower_bound(const key_type& x) { return t.lower_bound(x); }
	const_iterator lower_bound(const key_type& x) const {
		return t.lower_bound(x);
	}
	iterator upper_bound(const key_type& x) { return t.upper_bound(x); }
	const_iterator upper_bound(const key_type& x) const {
		return t.upper_bound(x);
	}
	std::pair<iterator, iterator> equal_range(const key_type& x) {
		return t.equal_range(x);
	}
	std::pair<const_iterator, const_iterator> equal_range(const key_type& x) const {
		return t.equal_range(x);
	}


	friend bool operator== <>(const multimap&, const multimap&);
	friend bool operator< <>(const multimap&, const multimap&);
};

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator==(const multimap<Key, Tp, Compare, Alloc>& lhs, const multimap<Key, Tp, Compare, Alloc>& rhs) {
	return lhs.t == rhs.t;
}

template <class Key, class Tp, class Compare, class Alloc>
inline bool operator<(const multimap<Key, Tp, Compare, Alloc>& lhs, const multimap<Key, Tp, Compare, Alloc>& rhs) {
	return lhs.t < rhs.t;
}
