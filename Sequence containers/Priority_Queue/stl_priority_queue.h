#pragma once
#include "stl_vector.h"
#include "stl_function.h"
#include "heap_algorithm.h"

namespace MiniSTL {

template <class T, class Sequence = vector<T>, class Compare = less<typename Sequence::value_type> >
class priority_queue {
public:// alias declarations
	using value_type = typename Sequence::value_type;
	using size_type = typename Sequence::size_type;
	using reference = typename Sequence::reference;
	using const_reference = typename Sequence::const_reference;
	// no iterators

private:// data
	Sequence c;
	Compare comp;

public:// ctor && dtor
	priority_queue() : c() {}
	explicit priority_queue(const Compare& compare) : c(), comp(compare) {}
	priority_queue(std::initializer_list<T> il, const Compare& compare=Compare()):c(il),comp(compare) {
		make_heap(c.begin(), c.end(), comp);
	}
	template <class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& compare=Compare())
		: c(first, last), comp(compare) {
		make_heap(c.begin(), c.end(), comp);
	}
	priority_queue(const priority_queue& rhs) :c(rhs.c), comp(rhs.comp) {}
	priority_queue(priority_queue&& rhs) noexcept :c(std::move(rhs.c)), comp(std::move(rhs.comp)) {}
	~priority_queue() = default;

public:// operator=
	priority_queue& operator=(const priority_queue& rhs) {
		c = rhs.c;
		comp = rhs.comp;
		return *this;
	}
	
	priority_queue& operator=(priority_queue&& rhs) noexcept{
		c = std::move(rhs.c);
		comp = std::move(rhs.comp);
		return *this;
	}

public: // getter
	bool empty() const noexcept{ return c.empty(); }
	size_type size() const noexcept{ return c.size(); }
	const_reference top() const noexcept{ return c.front(); }

public: // push && pop
	void push(const value_type& value) {
		try {
			c.push_back(value);
			push_heap(c.begin(), c.end(), comp);
		}
		catch(std::exception&){
			c.clear();
		}
	}
	void pop() {
		try {
			pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
		catch (std::exception&) {
			c.clear();
		}
	}
};
}