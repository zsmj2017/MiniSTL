#pragma once

#include <cstddef> // ptrdiff_t

//alloc为SGI STL默认空间配置器
template<class T,class Alloc = alloc>
class vector {
public:
	//嵌套类型别名定义
	using value_type = T;
	using pointer = value_type* ;
	using iterator = value_type* ;//vector迭代器为普通指针
	using reference = value_type& ;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

protected:
	//内存配置、批量构造函数

	//以simple_alloc为空间配置器
	using data_allocator = simple_alloc<value_type, Alloc>;

	iterator start;//目前已使用的头部
	iterator finish;//已使用尾部
	iterator end_of_stroage;//可用空间尾部

	void insert_aux(iterator position, const T& x);

	//释放本vector所占用的内存
	void deallocate() {
		if (start) {
			data_allocator::deallocate(start, end_of_stroage - start);
		}
	}

	//获取并初始化内存区域
	void fill_initialize(size_type n, const T& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_stroage = finish;
	}

	//配置空间并填满内容(具体实现见allocator）
	iterator allocate_and_fill(size_type n, const value_type& value) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, x);
		return result;
	}

public:
	//构造与析构函数
	vector() :start(nullptr), end(nullptr), end_of_stroage(nullptr) {}
	vector(size_type n, const value_type &value) { fill_initialize(n, value); }
	vector(long n, const value_type &value) { fill_initialize(n, value); }
	explicit vector(size_type n) { fill_initialize(n, value_type()); }

	~vector() {
		destory(start, finish);//全局函数，见allocator
		deallocate();
	}

public:
	//静态可写接口
	iterator begin() { return start; }
	iterator end() { return finish; }
	reference operator[](size_type n) { return *(begin() + n); }
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }

public:
	//静态只读接口
	size_type size() const { return static_cast<size_type>(end() - begin()); }
	size_type capacity() const { return static_cast<size_type>(end_of_stroage - begin()); }
	bool empty() const { return begin() == end(); }

public:
	//动态接口
	void push_back(const value_type&value) {
		if (finish != end_of_stroage) {
			construct(finish, x);//全局函数
			++finish;
		}
		else
			insert_aux(end(), value);
	}

	void pop_back() {
		--finish;
		destory(finish);
	}

	iterator erase(iterator position) {
		if (position + 1 != end()) {
			copy(position + 1, finish, position);
			pop_back(finish);
			return position;
		}
	}

	void resize(size_type new_size, const value_type& value) {
		if (new_size < size()) {
			erase(begin() + new_size, end());
		}
		else
			insert(end(),new_size-size(),value)
	}

	void resize(size_type new_size) {
		resize(new_size, value_type());
	}
};