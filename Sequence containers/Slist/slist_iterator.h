#pragma once

#include <cstddef>
#include "slist_node.h"
#include "stl_iterator.h"

namespace MiniSTL {

struct slist_iterator_base {
	// alias decalartion
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator_category = forward_iterator_tag;

	// data
	slist_node_base* node;

	// ctor
	slist_iterator_base(slist_node_base* n) :node(n) {}

	void incur() { node = node->next; }

	bool operator ==(const slist_iterator_base& rhs) { return node == rhs.node;}

	bool operator !=(const slist_iterator_base& rhs) { return node != rhs.node;}
};

template<class T, class Ref, class Ptr>
struct slist_iterator :public slist_iterator_base {
	using iterator = slist_iterator<T, T&, T*>;
	using const_iterator = slist_iterator<T, const T&, const T*>;
	using self = slist_iterator<T, Ref, Ptr>;

	using value_type = T;
	using pointer = Ptr;
	using reference = Ref;
	using list_node = slist_node<T>;

	// ctor
	slist_iterator(list_node* node) : slist_iterator_base(node) {}//调用slit.end()时将触发slist_iterator(nullptr)
	slist_iterator() : slist_iterator_base(nullptr) {}
	slist_iterator(const iterator& rhs) : slist_iterator_base(rhs.node) {}

	// dereference
	reference operator*() { return reinterpret_cast<list_node*>(node)->data; }
	pointer operator->() { return &(operator*()); }

	// incur
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

}// end namespace::MiniSTL