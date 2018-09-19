#pragma once
#include "stl_deque.h"

namespace MiniSTL {

template <class T, class Sequence = deque<T> >
class stack {
	// friend declarations
	friend bool operator== <> (const stack&, const stack&);
	friend bool operator!= <> (const stack&, const stack&);

public:
	using value_type = typename Sequence::value_type;
	using size_type = typename Sequence::size_type;
	using reference = typename Sequence::reference;
	using const_reference = typename Sequence::const_reference;

private:// data
	Sequence c;

public:// getter
	bool empty() const noexcept { return c.empty(); }
	size_type size() const noexcept{ return c.size(); }
	const_reference top() const noexcept { return c.back(); }

public:// setter
	reference top() { return c.back(); }

public:// push && pop
	void push(const value_type& value) { c.push_back(value); }
	void pop() { c.pop_back(); }
};

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return lhs.c == rhs.c;
}

template <class T, class Sequence>
bool operator!=(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs){
	return !(lhs.c == rhs.c);
}

}