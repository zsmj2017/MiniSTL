#pragma once

#include "slist_iterator.h"

template <class T, class Alloc = alloc>
class slist {
public:
	using value_type = T;
	using pointer = T* ;
	using const_pointer = const value_type*;
	using reference = value_type& ;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

	using iterator = __slist_iterator<T, T&, T*>;
	using const_iterator = __slist_iterator<T, const T&, const T*>;

private:
	using list_node = __slist_node<T>;
	using list_node_base = __slist_node_base;
	using iterator_base = __slist_iterator_base;
	using list_node_allocator = simple_alloc<list, node, Alloc>;

	//配置与释放node
	static list_node* create_node(const value_type& value) {
		list_node* node = list_node_allocator::allocate();
		try{
			construct(&node->data, x);
			node->next = 0;
		}
		catch{
			list_node_allocator::deallocate(node);
		}			
		return node;
	}

	static void destroy_node(list_node* node) {
		destroy(&node->data);
		list_node_allocator::deallocate(node);
	}

private:
	list_node_base head;//单链表头部,此并非指针，而是一个实体

public:
	slist() { head.next = nullptr; }
	~slist() { clear(); }

public:
	iterator begin() { return iterator(static_cast<listnode*>(head.next); }
	iterator end() { return iterator(nullptr); }
	size_type size() const { return __slist_size(head.next); }
	bool empty() const { return head.next == nullptr; }

	void swap(slist& rhs) {
		//交换单链表只需要交换彼此的第一个节点即可
		list_node_base* temp = head.next;
		head.next = rhs.head.next;
		rhs.head.next = temp;
	}

public:
	reference front() { return static_cast<list_node*>(head.next)->data; }

	void push_front(const value_type& value) {
		__slist_make_link(&head, create_node(value));
	}

	void pop_front() {
		list_node* node = static_cast<list_node*>(head.next);
		head.next = node->next;
		destory_node(node);
	}
};