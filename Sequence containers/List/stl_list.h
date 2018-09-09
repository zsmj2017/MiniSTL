#pragma once
#include "stl_list_iterator.h"
#include "allocator.h"
#include "uninitialized.h"

namespace MiniSTL {

template<class T, class Alloc = simpleAlloc<T> >
class list {
	// friend declarations
	template<class T>
	friend void swap(list<T>&, list<T>&);
	template <class T>
	friend bool operator== (const list<T>& lhs, const list<T>& rhs);
	template <class T>
	friend bool operator!= (const list<T>& lhs, const list<T>& rhs);

public:// alias declarations
	using iterator = __list_iterator<T, T&, T*>;
	using const_iterator = __list_iterator<T, const T&, const T*>;
	using reverse_iterator = MiniSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = MiniSTL::reverse_iterator<const_iterator>;
	using value_type = typename __list_iterator<T, T&, T*>::value_type;
	using pointer = typename __list_iterator<T, T&, T*>::pointer;
	using reference = typename __list_iterator<T, T&, T*>::reference;
	using link_type = typename __list_iterator<T, T&, T*>::link_type;
	using size_type = typename __list_iterator<T, T&, T*>::size_type;
	using difference_type = typename __list_iterator<T, T&, T*>::difference_type;

private: // interface about allocate/deallocate litsNode
	using list_node = __list_node<T>;
	using list_node_allocator = simpleAlloc<list_node>;
	link_type get_node() { return list_node_allocator::allocate(); }
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	link_type create_node(const T&);
	void destroy_node(link_type p) { destroy(&(p->data));put_node(p);}

private: // only data member(tail)
	link_type node;

private:// aux_interface
	void empety_initialized();
	// Move [first, last) before pos
	void transfer(iterator position, iterator first, iterator last);
	void swap(list&) noexcept;

public:// ctor && dtor
	list() { empety_initialized(); }
	explicit list(size_type, const value_type& value = value_type());
	list(std::initializer_list<T> il) { empety_initialized(); insert(begin(), il.begin(), il.end()); }
	~list() { clear(); put_node(node); }

public:// copy operation
	list(const list&);
	list& operator=(const list&) noexcept;

public:// move operation
	list(list&&) noexcept;
	list& operator=(list&&) noexcept;

public:// getter
	bool empty() const noexcept { return node->next == node; }
	size_type size() const noexcept { return distance(cbegin(), cend());}
	const_iterator cbegin() const noexcept {return static_cast<link_type>(node->next);}
	const_iterator cend() const noexcept  { return node; }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

public:// setter
	iterator begin() noexcept { return static_cast<link_type>(node->next); }
	iterator end() noexcept { return node; }
	reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
	reverse_iterator rend() noexcept { return reverse_iterator(begin()); }
	reference front() noexcept { return *begin(); }
	reference back() noexcept { return *(--end()); }

public:// interafce about insert && erase
	iterator insert(iterator, const value_type&);
	void insert(iterator, size_type, const value_type&);
	template<class InputIterator>
	void insert(iterator,InputIterator, InputIterator);
	// without this,InputIterator can be deduced as int
	void insert(iterator, int, int);
	iterator erase(iterator);
	iterator erase(iterator, iterator);
	void push_front(const T&value) { insert(begin(), value); }
	void push_back(const T&value) { insert(end(), value); }
	void pop_fornt() { erase(begin()); }
	void pop_back() { iterator temp = end();erase(--temp);}
	

public:// other interface
	void unique();
	void splice(iterator, list&);
	void splice(iterator, list&, iterator);
	void merge(list&);
	void reverse();
	void sort();
	void clear();
	void remove(const T&);
};

template<class T, class Alloc>
inline typename list<T, Alloc>::link_type list<T, Alloc>::create_node(const T & value){
	link_type p = get_node();
	construct(&p->data, value);
	return p;
}

template<class T, class Alloc>
inline void list<T, Alloc>::empety_initialized(){
	node = get_node();
	node->next = node;
	node->prev = node;
}

template<class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, const value_type& value){
	link_type temp = create_node(value);
	temp->next = position.node;
	temp->prev = position.node->prev;
	static_cast<link_type>(position.node->prev)->next = temp;
	position.node->prev = temp;
	return temp;
}

template<class T, class Alloc>
inline void list<T, Alloc>::insert(iterator position, size_type n, const value_type& value){
	for (size_type i = n; i != 0; --i)
		position = insert(position, value);
}

template<class T, class Alloc>
template<class InputIterator>
inline void list<T, Alloc>::insert(iterator position, InputIterator first, InputIterator last){
	for (--last; first != last; --last)
		position = insert(position, *last);
	insert(position, *last);
}

template<class T, class Alloc>
inline void list<T, Alloc>::insert(iterator position, int n, int value){
	for (int i = n; i != 0; --i)
		position = insert(position, value);
}

template<class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator position){
	link_type next_node = static_cast<link_type>(position.node->next);
	link_type prev_node = static_cast<link_type>(position.node->prev);
	prev_node->next = next_node;
	next_node->prev = prev_node;
	destroy_node(position.node);
	return static_cast<iterator>(next_node);
}

template<class T, class Alloc>
inline typename list<T, Alloc>::iterator list<T, Alloc>::erase(iterator first, iterator last){
	iterator res;
	while(first!=last)
		res = erase(first++);
	return res;
}

template<class T, class Alloc>
inline void list<T, Alloc>::transfer(iterator position, iterator first, iterator last){
	if (position != last) {
		(*(static_cast<link_type>((*last.node).prev))).next = position.node;
		(*(static_cast<link_type>((*first.node).prev))).next = last.node;
		(*(static_cast<link_type>((*position.node).prev))).next = first.node;
		link_type temp = static_cast<link_type>((*position.node).prev);
		(*position.node).prev = (*last.node).prev;
		last.node->prev = first.node->prev;
		first.node->prev = temp;
	}
}

template<class T, class Alloc>
inline void list<T, Alloc>::swap(list & rhs) noexcept{
	using std::swap;
	swap(node, rhs.node);
}

template<class T, class Alloc>
inline list<T, Alloc>::list(size_type n, const value_type & value){
	empety_initialized();
	while (n--)
		push_back(value);
}

template<class T, class Alloc>
inline list<T, Alloc>::list(const list &rhs){
	empety_initialized();
	for (auto it = rhs.cbegin(); it != rhs.cend(); ++it)
		push_back(*it);
}

template<class T, class Alloc>
inline list<T, Alloc>& list<T, Alloc>::operator=(const list &rhs) noexcept{
	list temp(rhs);
	swap(temp);
	return *this;
}

template<class T, class Alloc>
inline list<T, Alloc>::list(list &&rhs) noexcept{
	node = rhs.node;
	rhs.node = nullptr;
}

template<class T, class Alloc>
inline list<T, Alloc>& list<T, Alloc>::operator=(list&& rhs) noexcept{
	if (node != rhs.node) {
		clear();
		node = rhs.node;
		rhs.node = nullptr;
	}
	return *this;
}

template<class T, class Alloc>
inline void list<T, Alloc>::clear() {
	link_type cur = static_cast<link_type>(node->next);
	while (cur != node) {
		link_type temp = cur;
		cur = static_cast<link_type>(cur->next);
		destroy_node(temp);
	}
	//恢复原始状态
	node->next = node;
	node->prev = node;
}

template<class T, class Alloc>
inline void list<T, Alloc>::remove(const T& value) {
	iterator first = begin();
	iterator last = end();
	while (first != last) {
		iterator next = first;
		++next;
		if (*first == value) erase(first);
		first = next;
	}
}

template<class T, class Alloc>
inline void list<T, Alloc>::unique() {
	iterator first = begin();
	iterator last = end();
	if (first == last) return;
	iterator next = first;
	while (++next != last) {
		if (*first == *next) {
			erase(next);
			next = first;//修正next
		}
		else
			first = next;
	}
}

template<class T, class Alloc>
inline void list<T, Alloc>::splice(iterator position, list& x) {
	if (!x.empty())
		transfer(position, x.begin(), x.end());
}

template<class T, class Alloc>
inline void list<T, Alloc>::splice(iterator position, list &, iterator i) {
	iterator j = i;
	++j;
	//i==pos 自身无法插于自身之前
	//j==pos 已处于pos之前
	if (position == i || position == j) return;
	transfer(position, i, j);
}

template<class T, class Alloc>
inline void list<T, Alloc>::merge(list& x) {
	iterator first1 = begin();
	iterator last1 = end();
	iterator first2 = x.begin();
	iterator last2 = x.end();

	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1) {
			iterator next = first2;
			transfer(first1, first2, ++next);
			first2 = next;
		}
		else
			++first1;
	}
	if (first2 != last2) transfer(last1, first2, last2);
}

template<class T, class Alloc>
inline void list<T, Alloc>::reverse() {
	//空list或仅有一个元素
	if (node->next == node || static_cast<link_type>(node->next)->next == node) return;
	iterator first = begin();
	++first;//begin自身并不需要移动，它将作为指示末元素的哨兵（确切地说，最终begin.node->next == end.node)
	while (first != end()) {
		iterator old = first;
		++first;
		transfer(begin(), old, first);
	}
}

// More information can be seen at https://blog.csdn.net/qq276592716/article/details/7932483
template<class T, class Alloc>
inline void list<T, Alloc>::sort() {
	if (node->next == node || static_cast<link_type>(node->next)->next == node) return;
	//中介数据存放区 counter[n]中最多存放2^(n+1)个元素，若大于则与counter[n+1]作归并
	list carry;
	list counter[64];
	int fill = 0;
	while (!empty()) {
		carry.splice(carry.begin(), *this, begin());
		int i = 0;
		while (i < fill && !counter[i].empty()) {
			counter[i].merge(carry);
			carry.swap(counter[i++]);
		}
		carry.swap(counter[i]);
		if (i == fill)
			++fill;
	}
	for (int i = 1; i < fill; ++i)
		counter[i].merge(counter[i - 1]);
	swap(counter[fill - 1]);
}

template<class T>
inline void swap(list<T>& lhs, list<T>& rhs){
	lhs.swap(rhs);
}

template<class T>
bool operator==(const list<T>& lhs, const list<T>& rhs){
	auto it1 = lhs.cbegin(), it2 = rhs.cbegin();
	for (; it1 != lhs.cend() && it2 != rhs.cend(); ++it1, ++it2)
		if (*it1 != *it2) return false;
	return it1 == lhs.cend() && it2 == rhs.cend();
}

template<class T>
inline bool operator!=(const list<T>& lhs, const list<T>& rhs){
	return !(lhs == rhs);
}

}// end namespace::MiniSTL
