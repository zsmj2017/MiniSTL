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

	//以simple_alloc为空间配置器,便于以元素大小进行配置空间
	using data_allocator = simple_alloc<value_type, Alloc>;

	iterator start;
	iterator finish;
	iterator end_of_stroage;//可用空间尾部

	//插入接口，内含扩容操作
	void insert_aux(iterator position, const value_type& value);

	//释放本vector所占用的内存
	void deallocate() {
		if (start) {
			data_allocator::deallocate(start, end_of_stroage - start);
		}
	}

	//获取并初始化内存区域
	void fill_initialize(size_type n, const value_type& value) {
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

	iterator erase(iterator first, iterator last) {
		iterator i = copy(last, finish, first);
		destory(i, finish);//此时i即等价于new_finish
		finish -= (last - first);
		return first;
	}

	iterator erase(iterator position) {
		if (position + 1 != end()) //除却尾端节点外均需复制
			copy(position + 1, finish, position);
		pop_back();
		return position;
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

	void insert(iterator position, size_type n, const value_type& value);
};

template<class T, class Alloc>
inline void vector<T, Alloc>::insert_aux(iterator position, const value_type& value){
	if (finish != end_of_stroage) {
		//当前存在备用空间
		construct(finish, *(finish - 1));//以最后一个元素为初值构造元素于finish
		++finish;
		value_type value_copy = value;//STL copy in copy out
		copy_backward(position, finish - 2, finish - 1);//将[pos,finish-2)copy至finish-1处（finish-1为目的终点）
		*position = value_copy;
	}
	else {
		//扩容
		const size_type old_size = size();
		const size_type new_size = old_size ? 2 * old_size : 1;//2倍大小
		iterator new_start = data_allocator::allocate(new_size);
		iterator new_finish = newstart;
		try{
			new_finish = unitialized_copy(start,position,new_start);//复制前半段
			construct(new_finish, value);
			++new_finish;
			new_finish = unitialized_copy(position, finish, new_finish);//复制后半段
		}
		catch{
			//commit or rollback
			destory(new_start, new_finish);
			data_allocator::deallocate(new_start, new_size);
			throw;
		}
		//释放原有vector
		destory(begin(), end());
		deallocate();
		//调整迭代器指向新vector
		start = new_start;
		finish = new_finish;
		end_of_stroage = new_start + new_size;
	}
}

template<class T, class Alloc>
inline void vector<T, Alloc>::insert(iterator position, size_type n, const value_type & value){
	if (n) {
		if (static_cast<size_type>(end_of_stroage - finish) >= n) {
			//备用空间充足
			value_type value_copy = value;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				//插入点后元素个数m>=插入元素个数n
				unitialized_copy(finish - n, finish, finish);//先复制后n个元素
				finish += n;
				copy_backward(position, old_finish - n, old_finish);//复制m-n个元素
				fill(position, position + n, value_copy);
			}
			else {
				unitialized_fill_n(finish, n - eles_after, value_copy);//以m-n个value填充末尾
				finish += n - elems_after;
				unitialized_copy(position, old_finish, finish);//将m个填充至最末尾
				finish += elems_after;
				fill(position, old_finish,value_copy);//补足m
			}
		}
		else {
			//需要扩容
			const size_type old_size = size();
			const size_type new_size = oldsize + max(oldsize, n);
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = newstart;
			__STL_TRY{
				new_finish = unitialized_copy(start,position,new_start);
				new_finish = unitialized_fill_n(new_finish, n, value);
				new_finish = unitialized_copy(position, finish, new_finish);
			}
#ifdef __STL_USE_EXCEPTIONS
			catch{
				destory(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}
#endif /* __STL_USE_EXCEPTIONS*/
			destory(begin(), end());
			deallocate();
			start = new_start;
			finish = new_finish;
			end_of_stroage = new_start + new_size;
		}
	}
}
