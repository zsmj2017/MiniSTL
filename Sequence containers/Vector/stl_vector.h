#pragma once

#include <cstddef> // ptrdiff_t
#include "allocator.h"
#include "uninitialized.h"

namespace MiniSTL {
// use sub_allocator as default allocator
template<class T, class Alloc = simpleAlloc<T> >
class vector {
public:
	template<class _T, class _Alloc>
	friend bool operator == (const vector<_T, _Alloc>&, const vector<_T, _Alloc>&);
	template<class _T, class _Alloc>
	friend bool operator != (const vector<_T, _Alloc>&, const vector<_T, _Alloc>&);

public:// alias declarartions
	using value_type = T;
	using pointer = value_type * ;
	using iterator = value_type * ;// iterator is raw pointer
	using const_iterator = value_type * ;
	using reverse_iterator = MiniSTL::reverse_iterator<iterator>;
	using const_reverse_iterator = MiniSTL::reverse_iterator<const_iterator>;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

private: // iterator to indicate the vector's memory location
	iterator start;
	iterator finish;
	iterator end_of_storage;

private: // allocate and construct aux functions 
	using data_allocator = Alloc;

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

	void deallocate() noexcept {
		if (start) data_allocator::deallocate(start, end_of_storage - start);
	}

	void destroy_and_deallocate()noexcept {
		destroy(start, finish); // destroy in "construct.h"
		deallocate();
	}

private: // aux_interface
	void swap(vector&) noexcept;
	void insert_aux(iterator position, const value_type& value);

public:// ctor && dtor
	vector() :start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
	explicit vector(size_type n) { fill_initialize(n, value_type()); }
	vector(size_type n, const value_type &value) { fill_initialize(n, value); }
	template<class InputIterator>
	vector(InputIterator first, InputIterator last) { allocate_and_copy(first, last); }
	vector(std::initializer_list<T>);
	vector(const vector& rhs) { allocate_and_copy(rhs.start, rhs.finish); }
	vector(vector&&) noexcept;

	~vector() {
		destroy(start, finish); // destory in "construct.h"
		deallocate();
	}

public:// copy assignment operator
	vector & operator=(const vector&);
	vector& operator=(std::initializer_list<value_type>);
	vector& operator=(vector&&) noexcept;

public: // getter
	const_iterator begin() const noexcept { return start; }
	const_iterator end() const noexcept { return finish; }
	const_iterator cbegin() const noexcept { return start; }
	const_iterator cend() const noexcept { return finish; }
	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(finish); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(start); }
	const_reference front() const noexcept { return *begin(); }
	const_reference back() const noexcept { return *(end() - 1); }
	const_reference operator[](const size_type n) const noexcept { return *(start + n); }
	size_type size() const noexcept { return static_cast<size_type>(finish - start); }
	size_type capacity() const noexcept { return static_cast<size_type>(end_of_storage - start); }
	bool empty() const noexcept { return start == finish; }

public: // setter
	iterator begin() noexcept { return start; }
	iterator end() noexcept { return finish; }
	reverse_iterator rbegin() noexcept { return reverse_iterator(finish); }
	reverse_iterator rend() noexcept { return reverse_iterator(start); }
	reference operator[](const size_type n) noexcept { return *(start + n); }
	reference front() noexcept { return *begin(); }
	reference back() noexcept { return *(end() - 1); }

public: //  interface for size and capacity
	void resize(size_type, const value_type&);
	void resize(size_type new_size) { resize(new_size, value_type()); }
	void reserve(size_type);
	void shrink_to_fit() noexcept { vector temp(*this); swap(temp); }

public: // compare operator(member function)
	bool operator== (const vector&) const noexcept;
	bool operator!= (const vector& rhs) const noexcept { return !(*this == rhs); }

public: // interface for back operation
	void push_back(const value_type&);
	void pop_back() {--finish;destroy(finish);}

public: // interface for insert and erase
	iterator erase(iterator, iterator);
	iterator erase(iterator position) { return erase(position, position + 1); }
	void clear() { erase(begin(), end()); }
	void insert(iterator, size_type, const value_type&);
	iterator insert(iterator, const value_type&);
};

template<class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const value_type& value) {
	if (finish != end_of_storage) {// needn't expand
		construct(finish, *(finish - 1));
		++finish;
		value_type value_copy = value;// STL copy in copy out
		// TODO:
		// copy_backward needs _SCL_SECURE_NO_WARNINGS
		std::copy_backward(position, finish - 2, finish - 1);
		*position = value_copy;
	}
	else {// expand
		const size_type old_size = size();
		const size_type new_size = old_size ? 2 * old_size : 1; // new_cap=2*old_cap
		iterator new_start = data_allocator::allocate(new_size);
		iterator new_finish = new_start;
		try {
			new_finish = uninitialized_copy(start, position, new_start);//复制前半段
			construct(new_finish, value);
			++new_finish;
			new_finish = uninitialized_copy(position, finish, new_finish);//复制后半段
		}
		catch (std::exception&) {
			//commit or rollback
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

template<class T, class Alloc>
inline void vector<T, Alloc>::swap(vector &rhs) noexcept {
	using std::swap;
	swap(start, rhs.start);
	swap(finish, rhs.finish);
	swap(end_of_storage, rhs.end_of_storage);
}

template<class T, class Alloc>
inline vector<T, Alloc>::vector(vector &&rhs) noexcept {
	start = rhs.start;
	finish = rhs.finish;
	end_of_storage = rhs.end_of_storage;
	rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
}

template<class T, class Alloc>
inline vector<T, Alloc>& vector<T, Alloc>::operator=(const vector &rhs) {
	vector temp(rhs);
	swap(temp);
	return *this;
}

template<class T, class Alloc>
inline vector<T, Alloc> & vector<T, Alloc>::operator=(std::initializer_list<value_type> il) {
	destroy_and_deallocate();
	start = allocate_and_copy(il.begin(), il.end());
	finish = end_of_storage = start + (il.end() - il.begin());
}

template<class T, class Alloc>
inline vector<T, Alloc> & vector<T, Alloc>::operator=(vector &&rhs) noexcept {
	if (this != &rhs) {
		destroy_and_deallocate();
		start = rhs.start;
		finish = rhs.finish;
		end_of_storage = rhs.end_of_storage;
		rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
	}
	return *this;
}

template<class T, class Alloc>
inline void vector<T, Alloc>::resize(size_type new_size, const value_type & value){
	if (new_size < size())
		erase(begin() + new_size, end());
	else
		insert(end(), new_size - size(), value);
}

template<class T, class Alloc>
inline void vector<T, Alloc>::reserve(size_type new_capacity){
	if (new_capacity <= capacity())
		return;
	T* new_start = data_allocator::allocate(new_capacity);
	T* new_finish = uninitialized_copy(start, finish, new_start);
	destroy_and_deallocate();
	start = new_start;
	finish = new_finish;
	end_of_storage = start + new_capacity;
}

template<class T, class Alloc>
bool vector<T, Alloc>::operator==(const vector &rhs) const noexcept{
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
inline void vector<T, Alloc>::push_back(const value_type & value){
	if (finish != end_of_storage) {
		construct(finish, value);
		++finish;
	}
	else
		insert_aux(end(), value);
}

template<class T, class Alloc>
inline typename vector<T,Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last){
	iterator i = copy(last, finish, first);
	destroy(i, finish);
	finish -= (last - first);
	return first;
}

template<class T, class Alloc>
void vector<T, Alloc>::insert(iterator position, size_type n, const value_type & value) {
	if (n) {
		if (static_cast<size_type>(end_of_storage - finish) >= n) {// needn't expand
			value_type value_copy = value;
			const size_type elems_after = finish - position;
			iterator old_finish = finish;
			if (elems_after > n) {
				uninitialized_copy(finish - n, finish, finish);
				finish += n;
				// copy_backward needs _SCL_SECURE_NO_WARNINGS
				std::copy_backward(position, old_finish - n, old_finish);
				std::fill(position, position + n, value_copy);
			}
			else {
				uninitialized_fill_n(finish, n - elems_after, value_copy);
				finish += n - elems_after;
				uninitialized_copy(position, old_finish, finish);
				finish += elems_after;
				std::fill(position, old_finish, value_copy);//补足m
			}
		}
		else { // expand
			const size_type old_size = size();
			const size_type new_size = old_size + (old_size > n ? old_size : n);
			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;
			try {
				new_finish = uninitialized_copy(start, position, new_start);
				new_finish = uninitialized_fill_n(new_finish, n, value);
				new_finish = uninitialized_copy(position, finish, new_finish);
			}
			catch (std::exception&) {
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
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(iterator position, const value_type &value) {
	difference_type diff = position - begin();
	insert(position, 1, value);
	return begin() + diff;
}

template<class T, class Alloc>
inline vector<T, Alloc>::vector(std::initializer_list<T> il) {
	start = allocate_and_copy(il.begin(), il.end());
	finish = end_of_storage = start + (il.end() - il.begin());
}

template<class T, class Alloc>
inline bool operator==(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return lhs.operator==(rhs);
}

template<class T, class Alloc>
inline bool operator!=(const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs) {
	return !(lhs == rhs);
}

}// endnamespace::MiniSTL
