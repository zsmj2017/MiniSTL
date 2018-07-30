#pragma once

#include <functional>

//Forward declarations of operators == and <, needed for friend declarations.
template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs);

template <class Key, class Compare = less<Key>, class Alloc = alloc>
class set {
public:
	//Set的key与value是同一个
	using key_type = Key;
	using value_type = Key;
	using key_compare = Compare;
	using value_compare = Compare;

private:
	template <class T>
	struct identity :public unary_function<T,T>{//一元函数对象，看看后期能不能用function对象替换
		const T& operator()(const T& x) const { return x; }
	};

	using rep_type = rb_tree < key_type, value_type, identity<value_type>, Alloc>;

	rep_type t;//底层红黑树

public:
	//set禁止修改键值，因此其迭代器与指针均为const iterator或const_ptr，引用亦为const
	//set迭代器无法执行任何写入操作
	using pointer = typename rep_type::const_pointer;
	using const_pointer = typename rep_type::const_pointer;
	using reference = typename rep_type::const_reference;
	using const_reference = typename rep_type const_reference;
	using iterator = typename rep_type::const_iterator;
	using const_iterator = typename rep_type::const_iterator;
	using reverse_iterator = typename rep_type::const_reverse_iterator;
	using const_reverse_iterator = typename rep_type::const_reverse_iterator;

	using size_type = typename rep_type::size_type;
	using difference_type = typename rep_type::difference_type;
	using allocator_type = typename rep_type::allocator_type;

	//构造与拷贝

	// set只能使用RB-tree的insert-unique(),不能使用insert-equal() 
	set():t(key_compare()) {}
	explicit set (const key_compare& comp):t(comp) {}

	template <class InputIterator>
	set (InputIterator first, InputIterator last)
		: t(key_compare()) {
		t.insert_unique(first, last);
	}

	template <class InputIterator>
	set(InputIterator first, InputIterator last,const key_compare& comp)
		: t(comp) {
		t.insert_unique(first, last);
	}

	set(const set<Key, Compare, Alloc>& x) : t(x.t) {}

	set<Key, Compare, Alloc>& operator=(const set<Key, Compare, Alloc> x){
		t = x.t;//调用rb-tree的operator=
		return *this;
	}

	//accessor

	key_compare key_comp() const { return t.key_comp(); }
	value_compare value_comp() const { return t.key_comp(); }//set的value_comp即为rb-tree的key_comp
	allocator_type get_allocator() const { return t.get_allocator(); }

	//基本迭代器所指位置
	iterator begin() const { return _t.begin(); }
	iterator end() const { return t.end(); }
	reverse_iterator rbegin() const { return t.rbegin(); }
	reverse_iterator rend() const { return t.rend(); }

	//属性获取
	bool empty() const { return t.empty(); }
	size_type size() const { return t.size(); }
	size_type max_size() const { return t.max_size(); }
	void swap(set<Key, Compare, Alloc>& x) { t.swap(x.t); }//此处调用的是rb-tree成员函数，而非stl swap


	//insert && erase

	std::pair<iterator, bool> insert(const value_type& x) {
		pair<typename rep_type::iterator, bool> p = t.insert_unique(x);
		return pair<iterator, bool>(p.first, p.second);
	}
	
	iterator insert(iterator pos, const value_type& x) {
		using rep_iterator = rep_type::iterator;
		return t.insert_unique(static_cast<rep_iterator&>(pos),x);
	}
	template <class InputIterator>
	void insert(InputIterator first, InputIterator last) {
		t.insert_unique(first, last);
	}

	void erase(iterator pos) {
		using rep_iterator = rep_type::iterator;
		t.erase(static_cast<rep_iterator&>(pos))
	}

	size_type erase(const key_type& x) {
		return t.erase(x);
	}

	void erase(iterator first, iterator last) {
		using rep_iterator = rep_type::iterator;
		t.erase(static_cast<rep_iterator&>(first), static_cast<rep_iterator&>(last));
	}

	void clear() { t.clear(); }

	//find

	iterator find(const key_type& x) const { return t.find(x); }

	size_type count(const key_type& x) const {
		return t.find(x) == t.end() ? 0 : 1;
	}

	//返回不小于当前元素的第一个可插入的位置 
	iterator lower_bound(const key_type& x) const {
		return t.lower_bound(x);
	}

	// 返回大于当前元素的第一个可插入的位置 
	iterator upper_bound(const key_type& x) const {
		return t.upper_bound(x);
	}

	pair<iterator, iterator> equal_range(const key_type& x) const {
		return t.equal_range(x);
	}

	//operator

	friend bool operator== <>(const set&, const set&);
	friend bool operator<  <>(const set&, const set&);
};

template <class Key, class Compare, class Alloc>
inline bool operator==(const set<Key, Compare, Alloc>& lhs,const set<Key, Compare, Alloc>& rhs) {
	return lhs.t == rhs.t;
}


template <class Key, class Compare, class Alloc>
inline bool operator<(const set<Key, Compare, Alloc>& lhs, const set<Key, Compare, Alloc>& rhs) {
	return lhs.t < rhs.t;
}
