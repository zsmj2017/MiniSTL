#pragma once

#include "stl_list_node.h"
#include "stl_iterator.h"
#include <cstddef> // ptrdiff_t

namespace MiniSTL {

template<class T, class Ref, class Ptr>
struct __list_iterator {
	// alias declarations
	using iterator = __list_iterator<T, T&, T*>;
	using self = __list_iterator<T, Ref, Ptr>;

	using iterator_category = bidirectional_iterator_tag;
	using value_type = T;
	using pointer = Ptr;
	using reference = Ref;
	using link_type = __list_node<T>*;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	// data member
	link_type node;// raw pointer link to list_node

	// ctor
	__list_iterator(link_type x) : node(x) {}
	__list_iterator() {}
	__list_iterator(const iterator& x) : node(x.node) {}

	// dtor(trivial)

	bool operator==(const self& rhs) const noexcept { return node == rhs.node; }
	bool operator!=(const self& rhs) const noexcept { return node != rhs.node; }

	// dererence
	reference operator*() const { return node->data; }

	// member access
	pointer operator->() const { return &(operator*()); }

	// increasement
	self& operator++() {
		node = reinterpret_cast<link_type>(node->next); // convert void* into __list_node<T>*
		return *this;
	}

	self operator++(int i) {
		self temp = *this;
		++(*this);
		return temp;
	}

	// decreasement
	self& operator--() {
		node = reinterpret_cast<link_type>(node->prev);
		return *this;
	}

	self operator--(int i) {
		self temp = *this;
		--(*this);
		return temp;
	}
};

}// end namespace::MiniSTL