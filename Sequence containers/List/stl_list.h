#pragma once
#include "stl_list_iterator.h"

template<class T,class Alloc=alloc>//默认以alloc为空间配置器
class list {//双向循环链表
protected:
	using list_node = __list_node<T>;
	//专属适配器，每次配置一个节点的大小
	using list_node_allocator = simple_alloc<list_node, Alloc>;
public:
	using link_type = list_node;
	using iterator = __list_iterator::iterator;
	using reference = __list_iterator::reference;
	using size_type = __list_iterator::size_type;
protected:
	//配置与释放node
	link_type get_node() { return list_node_allocator::alloc(); }
	void put_node(link_type p) { list_node_allocator::deallocate(p); }
	//创建与析构node
	link_type create_node(const T& value) {
		link_type p = get_node();
		construct(&p->data, value);
		return p;
	}
	void destory_node(link_type p) {
		destory(&p->data);
		put_node(p);
	}
protected:
	list_node node;
protected:
	//产生一个空list
	void empety_initialized() {
		node = get_node();
		node->next = node;
		node->prev = node;
	}
	//在指定位置之前插入value
	iterator insert(iterator position, const T&value) {
		link_type temp = create_node(value);
		temp->next = position.node;
		temp->prev = position.node->prev;
		static_cast<link_type>(position.node->prev)->next = temp;
		position.node->prev = temp;
		return temp;
	}
	//删除指定位置的节点
	iterator erase(iterator position) {
		//构造局部对象是否影响效率？
		link_type next_node = static_cast<link_type>(position.node->next);
		link_type prev_node = static_cast<link_type>(position.node->prev);
		prev_node->next = next_node;
		next_node->prev = prev_node;
		destory_node(position.node);
		return static_cast<iterator>(next_node);
	}
	//将[first,last)区间移动至pos之前
	void transfer(iterator position, iterator first, iterator last) {
		if (position != last) {
			//将区间抽出
			static_cast<link_type>(last.node->prev)->next = position.node;
			static_cast<link_type>(first.node->prev)->next = last.node;
			//区间插入
			static_cast<link_type>(position.node->prev)->next = first.node;
			link_type temp = position.node->prev;
			position.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = temp;
		}
	}

public:
	list() { empety_initialized(); }
public:
	//静态接口
	//若以node为末节点(末节点似乎为哨兵），则以下五个接口时间复杂度为O(1)
	iterator begin() { return static_cast<link_type>(node->next); }
	iterator end() { return node; }
	bool empty() const { return node->next == node; }
	reference front() { return *begin(); }
	reference back()  { return *end(); }
	
	size_type size() const {
		size_type result = 0;
		distance(begin(), end(), result);//全局函数，见stl_iterator.h
		return result;
	}
public:
	//动态接口
	void push_front(const T&value) { insert(begin(), value); }
	void push_back(const T&value) { insert(end(), value); }
	void pop_fornt() { erase(begin()); }
	void pop_back() {
		iterator temp = end();
		erase(--temp);
	}
	void clear();
	void remove(const T& value);
	//连续去重
	void unique();
	//将x移动至pos之前,x必须不同于*this
	void splice(iterator position, list& x);
	//将i所指向元素移动至pos之前，pos与i可能指向同一个list
	void splice(iterator position, list&, iterator i);
	//将两个有序的list归并
	void merge(list& x);
	void reverse();
	void sort();
};

template<class T, class Alloc>
inline void list<T, Alloc>::clear(){
	link_type cur = static_cast<link_type>(node->next);//begin()
	while (cur != node) {
		link_type temp = cur;
		cur = static_cast<link_type>(cur->next);
		destory_node(temp);
	}
	//恢复原始状态
	node->next = node;
	node->prev = node;
}

template<class T, class Alloc>
inline void list<T, Alloc>::remove(const T& value){
	iterator first = begin();
	iterator end = end();
	while (first != end) {
		iterator next = first;
		++next;
		if (*first == value) erase(first);
		first = next;
	}
}

template<class T, class Alloc>
inline void list<T, Alloc>::unique(){
	iterator first = begin();
	iterator end = end();
	if (first == end) return;
	iterator next = first;
	while (++next != end) {
		if (*first == *next) {
			erase(next);
			next = first;//修正next
		}
		else
			first = next;
	}
}

template<class T, class Alloc>
inline void list<T, Alloc>::splice(iterator position, list& x){
	if (!x.empty()) 
		transfer(position, x.begin(), x.end();
}

template<class T, class Alloc>
inline void list<T, Alloc>::splice(iterator position, list &, iterator i){
	iterator j = i;
	++j;
	//i==pos 自身无法插于自身之前
	//j==pos 已处于pos之前
	if (position == i || position == j) return;
	transfer(position, i, j);
}

template<class T, class Alloc>
inline void list<T, Alloc>::merge(list& x){
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
inline void list<T, Alloc>::reverse(){
	//空list或仅有一个元素
	if (node->next == node || node->next->next == node) return;
	iterator first = begin();
	++first;//begin自身并不需要移动，它将作为指示末元素的哨兵（确切地说，最终begin.node->next == end.node)
	while (first != end()) {
		iterator old = first;
		++first;
		transfer(begin(), old, first);
	}
}

//STL list不可使用STL sort算法，后者需要randomAccess迭代器
//本member function采用quick sort（然而我感觉类似于归并排序）
//算法推衍见 https://blog.csdn.net/qq276592716/article/details/7932483
template<class T, class Alloc>
inline void list<T, Alloc>::sort(){
	if (node->next == node || node->next->next == node) return;
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
