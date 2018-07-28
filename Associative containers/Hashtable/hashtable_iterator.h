#pragma once

#include <cstddef>

template <class Value>
struct __hashtable_node {
	Value val;
	__hashtable_node* next;
};


template <class Value,class Key,class HashFcn,class ExtractKey,class EqualKey,class Alloc>
struct __hashtable_iterator {
	using hashtable = hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using iterator = __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using const_iterator = __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using node = __hashtable_node<Value>;

	using iterator_category = forward_iterator_tag;//前向迭代器
	using value_type = Value;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
	using reference = Value & ;
	using pointer = Value * ;

	node* cur;//当前指向的节点
	hashtable* ht;//保持与hashtable的连接

	__hashtable_iterator(node* n,hashtable* tab):cur(n),ht(tab) {}
	__hashtable_iterator() = default;
	reference operator*() const { return cur->val; }
	pointer operator->() const { return &(operator*()); }
	iterator& operator++();
	iterator operator++(int);
	bool operator==(const iterator&rhs) const { return cur == rhs.cur; }
	bool operator!=(const iterator&rhs) const { return cur != rhs.cur; }
};

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator&
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(){
	cur = cur->next;
	if (!cur) {
		//根据元素值定位下一个bucket
		size_type bucket = ht->bkt_num(old->val);
		while (!cur && ++bucket < ht->buckets.size())//直到找到一个内含list的bucket
			cur = ht->buckets[bucket];
	}
	return *this;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename __hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator 
__hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int){
	iterator temp = *this;
	++*this;
	return temp;
}
