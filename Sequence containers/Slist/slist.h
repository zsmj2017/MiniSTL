#pragma once

#include "slist_iterator.h"
#include "allocator.h"
#include "uninitialized.h"

namespace MiniSTL {

template <class T, class Alloc = simpleAlloc<T>>
class slist {
public:// alias declarations
	using value_type = T;
	using pointer = T * ;
	using const_pointer = const value_type*;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator = slist_iterator<T, T&, T*>;
	using const_iterator = slist_iterator<T, const T&, const T*>;

private:
	using list_node = slist_node<T>;
	using list_node_base = slist_node_base;
	using iterator_base = slist_iterator_base;
	using list_node_allocator = simpleAlloc<list_node>;

private:// allocate && deallocate node
	static list_node* create_node(const value_type& value) {
		list_node* node = list_node_allocator::allocate();
		try {
			construct(&node->data, value);
			node->next = 0;
		}
		catch(std::exception&){
			list_node_allocator::deallocate(node);
		}
		return node;
	}

	static void destroy_node(list_node* node) {
		destroy(&node->data);
		list_node_allocator::deallocate(node);
	}

	void clear() {
		while (head.next) {
			pop_front();
		}
	}

private:// data member
	list_node_base head;

public:// ctor && dtor
	slist() { head.next = nullptr; }
	~slist() { clear(); }

public:// setter
	iterator begin() { return iterator(static_cast<list_node*>(head.next)); }
	iterator end() { return iterator(nullptr); }
	reference front() { return static_cast<list_node*>(head.next)->data; }

public:// getter
	const_iterator cbegin() const noexcept { return iterator(static_cast<list_node*>(head.next)); }
	const_iterator cend() const noexcept { return iterator(nullptr); }
	size_type size() const noexcept { return slist_size(head.next); }
	bool empty() const noexcept { return head.next == nullptr; }
	const_reference front() const noexcept { return static_cast<list_node*>(head.next)->data; }

public:// swap
	void swap(slist& rhs) {
		list_node_base* temp = head.next;
		head.next = rhs.head.next;
		rhs.head.next = temp;
	}

public:// push && pop
	void push_front(const value_type& value) {
		slist_make_link(&head, create_node(value));
	}

	void pop_front() {
		list_node* node = static_cast<list_node*>(head.next);
		head.next = node->next;
		destroy_node(node);
	}
};

}// end namespace::MiniSTL