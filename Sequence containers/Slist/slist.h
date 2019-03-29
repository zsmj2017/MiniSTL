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
	slist(std::initializer_list<value_type> ils) { insert_after_range(&head,ils.begin(),ils.cend()); }
	// Here need not any dispatching tricks here, because insert_after_range already does them.
	template<class InputIterator>
	slist(InputIterator first,InputIterator last) { insert_after_range(&head,first,last); }

	~slist() { clear(); }

public:// copy operation
	slist(const slist& rhs) { insert_after_range(&head,rhs.begin(),rhs.end()); }
	slist& operator=(const slist&);

public:// setter
	iterator before_begin() { return iterator(reinterpret_cast<list_node*>(head)); }
	iterator begin() { return iterator(reinterpret_cast<list_node*>(head.next)); }
	iterator end() { return iterator(nullptr); }
	reference front() { return reinterpret_cast<list_node*>(head.next)->data; }
	iterator previous(const_iterator pos) {
    	return iterator(reinterpret_cast<list_node*>(slist_previous(head, pos.node)));
  	}

public:// getter
	const_iterator before_begin() const noexcept { return const_iterator(reinterpret_cast<list_node*>(head)); }
	const_iterator cbegin() const noexcept { return const_iterator(reinterpret_cast<list_node*>(head.next)); }
	const_iterator cend() const noexcept { return const_iterator(nullptr); }
	size_type size() const noexcept { return slist_size(head.next); }
	bool empty() const noexcept { return head.next == nullptr; }
	const_reference front() const noexcept { return reinterpret_cast<list_node*>(head.next)->data; } 
	const_iterator previous(const_iterator pos) const noexcept {
    	return const_iterator(reinterpret_cast<list_node*>(slist_previous(head, pos.node)));
  	}

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
	void assign(std::initializer_list<value_type> ils) { assign(ils.begin(),ils.end()); }
	slist& operator=(std::initializer_list<value_type> ils) { assign(ils); return *this; }

public:// swap
	void swap(slist& rhs) noexcept {
		MiniSTL::swap(head.next,rhs.head.next);
	}

public:// push && pop at front
	void push_front(const value_type& val) {
		slist_make_link(&head, create_node(val));
	}
	void pop_front() {
		list_node* node = reinterpret_cast<list_node*>(head.next);
		head.next = node->next;
		destroy_node(node);
	}

public:// resize
	void resize(size_type, const value_type&);
	void resize(size_type new_size) { resize(new_size,value_type()); }

public:// splice
	// insert [before_first+1,before_last+1] after pos in constant time
	void splice_after(iterator pos,iterator before_first,iterator before_last) {
		if(before_first != before_last)
			slist_splice_after(pos.node,before_first.node,before_last.node);
	}
	// insert all elements follows prev after pos in constant time
	void splice_after(iterator pos,iterator prev) {
		slist_splice_after(pos.node,pos.node,prev.node->next);
	}
	// insert all elements in rhs after pos in constant time, rhs must not be *this
	void splice_after(iterator pos,slist& rhs) {
		slist_splice_after(pos.node,&rhs.head);
	}
	// linear in distance(begin(),pos) and linerar in rhs.size()
	void splice(iterator pos,slist& rhs) {
		if(rhs.head.next)
			slist_splice_after(slist_previous(head,pos.node),&rhs.head,
				slist_previous(&rhs.head,nullptr));
	}
	// linear in distance(begin(),pos) and linerar in distance(rhs.begin(),i)
	void splice(iterator pos,slist& rhs,iterator i) {
		slist_splice_after(slist_previous(head,pos.node),
			slist_previous(&rhs.head,i.node),i.node);
	}
	// linear in distance(begin(),pos),in distance(rhs.begin(),i) and in distance(first,last)
	void splice(iterator pos,slist& rhs,iterator first,iterator last) {
		if(first != last)
			slist_splice_after(slist_previous(head,pos.node),
				slist_previous(&rhs.head,first.node),
				slist_previous(first.node,last.node));
	}

public:// remove
	void remove(const value_type&);
	template<class Predicate>
	void remove_if(Predicate);

public:// unique
	void unique();
	template<class BinaryPredicate>
	void unique(BinaryPredicate);

public:// reverse
	void reverse() { if(head.next) head.next = slist_reverse(head.next); }

public:// merge
	void merge(slist&); // merge two sorted slists and make the new slist sorted too
	template<class StrictWeakOrdering>
	void merge(slist&,StrictWeakOrdering);

public:// sort
	void sort();
	template<class StrictWeakOrdering>
	void sort(StrictWeakOrdering);
};

template<class T,class Alloc>
slist<T,Alloc>& slist<T,Alloc>::operator=(const slist& rhs) {
	if(this != &rhs) {
		list_node_base* p1 = &head;
		list_node* n1 = reinterpret_cast<list_node*>(head.next);
		const list_node* n2 = reinterpret_cast<const list_node*>(rhs.head.next);
		while(n1 && n2) {
			n1->data = n2->data;
			p1 = n1;
			n1 = reinterpret_cast<list_node*>(n1->next);
			n2 = reinterpret_cast<const list_node*>(n2->next);
		}
		if(n2 == nullptr)
			erase_after(p1, nullptr);
		else
			insert_after_range(p1, const_iterator(reinterpret_cast<list_node*>(n2)),const_iterator(nullptr));
	}
	return *this;
}

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

template<class T,class Alloc>
void slist<T,Alloc>::resize(size_type len,const value_type& val) {
	list_node_base* cur = &head;
	while(cur->next != nullptr && len > 0) {
		--len;
		cur = cur->next;
	}
	if(cur->next)
		erase_after(cur,nullptr);
	else
		insert_after_fill(cur,len,val);
}

template<class T,class Alloc>
void slist<T,Alloc>::remove(const value_type& val) {
	list_node_base* cur = head;
	while(cur && cur->next) {
		if(reinterpret_cast<list_node*>(cur->next)->data == val)
			erase_after(cur);
		else
			cur = cur->next;
	}	
}

template<class T,class Alloc>
template<class Predicate>
void slist<T,Alloc>::remove_if(Predicate pred) {
	list_node_base* cur = head;
	while(cur && cur->next) {
		if(pred(reinterpret_cast<list_node*>(cur->next)))
			erase_after(cur);
		else
			cur = cur->next;
	}	
}

template<class T,class Alloc>
void slist<T,Alloc>::unique() {
	list_node_base* cur = head.next;
	if(cur){
		while(cur->next) {
			if(reinterpret_cast<list_node*>(cur)->data == 
				reinterpret_cast<list_node*>(cur->next)->data)
				erase_after(cur);
			else
				cur = cur->next;			
		}
	}
}

template<class T,class Alloc>
template<class BinaryPredicate>
void slist<T,Alloc>::unique(BinaryPredicate pred) {
	list_node_base* cur = head.next;
	if(cur){
		while(cur->next) {
			if(pred(reinterpret_cast<list_node*>(cur)->data,
				reinterpret_cast<list_node*>(cur->next)->data))
				erase_after(cur);
			else
				cur = cur->next;			
		}
	}
}

template<class T,class Alloc>
void slist<T,Alloc>::merge(slist& rhs){
	list_node_base* n1 = head;
	while(n1->next && rhs.head.next) {
		if(reinterpret_cast<list_node*>(rhs.head.next)->data <
			reinterpret_cast<list_node*>(n1->next)->data)
			slist_splice_after(n1,&rhs.head,rhs.head.next);
		n1 = n1->next;
	}
	if(rhs.head.next) {
		n1->next = rhs.head.next;
		rhs.head.next = nullptr;
	}
}

template<class T,class Alloc>
template<class StrictWeakOrdering>
void slist<T,Alloc>::merge(slist& rhs,StrictWeakOrdering comp){
	list_node_base* n1 = head;
	while(n1->next && rhs.head.next) {
		if(comp(reinterpret_cast<list_node*>(rhs.head.next)->data,
			reinterpret_cast<list_node*>(n1->next)->data))
			slist_splice_after(n1,&rhs.head,rhs.head.next);
		n1 = n1->next;
	}
	if(rhs.head.next) {
		n1->next = rhs.head.next;
		rhs.head.next = nullptr;
	}
}

template<class T,class Alloc>
void slist<T,Alloc>::sort() {
	// use merge sort
	if(head.next && head.next->next) {
		slist carry;
		slist counter[64];
		int fill = 0;
		while(!empty()) {
			slist_splice_after(&carry.head,&head,head.next);
			int i = 0;
			while(i < fill && !counter[i].empty()) {
				counter[i].merge(carry);
				carry.swap(counter[i]);
				++i;
			}
			carry.swap(counter[i]);
			if(i == fill)
				++fill;
		}

		for(int i = 1; i < fill; ++i)
			counter[i].merge(counter[i-1]);
		swap(counter[fill-1]);
	}
}

template<class T,class Alloc>
template<class StrictWeakOrdering>
void slist<T,Alloc>::sort(StrictWeakOrdering comp) {
	// use merge sort
	if(head.next && head.next->next) {
		slist carry;
		slist counter[64];
		int fill = 0;
		while(!empty()) {
			slist_splice_after(&carry.head,&head,head.next);
			int i = 0;
			while(i < fill && !counter[i].empty()) {
				counter[i].merge(carry,comp);
				carry.swap(counter[i]);
				++i;
			}
			carry.swap(counter[i]);
			if(i == fill)
				++fill;
		}

		for(int i = 1; i < fill; ++i)
			counter[i].merge(counter[i-1],comp);
		swap(counter[fill-1]);
	}
}

// operator
template<class T,class Alloc>
inline bool operator==(const slist<T,Alloc>& lhs,const slist<T,Alloc>& rhs) {
	using const_iterator = typename slist<T,Alloc>::const_iterator;

	const_iterator end1 = lhs.cend();
	const_iterator end2 = rhs.cend();
	const_iterator i1 = lhs.cbegin();
	const_iterator i2 = rhs.cbegin();

	while(i1 != end1 && i2 != end2 && *i1 == *i2) {
		++i1;
		++i2;
	}
	return i1 == end1 && i2 == end2;
}

template<class T,class Alloc>
inline bool operator<(const slist<T,Alloc>& lhs,const slist<T,Alloc>& rhs) {
	return MiniSTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template<class T,class Alloc>
inline bool operator!=(const slist<T,Alloc>& lhs,const slist<T,Alloc>& rhs) {
	return !(lhs == rhs);
}

template<class T,class Alloc>
inline bool operator>(const slist<T,Alloc>& lhs,const slist<T,Alloc>& rhs) {
	return rhs < lhs;
}

template<class T,class Alloc>
inline bool operator<=(const slist<T,Alloc>& lhs,const slist<T,Alloc>& rhs) {
	return !(rhs < lhs);
}

template<class T,class Alloc>
inline bool operator>=(const slist<T,Alloc>& lhs,const slist<T,Alloc>& rhs) {
	return !(lhs < rhs);
}

// swap
template<class T,class Alloc>
inline void swap(slist<T,Alloc>& lhs,slist<T,Alloc>& rhs) {
	lhs.swap(rhs);
}

// specialization of insert_iterator so that insertions will be constant rather than linear time
template <class T, class Alloc>
class insert_iterator<slist<T,Alloc> > {
private:// data member
	using container = slist<T,Alloc>;
	container* con;
	typename container::iterator iter;

public:// alias declarations
	using container_type = container;
	using iterator_category = output_iterator_tag;
	using value_type = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

public:// ctor
  insert_iterator(container& c, typename container::iterator i) 
    : con(&c) {
    if (i == c.begin())
      iter = c.before_begin();
    else
      iter = c.previous(i);
  }

  insert_iterator<container>&
  operator=(const typename container::value_type& val) { 
    iter = con->insert_after(iter, val);
    return *this;
  }

public:// dereference
  insert_iterator<container>& operator*() { return *this; }

public:// increasement
  insert_iterator<container>& operator++() { return *this; }
  insert_iterator<container>& operator++(int) { return *this; }
};


}// end namespace::MiniSTL