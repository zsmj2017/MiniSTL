#pragma once

#include <cstddef> // ptrdiff_t
#include "allocator.h"
#include "uninitialized.h"
#include "stl_iterator.h"

// use sub_allocator as default allocator
template<class T,class Alloc = simpleAlloc<T> >
class vector {
public:
	template<class T, class Alloc>
	friend bool operator == (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);
	template<class T, class Alloc>
	friend bool operator != (const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs);

public:// alias declarartions
	using value_type = T;
	using pointer = value_type* ;
	using iterator = value_type* ;// iterator is raw pointer
	using const_iterator = value_type* ;
	using r_iterator = reverse_iterator<iterator>;
	using const_r_iterator = reverse_iterator<const_iterator>;
	using reference = value_type& ;
	using const_reference = const value_type& ;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

private: // allocate and construct aux functions 
	using data_allocator = Alloc;

	iterator start;
	iterator finish;
	iterator end_of_storage;

	void insert_aux(iterator position, const value_type& value);

	void deallocate() {
		if (start) data_allocator::deallocate(start, end_of_storage - start);
	}

	void fill_initialize(size_type n, const value_type& value) {
		start = allocate_and_fill(n, value);
		finish = start + n;
		end_of_storage = finish;
	}

	iterator allocate_and_fill(size_type n, const value_type& value) {
		iterator result = data_allocator::allocate(n);
		uninitialized_fill_n(result, n, value);
		return result;
	}

	template<class InputIterator>
	iterator allocate_and_copy(InputIterator first, InputIterator last) {
		start = data_allocator::allocate(last - first);
		finish = uninitialized_copy(first, last, start);
		end_of_storage = finish;
		return start;
	}

	void destroy_and_deallocate() {
		destroy(start, finish);
		deallocate();
	}

	void swap(vector&);

public:
	// ctor && dtor
	vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
	explicit vector(size_type n) { fill_initialize(n, value_type()); }
	vector(size_type n, const value_type &value) { fill_initialize(n, value); }
	template<class InputIterator>
	vector(InputIterator first, InputIterator last) { allocate_and_copy(first, last); }
	vector(std::initializer_list<T>);
	vector(const vector&);
	vector(vector&&) noexcept;

	~vector() {
		destroy(start, finish);//全局函数，见allocator
		deallocate();
	}

public:
	vector& operator=(const vector&);
	vector& operator=(std::initializer_list<value_type>);
	vector& operator=(vector&&) noexcept;

public:
	//静态可写接口
	iterator begin() { return start; }
	iterator end() { return finish; }
	r_iterator rbegin() { return r_iterator(start); }
	r_iterator rend() { return r_iterator(end); }
	reference operator[](const size_type n) { return *(start + n); }
	reference front() { return *begin(); }
	reference back() { return *(end() - 1); }

public:
	//静态只读接口
	const_iterator begin() const { return start; }
	const_iterator end() const { return end; }
	const_iterator cbegin() const { return start; }
	const_iterator cend() const { return end; }
	const_r_iterator crbegin() const { return const_r_iterator(start); }
	const_r_iterator crend() const { return const_r_iterator(end); }
	const_reference operator[](const size_type n) const  { return *(start + n); }
	size_type size() const { return static_cast<size_type>(finish - start); }
	size_type capacity() const { return static_cast<size_type>(end_of_storage - start); }
	bool empty() const { return start == finish; }
	bool operator== (const vector&) const;
	bool operator!= (const vector&) const;

public:
	//动态接口
	void push_back(const value_type&value) {
		if (finish != end_of_storage) {
			construct(finish, value);//全局函数
			++finish;
		}
		else
			insert_aux(end(), value);
	}

	void pop_back() {
		--finish;
		destroy(finish);
	}

	iterator erase(iterator first, iterator last) {
		iterator i = copy(last, finish, first);
		destroy(i, finish);// 此时i即等价于new_finish
		finish -= (last - first);
		return first;
	}

	iterator erase(iterator position) {
		if (position + 1 != end()) // 除却尾端节点外均需复制
			copy(position + 1, finish, position);
		pop_back();
		return position;
	}

	void resize(size_type new_size, const value_type& value) {
		if (new_size < size()) {
			erase(begin() + new_size, end());
		}
		else
			insert(end(), new_size - size(), value);
	}

	void resize(size_type new_size) {
		resize(new_size, value_type());
	}

	void reserve(size_type new_capacity) {
		if (new_capacity <= capacity()) return;
		T* new_start = data_allocator::allocate(new_capacity);
		T* new_finish = uninitialized_copy(start, finish, new_start);
		destroy_and_deallocate();
		start = new_start;
		finish = new_finish;
		end_of_storage = start + new_capacity;
	}

	void insert(iterator, size_type, const value_type&);
	iterator insert(iterator, const value_type&);

	void shrink_to_fit() { vector temp(*this); swap(temp); }
};

template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const value_type& value){
	if (finish != end_of_storage) {
		//当前存在备用空间
		construct(finish, *(finish - 1));// 以最后一个元素为初值构造元素于finish
		++finish;
		value_type value_copy = value;// STL copy in copy out
		// copy_backward needs _SCL_SECURE_NO_WARNINGS
		std::copy_backward(position, finish - 2, finish - 1);//将[pos,finish-2)copy至finish-1处（finish-1为目的终点）
		*position = value_copy;
	}
	else {
		//扩容
		const size_type old_size = size();
		const size_type new_size = old_size ? 2 * old_size : 1;//2倍大小
		iterator new_start = data_allocator::allocate(new_size);
		iterator new_finish = new_start;
		try{
			new_finish = uninitialized_copy(start,position,new_start);//复制前半段
			construct(new_finish, value);
			++new_finish;
			new_finish = uninitialized_copy(position, finish, new_finish);//复制后半段
		}
		catch(std::exception&){
			//commit or rollback
			destroy(new_start, new_finish);
			data_allocator::deallocate(new_start, new_size);
			throw;
		}
		//释放原有vector
		destroy_and_deallocate();
		//调整迭代器指向新vector
		start = new_start;
		finish = new_finish;
		end_of_storage = new_start + new_size;
	}
}

template<class T, class Alloc>
inline void vector<T, Alloc>::swap(vector &rhs){
	using std::swap;
	swap(start, rhs.start);
	swap(finish, rhs.finish);
	swap(end_of_storage, rhs.end_of_storage);
}

template<class T, class Alloc>
inline vector<T, Alloc>::vector(const vector &rhs){
	allocate_and_copy(rhs.start, rhs.finish);
}

template<class T, class Alloc>
inline vector<T, Alloc>::vector(vector &&rhs) noexcept{
	start = rhs.start;
	finish = rhs.finish;
	end_of_storage = rhs.end_of_storage;
	rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
}

template<class T, class Alloc>
inline vector<T, Alloc>& vector<T, Alloc>::operator=(const vector &rhs){
	vector temp(rhs);
	swap(temp);
	return *this;
}

template<class T, class Alloc>
inline vector<T,Alloc> & vector<T, Alloc>::operator=(std::initializer_list<value_type> il){
	destroy_and_deallocate();
	start = allocate_and_copy(il.begin(), il.end());
	finish = end_of_storage = start + (il.end() - il.begin());
}

template<class T, class Alloc>
inline vector<T, Alloc> & vector<T, Alloc>::operator=(vector &&rhs) noexcept{
	if (this != &rhs) {
		destroy_and_deallocate();
		start = rhs.start;
		finish = rhs.finish;
		end_of_storage_= rhs.end_of_storage;
		rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
	}
	return *this;
}

template<class T, class Alloc>
bool vector<T, Alloc>::operator==(const vector &rhs) const{
	if (size() != rhs.size()) {
		return false;
	}
	else {
		iterator ptr1 = start;
		iterator ptr2 = rhs.start;
		for (; ptr1 != finish && ptr2 != rhs.finish; ++ptr1, ++ptr2)
			if (*ptr1 != *ptr2)
				return false;
		return true;
	}
}

template<class T, class Alloc>
inline bool vector<T, Alloc>::operator!=(const vector &rhs) const{
	return !(operator==(rhs));
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const value_type & value){
	if (n) {
		if (static_cast<size_type>(end_of_storage - finish) >= n) {
			//备用空间充足
			value_type value_copy = value;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				//插入点后元素个数m>=插入元素个数n
				uninitialized_copy(finish - n, finish, finish);//先复制后n个元素
				finish += n;
				// copy_backward needs _SCL_SECURE_NO_WARNINGS
				std::copy_backward(position, old_finish - n, old_finish);//复制m-n个元素
				fill(position, position + n, value_copy);
			}
			else {
				uninitialized_fill_n(finish, n - elems_after, value_copy);//以m-n个value填充末尾
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);//将m个填充至最末尾
				finish += elems_after;
				fill(position, old_finish,value_copy);//补足m
			}
		}
		else {
			//需要扩容
			const size_type old_size = size();
			const size_type new_size = old_size + (old_size>n?old_size:n);
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try{
				new_finish = uninitialized_copy(start,position,new_start);
				new_finish = uninitialized_fill_n(new_finish, n, value);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch(std::exception&){
				destroy(new_start, new_finish);
				data_allocator::deallocate(new_start, new_size);
				throw;
			}
			destroy_and_deallocate();
			start = new_start;
			finish = new_finish;
			end_of_storage = new_start + new_size;
		}
	}
}

template<class T, class Alloc>
inline typename vector<T,Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &value){
	difference_type diff = position - begin();
	insert(position, 1, value);
	return begin() + diff;
}

template<class T, class Alloc>
inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs){
	return lhs.operator==(rhs);
}

template<class T, class Alloc>
inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs){
	return !(lhs==rhs);
}

template<class T, class Alloc>
inline vector<T, Alloc>::vector(std::initializer_list<T> il){
	start = allocate_and_copy(il.begin(), il.end());
	finish = end_of_storage = start +(il.end() - il.begin());
}
