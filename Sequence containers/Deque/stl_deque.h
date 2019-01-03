#pragma once

#include "deque_iterator.h"
#include "allocator.h"
#include "uninitialized.h"

namespace MiniSTL {

template<class T, class Alloc = simpleAlloc<T>>
class deque {
public:// alias declarations
	using value_type = T;
	using pointer = T* ;
	using reference = T& ;
	using const_reference = const T&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using iterator = __deque_iterator<T,T&,T*>;
	using reverse_iterator = MiniSTL::__reverse_iterator<iterator>;
	using const_iterator = __deque_iterator<T, const T&, const T*>;
	using const_reverse_iterator = MiniSTL::__reverse_iterator<const_iterator>;

private:// internal alias declarations
	using map_pointer = pointer* ;
	using node_allocator = simpleAlloc<value_type>;
	using map_allocator = simpleAlloc<pointer>;

private:// data member
	iterator start;// 第一个节点
	iterator finish;// 最后一个节点
	map_pointer map;// 指向节点的指针
	size_type map_size;
	
private:// aux_interface for node
	value_type* allocate_node() { return node_allocator::allocate(__deque_buf_size(sizeof(value_type))); }
	void deallocate_node(value_type* p) { node_allocator::deallocate(p, __deque_buf_size(sizeof(value_type))); }
	void create_nodes(map_pointer,map_pointer);
	void destroy_nodes(map_pointer,map_pointer);

private:// aux_interface for map
	void initialize_map(size_type);
	void deallocate_map(map_pointer p,size_type n) { map_allocator::deallocate(p,n); }
	void reallocate_map(size_type, bool);
	void reverse_map_at_back(size_type nodes_to_add = 1);
	void reverse_map_at_front(size_type nodes_to_add = 1);

private:// aux_interface for ctor
	size_type initial_map_size() const noexcept { return 8U; }
	size_type buffer_size() const noexcept { return iterator::buffer_size(); }
	void fill_initialize(const value_type&);
	template<class Integer>
	void initialize_dispatch(Integer n,Integer val,_true_type){
		initialize_map(static_cast<size_type>(n));
		fill_initialize(static_cast<value_type>(val));
	}
	template<class InputIterator>
	void initialize_dispatch(InputIterator first,InputIterator last,_false_type){
		range_initialize(first,last,iterator_category_t<InputIterator>());
	}

public:// ctor && dtor
	deque():start(), finish(), map(nullptr), map_size(0) { initialize_map(0); }
	explicit deque(size_type n) :start(), finish(), map(nullptr), map_size(0) { initialize_map(n);fill_initialize(value_type()); }
	deque(size_type n,const value_type& val) :start(), finish(), map(nullptr), map_size(0) { initialize_map(n);fill_initialize(val); }
	template<class InputIterator>
	deque(InputIterator first, InputIterator last) { initialize_dispatch(first,last,_is_integer_t<InputIterator>()); }
	~deque();

public:// copy operations
	deque(const deque& rhs) { initialize_map(rhs.size());MiniSTL::uninitialized_copy(rhs.begin(), rhs.end(), start); }
	deque& operator=(const deque&);

public:// move operations
	deque(deque&&);
	// TODO:
	//deque& operator=(const deque&);

public:// getter
	const_iterator begin() const noexcept { return start; }
	const_iterator end() const noexcept { return finish; }
	const_iterator cbegin() const noexcept { return start; }
	const_iterator cend() const noexcept { return finish; }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(finish); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(start); }
	const_reference operator[](size_type n) const noexcept { return start[static_cast<difference_type>(n)]; }
	const_reference front() const noexcept { return *start; }
	const_reference back() const noexcept { return *(finish - 1);}
	size_type size() const noexcept { return finish - start; }
	bool empty() const noexcept { return finish == start; }

public:// setter
	iterator begin() { return start; }
	iterator end() { return finish; }
	reverse_iterator rbegin() { return reverse_iterator(finish); }
	reverse_iterator rend() { return reverse_iterator(start); }
	reference operator[](size_type n) {return start[static_cast<difference_type>(n)];}
	reference front() { return *start; }
	reference back() { return *(finish - 1); }

private:// aux_interface for push && pop
	void push_back_aux(const value_type&);
	void push_front_aux(const value_type&);
	void pop_back_aux();
	void pop_front_aux();

public:// push && pop
	void push_back(const value_type&);
	void push_front(const value_type&);
	void pop_back();
	void pop_front();

private:// aux_interface for insert
	iterator insert_aux(iterator, const value_type&);

public:// insert
	iterator insert(iterator pos, const value_type &value);

public:// erase
	iterator erase(iterator pos);
	iterator erase(iterator first, iterator last);
	void clear();

public:// swap
	void swap(deque& rhs) noexcept;
};

template<class T, class Alloc>
void deque<T, Alloc>::create_nodes(map_pointer nstart,map_pointer nfinish) {
	map_pointer cur;
	try {
		// 为每一个节点配置空间
		for (cur = nstart; cur <= nfinish; ++cur)
			*cur = allocate_node();
	}
	catch (std::exception&) {
		destroy_nodes(nstart,cur);
		throw;
	}
}

template<class T, class Alloc>
void deque<T, Alloc>::destroy_nodes(map_pointer nstart,map_pointer nfinish) {
	for(map_pointer n = nstart; n < nfinish; ++n)
		deallocate_node(*n);
}

template<class T, class Alloc>
void deque<T, Alloc>::initialize_map(size_type n) {
	// 所需节点数（整除则多配置一个）
	size_type num_nodes = n / buffer_size() + 1;
	// 一个map至少管理8个节点，至多管理num_nodes+2个
	map_size = MiniSTL::max(initial_map_size(),num_nodes+2);
	map = map_allocator::allocate(map_size);
	// 令nstart与nfinish指向map所拥有的全部node的中部,以便日后扩充头尾
	map_pointer nstart = map + (map_size - num_nodes) / 2;
	map_pointer nfinish = nstart + num_nodes - 1;
	try {
		create_nodes(nstart,nfinish);
	}
	catch (std::exception&) {
		deallocate_map(map,map_size);
		map = nullptr;
		map_size = 0;
		throw;
	}
	start.set_node(nstart);
	finish.set_node(nfinish);
	start.cur = start.first;
	finish.cur = finish.first + n % buffer_size();// 若n%buffer_size==0,会多配置一个节点，此时cur指向该节点头部
}

template<class T, class Alloc>
void deque<T, Alloc>::fill_initialize(const value_type & val) {
	map_pointer cur;
	try {
		// 为每个缓冲区设定初值
		for (cur = start.node; cur < finish.node; ++cur)
			MiniSTL::uninitialized_fill(*cur, *cur + buffer_size(), val);
		// 最后一个缓冲区只设定至需要处
		MiniSTL::uninitialized_fill(finish.first, finish.cur, val);
	}
	catch (std::exception&) {
		destroy(start,iterator(*cur,cur));
		throw;
	}
}

template<class T, class Alloc>
inline void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front) {
	size_type old_num_nodes = finish.node - start.node + 1;
	size_type new_num_nodes = old_num_nodes + nodes_to_add;

	map_pointer new_nstart;
	if (map_size > 2 * new_num_nodes) {
		// 某一端出现失衡，因此释放存储区完成重新中央分配，
		// 规定新的nstart，若添加在前端则向后多移动n个单位
		new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		if (new_nstart < start.node)// 整体前移
			MiniSTL::copy(start.node, finish.node + 1, new_nstart);
		else// 整体后移
			MiniSTL::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
	}
	else {
		size_type new_map_size = map_size + max(map_size, nodes_to_add) + 2;
		// 分配新空间
		map_pointer new_map = map_allocator::allocate(new_map_size);
		new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
		// 拷贝原有内容
		MiniSTL::copy(start.node, finish.node + 1, new_nstart);
		// 释放原map
		map_allocator::deallocate(map, map_size);
		// 重新设定map
		map = new_map;
		map_size = new_map_size;
	}

	//设定start与finish
	start.set_node(new_nstart);
	finish.set_node(new_nstart + old_num_nodes - 1);// 注意并非new_num,接下来的设定转交其他函数处理
}

template<class T, class Alloc>
inline deque<T, Alloc>& deque<T, Alloc>::operator=(const deque& rhs) {
	const size_type len = size();
	if(&rhs != this){
		if( len >= rhs.size())
			erase(MiniSTL::copy(rhs.begin(),rhs.end(),start),finish);
		else{
			const_iterator mid = rhs.begin() + static_cast<difference_type>(len);
			MiniSTL::copy(rhs.begin(),mid,start);
			insert(finish,mid,rhs.end());
		}
	}
	return *this;
}

template<class T, class Alloc>
inline void deque<T, Alloc>::reverse_map_at_back(size_type nodes_to_add) {
	// map_size-(finish.node-map+1) == 后端剩余node个数
	if (nodes_to_add > map_size - (finish.node - map + 1))
		reallocate_map(nodes_to_add, false);
}

template<class T, class Alloc>
inline void deque<T, Alloc>::reverse_map_at_front(size_type nodes_to_add) {
	// start.node-map==前端剩余node个数
	if (nodes_to_add > start.node - map)
		reallocate_map(nodes_to_add, true);
}

template<class T, class Alloc>
inline void deque<T, Alloc>::push_back_aux(const value_type & value) {
	value_type value_copy = value;
	reverse_map_at_back();// 若符合条件则重新更换map
	*(finish.node + 1) = allocate_node();// 配置新节点
	try {
		construct(finish.cur, value_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;// 更新finish.cur为当前first
	}
	catch(std::exception&){
		deallocate_node(*(finish.node + 1));
	}
}

template<class T, class Alloc>
inline void deque<T, Alloc>::push_front_aux(const value_type& value) {
	value_type value_copy = value;
	reverse_map_at_front();// 若符合条件则重新更换map
	*(start.node - 1) = allocate_node();// 配置新节点
	try {
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, value);
	}
	catch (std::exception&){
		start.set_node(start.node + 1);
		start.cur = start.first;
		deallocate_node(*(start.node - 1));
	}
}

template<class T, class Alloc>
inline void deque<T, Alloc>::pop_back_aux() {
	node_allocator::deallocate(finish.first);
	finish.set_node(finish.node - 1);
	finish.cur = finish.last - 1;
	destroy(finish.cur);
}

template<class T, class Alloc>
inline void deque<T, Alloc>::pop_front_aux() {
	destroy(start.cur);
	node_allocator::deallocate(start.first);
	start.set_node(finish.node + 1);
	start.cur = start.first;
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::insert_aux(iterator pos, const value_type & value) {
	difference_type index = pos - start;// 插入点之前的元素个数
	value_type value_copy = value;
	if (static_cast<size_type>(index) < size() / 2) {// 前移
		// 插图见书
		push_front(front());// 最前端加入哨兵以作标识，注意此时start发生了改变
		iterator front1 = start; ++front1; // 复制后自增效率较高
		iterator front2 = front1; ++front2;
		pos = start + index;
		iterator pos1 = pos; ++pos1;
		MiniSTL::copy(front2, pos1, front1);// 移动元素
	}
	else {
		// 过程类似于上
		push_back(back());
		iterator back1 = finish;--back1;
		iterator back2 = back1;--back2;
		pos = start + index;
		MiniSTL::copy_backward(pos, back2, back1);
	}
	*pos = value_copy;
	return pos;
}

template<class T, class Alloc>
inline deque<T, Alloc>::deque(deque&& rhs){
	initialize_map(0);
	if(rhs.map){
		swap(rhs);
	}
}

template<class T, class Alloc>
inline deque<T, Alloc>::~deque(){
	destroy(start, finish);
	if(map){
		destroy_nodes(start.node,finish.node + 1); // 也需要destroy finish.node
		deallocate_map(map,map_size);
	}
}

template<class T, class Alloc>
inline void deque<T, Alloc>::push_back(const value_type& value) {
	// finish的cur指向最后一个元素的下一个位置，因此if语句表征至少还有一个备用空间
	if (finish.cur != finish.last - 1) {
		construct(finish.cur, value);
		++finish.cur;
	}
	else
		// 最终缓冲区已无或仅剩一个空间（我认为必然为仅剩一个空间的状态）
		push_back_aux(value);
}

template<class T, class Alloc>
inline void deque<T, Alloc>::push_front(const value_type& value) {
	if (start.cur != start.first) {
		construct(start.cur - 1, value);
		--start.cur;
	}
	else
		push_front_aux(value);
}

template<class T, class Alloc>
inline void deque<T, Alloc>::pop_back() {
	if (finish.cur != finish.first) {
		// 缓冲区至少存在一个元素
		--finish.cur;
		destroy(finish.cur);
	}
	else
		pop_back_aux();
}

template<class T, class Alloc>
inline void deque<T, Alloc>::pop_front() {
	if (start.cur != start.last - 1) {
		destroy(start.cur);
		++start.cur;
	}
	else
		pop_front_aux();
}

template<class T, class Alloc>
inline void deque<T, Alloc>::clear() {
	// 清空所有node，保留唯一缓冲区（需要注意的是尽管map可能存有更多节点，但有[start,finish]占据内存
	for (map_pointer node = start.node + 1; node < finish.node; ++node) {//内部均存有元素
		destroy(*node, *node + buffer_size());//析构所有元素
		node_allocator::deallocate(*node, buffer_size());
	}
	if (start.node != finish.node) {// 存在头尾两个缓冲区
		// 析构其中所有元素
		destroy(start.cur, start.last);
		destroy(finish.first, finish.cur);
		// 保存头部，释放尾部
		node_allocator::deallocate(finish.first, buffer_size());
	}
	else
		destroy(start.cur, finish.cur);// 利用finish.cur标记末尾
	finish = start;
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::erase(iterator pos) {
	iterator next = pos + 1;
	difference_type index = pos - start;// 清除点前的元素个数
	if (index < size() / 2) {// 后移开销较低
		MiniSTL::copy_backward(start, pos, pos);
		pop_front();
	}
	else {
		copy(next, finish, pos);
		pop_back();
	}
	return start + index;
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::erase(iterator first, iterator last) {
	if (first == start && last == end) {
		clear();
		return finish;
	}
	else {
		difference_type n = last - first; // 清除区间长度
		difference_type elems_before = first - start; // 前方元素个数
		if (elems_before < (size() - n) / 2) {// 后移开销较低
			MiniSTL::copy_backward(start, first, last);
			iterator new_start = start + n;// 标记新起点
			destroy(start, new_start);// 析构多余元素
			// 释放多余缓冲区
			for (map_pointer cur = start.node; cur < new_start.node; ++cur)
				node_allocator::deallocate(*cur, buffer_size());
			start = new_start;
		}
		else {// 前移开销较低		
			copy(last, finish, first);
			iterator new_finish = finish - n;// 标记末尾
			destroy(new_finish, finish);
			// 释放多余缓冲区
			for (map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur)
				node_allocator::deallocate(*cur, buffer_size());
			finish = new_finish;
		}
		return start + elems_before;
	}
}

template<class T, class Alloc>
typename deque<T, Alloc>::iterator 
deque<T, Alloc>::insert(iterator pos, const value_type& value) {
	if (pos.cur == start.cur) {
		push_front(value);
		return start;
	}
	else if (pos.cur == finish.cur) {
		push_back(value);
		iterator temp = finish - 1;
		return temp;
	}
	else
		return insert_aux(pos, value);
}

template<class T, class Alloc>
void deque<T, Alloc>::swap(deque& rhs) noexcept {
	MiniSTL::swap(start,rhs.start);
	MiniSTL::swap(finish,rhs.finish);
	MiniSTL::swap(map,rhs.map);
	MiniSTL::swap(map_size,rhs.map_size);
}

}// end namespace::MiniSTL
