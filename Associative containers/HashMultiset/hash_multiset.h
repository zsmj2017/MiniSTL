#pragma once


template <class Value, class HashFcn, class EqualKey, class Alloc>
class hash_multiset;

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs, const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs);

template <class Value, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc>
class hash_multiset {
private:
	using ht = hashtable<Value, Value, HashFcn, identity<Value>, EqualKey, Alloc>;
	ht rep;//底层数据结构

public:
	using key_type = typename ht::key_type;
	using value_type = typename ht::value_type;
	using hasher = typename ht::hasher;
	using key_equal = typename ht::key_equal;

	//显然，set的键值无法修改，采取const形式
	using pointer = typename ht::const_pointer;
	using const_pointer = typename ht::const_pointer;
	using reference = typename ht::const_reference;
	using const_reference = typename ht::const_reference;
	using iterator = typename ht::const_iterator;
	using const_iterator = typename ht::const_iterator;

	using size_type = typename ht::size_type;
	using difference_type = typename ht::difference_type;
	using allocator_type = typename ht::allocator_type;

	hasher hash_funct() const { return rep.hash_funct(); }
	key_equal key_eq() const { return rep.key_eq(); }
	allocator_type get_allocator() const { return rep.get_allocator(); }

public:
	//ctor
	//默认构建一个size为100的hashtable
	hash_multiset() :rep(100, hasher(), key_equal(), allocator_type()) {}
	explicit hash_multiset(size_type n) :rep(n, hasher(), key_equal(), allocator_type()) {}
	hash_multiset(size_type n, const hasher& hf) :rep(n, hf, key_equal(), allocator_type()) {}
	hash_multiset(size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		:rep(n, hf, eql, a) {}

	//与hash_multiset的不同之处，调用insert_equal
	template <class InputIterator>
	hash_multiset(InputIterator f, InputIterator l)
		: rep(100, hasher(), key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	template <class InputIterator>
	hash_multiset(InputIterator f, InputIterator l, size_type n)
		: rep(n, hasher(), key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	template <class InputIterator>
	hash_multiset(InputIterator f, InputIterator l, size_type n, const hasher& hf)
		: rep(n, hf, key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	template <class InputIterator>
	hash_multiset(InputIterator f, InputIterator l, size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		: rep(n, hf, eql, a) {
		rep.insert_equal(f, l);
	}

	//C API
	hash_multiset(const value_type* f, const value_type* l)
		:rep(100, hasher(), key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	hash_multiset(const value_type* f, const value_type* l, size_type n)
		:rep(n, hasher(), key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	hash_multiset(const value_type* f, const value_type* l, size_type n, const hasher& hf)
		:rep(n, hf, key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	hash_multiset(const value_type* f, const value_type* l, size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		:rep(n, hf, eql, a) {
		rep.insert_equal(f, l);
	}

	hash_multiset(const_iterator f, const_iterator l)
		:rep(100, hasher(), key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	hash_multiset(const_iterator f, const_iterator l, size_type n)
		:rep(n, hasher(), key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	hash_multiset(const_iterator f, const_iterator l, size_type n,
		const hasher& hf)
		:rep(n, hf, key_equal(), allocator_type()) {
		rep.insert_equal(f, l);
	}
	hash_multiset(const_iterator f, const_iterator l, size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		:rep(n, hf, eql, a) {
		rep.insert_equal(f, l);
	}


public:
	size_type size() const { return rep.size(); }
	size_type max_size() const { return rep.max_size(); }
	bool empty() const { return rep.empty(); }
	void swap(hash_multiset& rhs) { rep.swap(rhs.rep); }
	friend bool operator== <>(const hash_multiset&, const hash_multiset&);
	iterator begin() const { return rep.begin(); }
	iterator end() const { return rep.end(); }

public:
	//multi insert必然成功，不必返回pair
	iterator insert(const value_type& obj) {
		return rep.insert_equal(obj);
	}
	template <class InputIterator>
	void insert(InputIterator f, InputIterator l) {
		rep.insert_equal(f, l);
	}
	void insert(const value_type* f, const value_type* l) {
		rep.insert_equal(f, l);
	}
	void insert(const_iterator f, const_iterator l) {
		rep.insert_equal(f, l);
	}
	iterator insert_noresize(const value_type& obj) {
		return rep.insert_equal_noresize(obj);
	}

	size_type erase(const key_type& key) { return rep.erase(key); }
	void erase(iterator it) { rep.erase(it); }
	void erase(iterator f, iterator l) { rep.erase(f, l); }

	void clear() { rep.clear(); }

	iterator find(const key_type& key) const { return rep.find(key); }

	size_type count(const key_type& key) const { return rep.count(key); }

	pair<iterator, iterator> equal_range(const key_type& key) const {
		return rep.equal_range(key);
	}

public:
	void resize(size_type hint) { rep.resize(hint); }
	size_type bucket_count() const { return rep.bucket_count(); }
	size_type max_bucket_count() const { return rep.max_bucket_count(); }
	size_type elems_in_bucket(size_type n) const {//n号bucket的元素个数
		return rep.elems_in_bucket(n);
	}
};

template <class Value, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_multiset<Value, HashFcn, EqualKey, Alloc>& lhs, const hash_multiset<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.rep == rhs.rep;
}