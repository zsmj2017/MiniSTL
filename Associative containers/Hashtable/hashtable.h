#pragma once

#include <cstddef>
#include "stl_algo.h"
#include "stl_vector.h"

namespace MiniSTL{

template <class Value>
struct hashtable_node {
	Value val;
	hashtable_node* next;
};

// forward declarations
template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable;

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable_iterator;

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
class hashtable_const_iterator;

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct hashtable_iterator {
	// alias declarations
	using __hashtable = MiniSTL::hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using iterator = hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using const_iterator = hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using node = hashtable_node<Value>;

	// iterator declarations
	using iterator_category = forward_iterator_tag;//前向迭代器
	using value_type = Value;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
	using reference = Value & ;
	using pointer = Value * ;

	node* cur;//当前指向的节点
	__hashtable* ht;//保持与hashtable的连接

	hashtable_iterator(node* n, __hashtable* tab) :cur(n), ht(tab) {}
	hashtable_iterator() = default;
	reference operator*() const noexcept { return cur->val; }
	pointer operator->() const noexcept { return &(operator*()); }
	iterator& operator++() noexcept;
	iterator operator++(int) noexcept;
	bool operator==(const iterator&rhs) const noexcept { return cur == rhs.cur; }
	bool operator!=(const iterator&rhs) const noexcept { return cur != rhs.cur; }
};

template <class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
struct hashtable_const_iterator {
	// alias declarations
	using __hashtable = MiniSTL::hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using iterator = hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using const_iterator = hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>;
	using node = hashtable_node<Value>;

	// iterator declarations
	using iterator_category = forward_iterator_tag;
	using value_type = Value;
	using difference_type = ptrdiff_t;
	using size_type = size_t;
	using reference = const Value & ;
	using pointer = const Value * ;

	node* cur;
	__hashtable* ht;

	hashtable_const_iterator(node* n, __hashtable* tab) :cur(n), ht(tab) {}
	hashtable_const_iterator() = default;
	reference operator*() const noexcept { return cur->val; }
	pointer operator->() const noexcept { return &(operator*()); }
	const_iterator& operator++() noexcept;
	const_iterator operator++(int) noexcept;
	bool operator==(const iterator&rhs) const noexcept { return cur == rhs.cur; }
	bool operator!=(const iterator&rhs) const noexcept { return cur != rhs.cur; }
};

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc=simpleAlloc<Value> >
class hashtable {
public:// alias declarations
	using hasher = HashFcn;
	using key_equal = EqualKey;
	using size_type = size_t;
	using value_type = Value;
	using key_type = Key;

	using iterator = hashtable_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>;
	using const_iterator = hashtable_const_iterator<Value,Key,HashFcn,ExtractKey,EqualKey,Alloc>;

private:// data member
	// function object
	hasher hash;
	key_equal equals;
	ExtractKey get_key;

	using node = hashtable_node<Value>;
	using node_allocator = simpleAlloc<node>;

	vector<node*, Alloc> buckets;//以vector表征
	size_type num_elements;

private:// allocate && deallocate
	node * new_node(const value_type& obj) {
		node* n = node_allocator::allocate();
		try{
			construct(&n->val, obj);
			return n;
		}
		catch(std::exception&){
			node_allocator::deallocate(n);
		}
	}

	void delete_node(node* n) {
		destory(&n->val);
		node_allocator::deallocate(n);
	}

private:// data && interface for bucket
	// 当size为质数且负载低于0.5时两次查找必然可获取正确位置
	// 预先存储28个质数
	static const int __stl_num_primes = 28;
	constexpr static const unsigned long __stl_prime_list[__stl_num_primes] = {
		53ul,         97ul,         193ul,       389ul,       769ul,
		1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
		49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
		1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
		50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};

	//找出大于n的最小质数
	unsigned long __stl_next_prime(unsigned long n) const noexcept{
		const unsigned long* first = __stl_prime_list;
		const unsigned long* last = __stl_prime_list + __stl_num_primes;
		const unsigned long* pos = lower_bound(first, last, n);
		return pos == last ? *(last - 1) : *pos;
	}

	size_type max_bucket_count() const noexcept {
		return __stl_prime_list[__stl_num_primes - 1];
	}

	size_type bkt_num(const value_type& obj, size_type n) const { return bkt_num_key(get_key(obj), n); }
	size_type bkt_num(const value_type& obj) const { return bkt_num_key(get_key(obj)); }
	size_type bkt_num(const key_type& key) const { return bkt_num_key(key); }
	size_type bkt_num(const key_type& key, size_type n) const { return bkt_num_key(key, n); }
	void resize(size_type);
	pair<iterator, bool> insert_unique_noreseize(const value_type&);
	pair<iterator, bool> insert_equal_noresize(const value_type&);
private:// initialize
	void initialize_buckets(size_type n) {
		const size_type n_buckets = __stl_next_prime(n);
		//保留空间，由于此时vector's size==0，因此等价于全部置0
		buckets.reserve(n_buckets);
		buckets.insert(buckets.end(), n_buckets, static_cast<node*>(nullptr));
		num_elements = 0;
	}

public:// ctor
	hashtable(size_type n, const hasher& hf, const key_equal& eql)
		:hash(hf), equals(eql), get_key(ExtractKey()) {
		initialize_buckets(n);
	}

public:
	size_type bucket_count() const { return buckets.size(); }
	pair<iterator, bool> insert_unique(const value_type&);
	pair<iterator, bool> insert_equal(const value_type&);
	void clear();
	void copy_from(const hashtable&);
};

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint){
	//是否重建表格的原则为：若元素个数大于bucket's size,则试图重建表格（我想是为了保证负载率低于0.5）
	const size_type old_n = buckets.size();
	if (num_elements_hint > old_n) {//确定需要扩容
		const size_type n = __stl_next_prime(num_elements_hint);
		if (n > old_n) {
			vector<node*, Alloc> temp(n, static_cast<node*>(nullptr));
			try {
				//处理每一个旧bucket
				for (size_type bucket = 0; bucket < old_n; ++bucket) {
					node* first = buckets[bucket];
					while (first) {
						//找出节点应置于new_bucket的何处
						size_type new_bucket = bkt(first->val, n);
						buckets[bucket] = first->next;//将first与原vector分离
						first->next = temp[new_bucket];//first连接至new buckets
						temp[new_bucket] = first;//将first彻底放入new bucket内部
						first = buckets[bucket];//回归old bucket的下一个节点
					}
				}
				buckets.swap(temp);// copy && swap
			}
			catch(std::exception&){
				//TODO:
			}
		}
	}
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique_noreseize(const value_type& obj){
	const size_type n = bkt_num(obj);//决定位于哪个bucket
	node* first = buckets[n];
	for (node* cur = first; cur; cur=cur->next) {
		if (equals(get_key(cur->val)), get_key(obj))//存在相同键值，拒绝插入
			return pair<iterator, bool>(iterator(cur,this), false);
	}
	//当前已离开循环或根本未进入循环,创造新节点并将其作为bucket的头部
	node* temp = new_node(obj);
	temp->next = first;
	buckets[n] = temp;
	++num_elements;
	return pair<iterator, bool>(iterator(temp), true);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj){
	const size_type n = bkt_num(obj);
	node* first = buckets[n];
	for (node* cur = first; cur; cur = cur->next) {
		if (equals(get_key(cur->val)), get_key(obj)) {
			node* temp = new_node(obj);
			temp->next = cur->next;
			cur->next = temp;
			++num_elements;
			return pair<iterator, bool>(iterator(temp), true);
		}
	}
	node* temp = new_node(obj);
	temp->next = first;
	buckets[n] = temp;
	++num_elements;
	return pair<iterator, bool>(iterator(temp), true);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator, bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_unique(const value_type & obj){
	resize(num_elements + 1);//判断是否需要扩充
	return insert_unique_noreseize(obj);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline pair<typename hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator,bool>
hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::insert_equal(const value_type& obj){
	resize(num_elements + 1);
	return insert_equal_noresize(obj);
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::clear(){
	for (size_type i = 0; i != buckets.size(); ++i) {
		node* cur = buckets[i];
		while (cur) {
			node* next = cur->next;
			delete_node(cur);
			cur = next;
		}
		buckets[i] = nullptr;
	}
	num_elements = 0;
	//注意clear并没有释放vector
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline void hashtable<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable & rhs){
	buckets.clear();
	buckets.reserve(rhs.buckets.size());
	buckets.insert(buckets.end(), rhs.buckets.size(), static_cast<node*>(nullptr));
	try {
		for (size_type i = 0; i != rhs.buckets.size(); ++i) {
			//复制每一个vector元素
			if (const node* cur = rhs.buckets[i]) {
				node* copy = new_node(cur->val);
				buckets[i] = copy;
				for (node* next = cur->next; next; cur = next, next = next->next) {
					copy->next = new_node(next->val);
					copy = copy->next;
				}
			}
		}
		num_elements = rhs.num_elements;
	}
	catch(std::exception&){
		clear(); 
	}
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator&
hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() noexcept {
	const node* old = cur;
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
inline typename hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::iterator
hashtable_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) noexcept {
	iterator temp = *this;
	++*this;
	return temp;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
typename hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator&
hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++() noexcept {
	const node* old = cur;
	cur = cur->next;
	if (!cur) {
		size_type bucket = ht->bkt_num(old->val);
		while (!cur && ++bucket < ht->buckets.size())
			cur = ht->buckets[bucket];
	}
	return *this;
}

template<class Value, class Key, class HashFcn, class ExtractKey, class EqualKey, class Alloc>
inline typename hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::const_iterator
hashtable_const_iterator<Value, Key, HashFcn, ExtractKey, EqualKey, Alloc>::operator++(int) noexcept {
	const_iterator temp = *this;
	++*this;
	return temp;
}

}// end namespace::MiniSTL
