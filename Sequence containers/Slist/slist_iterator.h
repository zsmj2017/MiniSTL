#pragma once

#include <cstddef>
#include "slist_node.h"

struct __slist_iterator_base {
	//Alias decalartion
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator_category = forward_iterator_tag;

	//持有base_node
	__slist_node_base* node;

	//Constructor
	__slist_iterator_base(__slist_node_base* n):node(n) {}

	//前进
	void incur() { node = node->next; }

	//等值判断
	bool operator ==(const __slist_iterator_base& rhs) {
		return node == rhs.node;
	}

	bool operator !=(const __slist_iterator_base& rhs) {
		return node != rhs.node;
	}
};

template<class T, class Ref, class Ptr>
struct __slist_iterator :public __slist_iterator_base {
	using iterator = __slist_iterator<T, T&, T*>;
	using const_iterator = __slist_iterator<T, const T&, const T*>;
	using self = __slist_iterator<T, Ref, Ptr>;

	using value_type = T;
	using pointer = Ptr;
	using reference = Ref;
	using list_node = __slist_node<T>;

	//Constructor
	__slist_iterator(list_node* node) : __slist_iterator_base(node) {}//调用slit.end()时将触发__slist_iterator(nullptr)
	__slist_iterator() : __slist_iterator_base(nullptr) {}
	__slist_iterator(const iterator& rhs) : __slist_iterator_base(rhs.node) {}

	//Dereference
	reference opertaor*() { return static_cast<list_node*>(node)->data; }
	pointer operator->() { return &(operator*()); }

	//Incur
	self& operator++() {
		incur();
		return *this;
	}

	self operator++(int) {
		self temp;
		incur();
		return temp;
	}
};