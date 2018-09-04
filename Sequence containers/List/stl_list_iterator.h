#pragma once

#include "stl_list_node.h"
#include "stl_iterator.h"
#include <cstddef>

namespace MiniSTL {

template<class T, class Ref, class Ptr>
struct __list_iterator {
	using iterator = __list_iterator<T, T&, T*>;
	using self = __list_iterator<T, Ref, Ptr>;

	using iterator_category = bidirectional_iterator_tag;
	using value_type = T;
	using pointer = Ptr;
	using reference = Ref;
	using link_type = __list_node<T>*;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	link_type node;//迭代器内部存在一个raw pointer指向__list_node

	//constructor
	__list_iterator(link_type x) : node(x) {}
	__list_iterator() {}
	__list_iterator(const iterator& x) : node(x.node) {}

	//__list_iterator析构函数为trivial

	bool operator==(const self&rhs) const { return node == rhs.node; }
	bool operator!=(const self&rhs) const { return node != rhs.node; }

	//dererence
	reference operator*() const { return node->data; }

	//member access
	pointer operator->() const { return &(operator*()); }

	//自增
	self& operator++() {
		node = static_cast<link_type>(node->next);//将__link_node*强制转为迭代器
		return *this;
	}

	self operator++(int i) {
		self temp = *this;
		++(*this);
		return temp;
	}

	//自减
	self& operator--() {
		node = static_cast<link_type>(node->prev);//将__link_node*强制转为迭代器
		return *this;
	}

	self operator--(int i) {
		self temp = *this;
		--(*this);
		return temp;
	}
};

}// end namespace::MiniSTL