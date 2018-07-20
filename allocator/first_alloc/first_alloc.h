#pragma once
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

namespace lxalloc {

	template<class T>
	inline T* _allocate(ptrdiff_t size, T*) {
		std::set_new_handler(nullptr);
		T* tmp = static_cast<T*>(::operator new(static_cast<size_t>(size * sizeof(T)));
		if (tmp == nullptr) {
			std::cerr << "out of memory";
			exit(-1);
		}
		return tmp;
	}

	template<class T>
	inline void _deallocate(T* buffer) {
		::operator delete(buffer);
	}

	template<class T1, class T2>
	inline _construct(T1* p, const T2& value) {
		new(p) T1(value);
	}

	template<class T>
	ininle _destory(T* p) {
		p->~T();
	}

	template<class T>
	class allocator {
	public:
		typedef T value_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;

		template <class U>
		struct rebind {
			typedef allocator<U> other;
		};

		pointer allocate(size_type n, const void* hint = nullptr) {
			return _allocate(static_cast<difference_type>(n), nullptr);
		}

		void deallocate(pointer p, size_type n) {
			_deallocate(p);
		}

		void construct(pointer p, const T& value) {
			_construct(p, value);
		}

		void destroy(pointer p) {
			_destory(p);
		}
		
		pointer address(reference x) {
			return static_cast<pointer>(&x);
		}

		const_pointer const_address(reference x) {
			return static_cast<const_pointer>(&x);
		}

		size_type max_size() const {
			return size_type(UINT_MAX / sizeof(T));
		}
	};
}