#pragma once

template <class Key,class T, class HashFcn, class EqualKey class Alloc>
class hash_map;

template <class Key,class T, class HashFcn, class EqualKey, class Alloc>
inline bool operator==(const hash_map<Value, HashFcn, EqualKey, Alloc>& lhs, const hash_map<Value, HashFcn, EqualKey, Alloc>& rhs);

template <class Key, class T, class HashFcn = hash<Value>, class EqualKey = equal_to<Value>, class Alloc = alloc >
class hash_map{
private:
	using ht = hashtable<pair<const Key,T>, Key, HashFcn, select1st<pair<const Key,T>, EqualKey, Alloc>;
	ht rep;//底层数据结构

public:
	using key_type = typename ht::key_type;
	using data_type = T;
	using mapped_type = T;
	using value_type = typename ht::value_type;
	using hasher = typename ht::hasher;
	using key_equal = typename ht::key_equal;

	using pointer = typename ht::pointer;
	using const_pointer = typename ht::const_pointer;
	using reference = typename ht::reference;
	using const_reference = typename ht::const_reference;
	using iterator = typename ht::iterator;
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
	hash_map():rep(100, hasher(), key_equal(), allocator_type()) {}
	explicit hash_map(size_type n) :rep(n, hasher(), key_equal(), allocator_type()) {}
	hash_map(size_type n, const hasher& hf) :rep(n, hf, key_equal(), allocator_type()) {}
	hash_map(size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		:rep(n, hf, eql, a) {}

	//以某范围的迭代器初构造hash_map，调用insert_unique
	template <class InputIterator>
	hash_map(InputIterator f, InputIterator l)
		: rep(100, hasher(), key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	template <class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n)
		: rep(n, hasher(), key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	template <class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf)
		: rep(n, hf, key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	template <class InputIterator>
	hash_map(InputIterator f, InputIterator l, size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		: rep(n, hf, eql, a) {
		rep.insert_unique(f, l);
	}

	//C API
	hash_map(const value_type* f, const value_type* l)
		:rep(100, hasher(), key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	hash_map(const value_type* f, const value_type* l, size_type n)
		:rep(n, hasher(), key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	hash_map(const value_type* f, const value_type* l, size_type n, const hasher& hf)
		:rep(n, hf, key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	hash_map(const value_type* f, const value_type* l, size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		:rep(n, hf, eql, a) {
		rep.insert_unique(f, l);
	}

	hash_map(const_iterator f, const_iterator l)
		:rep(100, hasher(), key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	hash_map(const_iterator f, const_iterator l, size_type n)
		:rep(n, hasher(), key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	hash_map(const_iterator f, const_iterator l, size_type n,
		const hasher& hf)
		:rep(n, hf, key_equal(), allocator_type()) {
		rep.insert_unique(f, l);
	}
	hash_map(const_iterator f, const_iterator l, size_type n, const hasher& hf, const key_equal& eql, const allocator_type& a = allocator_type())
		:rep(n, hf, eql, a) {
		rep.insert_unique(f, l);
	}

public:
	size_type size() const { return rep.size(); }
	size_type max_size() const { return rep.max_size(); }
	bool empty() const { return rep.empty(); }
	void swap(hash_map& rhs) { rep.swap(rhs.rep); }
	friend bool operator== <>(const hash_map&, const hash_map&);
	iterator begin() const { return rep.begin(); }
	iterator end() const { return rep.end(); }

public:
	pair<iterator, bool> insert(const value_type& obj) {
		return rep.insert_unique(obj);
	}
	template <class InputIterator>
	void insert(InputIterator f, InputIterator l) {
		rep.insert_unique(f, l);
	}
	void insert(const value_type* f, const value_type* l) {
		rep.insert_unique(f, l);
	}
	void insert(const_iterator f, const_iterator l) {
		rep.insert_unique(f, l);
	}
	pair<iterator, bool> insert_noresize(const value_type& obj) {
		 return rep.insert_unique_noresize(obj);
	}

	data_type& operator[](const key_type& key) {
		return rep.find_or_insert(value_type(key, data_type())).second;//见Effective STL Item24
	}

	size_type erase(const key_type& key) { return rep.erase(key); }
	void erase(iterator it) { rep.erase(it); }
	void erase(iterator f, iterator l) { rep.erase(f, l); }

	void clear() { rep.clear(); }

	iterator find(const key_type& key) const { return rep.find(key); }

	size_type count(const key_type& key) const { return rep.count(key); }

	pair<iterator, iterator> equal_range(const key_type& key) {
		return rep.equal_range(key);
	}

	pair<const_iterator, const_iterator> equal_range(const key_type& key) const {
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
inline bool operator==(const hash_map<Value, HashFcn, EqualKey, Alloc>& lhs, const hash_map<Value, HashFcn, EqualKey, Alloc>& rhs) {
	return lhs.rep == rhs.rep;
}