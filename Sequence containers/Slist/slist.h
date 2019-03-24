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
	list_node* create_node(const value_type& value) {
		list_node* node = list_node_allocator::allocate();
		try {
			construct(&node->data, value);
			node->next = nullptr;
		}
		catch(std::exception&){
			list_node_allocator::deallocate(node);
		}
		return node;
	}

	void destroy_node(list_node* node) {
		destroy(&node->data);
		list_node_allocator::deallocate(node);
	}

private:// data member
	list_node_base head;

public:// ctor && dtor
	slist() { head.next = nullptr; }
	explicit slist(size_type n) { insert_after_fill(&head,n,value_type()); }
	slist(size_type n,const value_type& val) { insert_after_fill(&head,n,val); }
	// Here need not any dispatching tricks here, because insert_after_range already does them.
	template<class InputIterator>
	slist(InputIterator first,InputIterator last) { insert_after_range(&head,first,last); }
	~slist() { clear(); }

public:// copy operation
	slist(const slist& rhs) { insert_after_range(&head,rhs.begin(),rhs.end()); }
	slist& operator==(const slist&);

public:// setter
	iterator begin() { return iterator(reinterpret_cast<list_node*>(head.next)); }
	iterator end() { return iterator(nullptr); }
	reference front() { return reinterpret_cast<list_node*>(head.next)->data; }

public:// getter
	const_iterator cbegin() const noexcept { return iterator(reinterpret_cast<list_node*>(head.next)); }
	const_iterator cend() const noexcept { return iterator(nullptr); }
	size_type size() const noexcept { return slist_size(head.next); }
	bool empty() const noexcept { return head.next == nullptr; }
	const_reference front() const noexcept { return reinterpret_cast<list_node*>(head.next)->data; }

private:// aux interface for insert
	iterator insert_after(list_node_base* pos, const value_type& val){
		return slist_make_link(pos,create_node(val));
	}
	void insert_after_fill(list_node_base* pos, size_type n, const value_type& val){
		for(size_type i = 0; i < n; ++i)
			slist_make_link(pos,create_node(val));
	}
	template<class InputIterator>
	void insert_after_range(list_node_base* pos, InputIterator first, InputIterator last){
		insert_after_range_aux(pos,first,last,_is_integer_t<InputIterator>());
	}
	template<class Integer>
	void insert_after_range_aux(list_node_base* pos, Integer n, Integer val, _true_type){
		insert_after_fill(pos,static_cast<size_type>(n),static_cast<value_type>(val));
	}
	template<class InputIterator>
	void insert_after_range_aux(list_node_base* pos, InputIterator first, InputIterator last, _false_type){
		while(first != last) {
			pos = slist_make_link(pos,create_node(*first));
			++first;
		}
	}

public:// insert
	iterator insert(iterator pos,const value_type& val) { 
		return insert_after(__slist_previous(&head,pos.node),val);
	}
	void insert(iterator pos,size_type n, const value_type& val) {
		insert_after_fill(__slist_previous(&head,pos.node),n,val);
	}
	// Here need not any dispatching tricks here, because insert_after_range already does them.
	template<class InputIterator>
	void insert(iterator pos, InputIterator first, InputIterator last) {
		insert_after_range(slist_previous(&head,pos.node),first,last);
	}

private:// aux interface for erase
	iterator erase_after(list_node_base* pos) {
		list_node* next = reinterpret_cast<list_node*>(pos->next);
		slist_node_base* next_next = next->next;
		pos->next = next_next;
		destroy_node(next);
		return next_next;
	}
	iterator erase_after(list_node_base* before_first,list_node_base* last) {
		list_node_base* cur = before_first->next;
		while(cur != last) {
			list_node* temp = reinterpret_cast<list_node*>(cur);
			cur = cur->next;
			destroy_node(temp); 
		}
		before_first->next = last;
		return last;
	}

public:// erase
	iterator erase(iterator pos) {
		return erase_after(slist_previous(&head,pos.node));
	}
	iterator erase(iterator first,iterator last){
		return erase_after(slist_previous(&head,first.node),last.node);
	}
	void clear() { erase_after(&head,nullptr); }

private:// aux interface for assign
	void fill_assign(size_type,const value_type&);
	template<class Integer>
	void assign_dispatch(Integer n,Integer val,_true_type) {
		fill_assign(static_cast<size_type>(n),static_cast<value_type>(val));
	}
	template<class InputIterator>
	void assign_dispatch(InputIterator first,InputIterator last,_false_type);

public:// assign
	void assign(size_type n,const value_type& val) { fill_assign(n,val); }
	template<class InputIterator>
	void assign(InputIterator first,InputIterator last) { 
		assign_dispatch(first,last,_is_integer_t<InputIterator>()); 
	}

public:// swap
	void swap(slist& rhs) noexcept {
		MiniSTL::swap(head.next,rhs.head.next);
	}

public:// push && pop at front
	void push_front(const value_type& value) {
		slist_make_link(&head, create_node(value));
	}

	void pop_front() {
		list_node* node = reinterpret_cast<list_node*>(head.next);
		head.next = node->next;
		destroy_node(node);
	}
};

template<class T,class Alloc>
void slist<T,Alloc>::fill_assign(size_type n, const value_type& val) {
	list_node_base* prev = head;
	list_node* cur = reinterpret_cast<list_node*>(head.next);
	for(;cur != nullptr && n > 0;--n) {
		cur->data = val;
		prev = cur;
		cur = reinterpret_cast<list_node*>(cur->next);
	}
	if(n > 0)
		insert_after_fill(prev,n,val);
	else
		erase_after(prev,nullptr);
}

template<class T,class Alloc>
template<class InputIterator>
void slist<T,Alloc>::assign_dispatch(InputIterator first,InputIterator last,_false_type) {
	list_node_base* prev = head;
	list_node* cur = reinterpret_cast<list_node*>(head.next);
	while(first != last) {
		cur->data = *first;
		prev = cur;
		cur = reinterpret_cast<list_node*>(cur->next);
		++first;
	}
	if(first != last)
		insert_after_range(prev,first,last);
	else
		erase_after(prev,nullptr);
}

}// end namespace::MiniSTL