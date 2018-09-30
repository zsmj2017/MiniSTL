#pragma once

#include <cstddef>
#include "stl_iterator.h"

namespace MiniSTL {

//缓冲区大小设定函数（在预设情况下传回可容纳的元素个数）
//若n不为0，则传回n，表示由用户自定
//若n为0则采用预设值 预设值根据sz（元素大小）而定
inline size_t __deque_buf_size(size_t sz) {
	return sz < 512 ? size_t(512 / sz) : size_t(1);
}

template<class T, class Ref, class Ptr, size_t BufSiz=0>
struct __deque_iterator {
	// alias declarartions
	using iterator = __deque_iterator<T, T&, T*, BufSiz>;
	using const_iterator = __deque_iterator<T, const T&, const T*, BufSiz>;
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using pointer = Ptr;
	using reference = Ref;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using map_pointer = T**;
	using self = __deque_iterator;


	// 保持与容器的连接
	value_type* cur;//当前缓冲区的当前元素
	value_type* first;//当前缓冲区头
	value_type* last;//当前缓冲区尾(含备用空间）
	map_pointer node;//指向管控中心

	static size_t buffer_size() { return __deque_buf_size(sizeof(value_type)); }

	// ctor
	__deque_iterator() : cur(nullptr), first(nullptr), last(nullptr), node(nullptr) {}
	__deque_iterator(pointer x, map_pointer y) : cur(x), first(*y), last(*y + buffer_size()), node(y) {}
	__deque_iterator(const iterator& rhs) : cur(rhs.cur), first(rhs.first), last(rhs.last), node(rhs.node) {}

	self& operator = (const iterator& deq_iter){
		cur = deq_iter.cur;
		first = deq_iter.first;
		last = deq_iter.last;
		node = deq_iter.node;
		return *this;
	}

	//跳转缓冲区
	void set_node(map_pointer new_node) {
		node = new_node;
		first = *new_node;
		last = first + static_cast<difference_type>(buffer_size());
	}

	// implicit conversion
	operator const_iterator() {
		const_iterator res;
		res.cur = cur;
		res.first = first;
		res.last = last;
		res.node = node;
		return res;
	}

	//解引用运算符
	reference operator*() const { return *cur; }
	pointer operator->() const { return &(operator*()); }

	difference_type operator-(const self& rhs) const{
		return buffer_size()*(node - rhs.node - 1) + (cur - first) + (rhs.last - rhs.cur);
	}

	self& operator++() {
		++cur;//单纯指针运算
		if (cur == last) {//若已抵达尾端
			set_node(++node);
			cur = first;
		}
		return *this;
	}

	self operator++(int) {
		self temp = *this;
		++*this;
		return temp;
	}

	self& operator--() {
		if (cur == first) {
			set_node(--node);
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

	//实现随机存取
	self& operator+=(difference_type n) {
		difference_type off_set = n + (cur - first);
		if (off_set >= 0 && off_set < static_cast<difference_type>(buffer_size())) {
			//不需要跳转
			cur += n;
		}
		else {
			//需要注意的是off_set小于0则必然需要跳转
			difference_type node_offset = off_set > 0 ? off_set / static_cast<difference_type>(buffer_size())
				: -static_cast<difference_type>((-off_set - 1) / buffer_size()) - 1;
			node += node_offset;
			cur = first + (off_set - static_cast<difference_type>(node_offset*(buffer_size())));
		}
		return *this;
	}

	self operator+(difference_type n) {
		self temp = *this;
		return temp += (n);
	}

	self& operator-=(difference_type n) {
		return *this += -n;
	}

	self operator-(difference_type n) {
		self temp = *this;
		return temp -= n;
	}

	reference operator[](difference_type n) {
		return *(*this += n);
	}

	bool operator==(const self&rhs) {
		return cur == rhs.cur;
	}

	bool operator!=(const self&rhs) {
		return !(*this == rhs);
	}

	bool operator<(const self& rhs) {
		return node == rhs.node ? cur < rhs.cur : node < rhs.node;
	}
};

template<class T, class Ref, class Ptr>
__deque_iterator<T, Ref, Ptr> operator+(typename __deque_iterator<T, Ref, Ptr>::distance_type n, const __deque_iterator<T, Ref, Ptr>& it){
	return it + n;
}

template<class T, class Ref, class Ptr>
__deque_iterator<T, Ref, Ptr> operator-(const __deque_iterator<T, Ref, Ptr>& it, typename __deque_iterator<T, Ref, Ptr>::distance_type n){
	return it + (-n);
}

template<class T, class Ref, class Ptr>
inline
typename __deque_iterator<T, Ref, Ptr>::difference_type operator-(const __deque_iterator<T, Ref, Ptr>& it1,const __deque_iterator<T, Ref, Ptr>& it2){
	return it1.operator-(it2);
}

template<class T, class Ref, class Ptr>
inline bool operator==(const __deque_iterator<T, Ref, Ptr>& it1, const __deque_iterator<T, Ref, Ptr>& it2) {
	return const_cast<__deque_iterator<T, Ref, Ptr>&>(it1).operator==(it2);
}

template<class T, class Ref, class Ptr>
inline bool operator!=(const __deque_iterator<T, Ref, Ptr>& it1, const __deque_iterator<T, Ref, Ptr>& it2) {
	return const_cast<__deque_iterator<T, Ref, Ptr>&>(it1).operator!=(it2);
}

}//  end namespace::MiniSTL