#pragma once

//Forward declarations of operators == and <, needed for friend declarations.

template<class T,class Sequence>
class queue;

template <class T, class Sequence>
bool operator==(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs);

template <class T, class Sequence>
bool operator<(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs);

template <class T, class Sequence = deque<T> >
class queue {
	friend bool operator== <>(const queue& lhs, const queue& rhs);
	friend bool operator< <>(const queue& lhs, const queue& rhs);
public:
	using value_type = Sequence::value_type;
	using size_type = Sequence::size_type;
	using reference = Sequence::reference;
	using const_reference = Sequence::const_reference;
protected:
	Sequence c;
public:
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference front() { return c.front(); }
	const_reference front() const { return c.front(); }
	reference back() { return c.back(); }
	const_reference back() const { return c.back(); }
	void push(const value_type& value) { c.push_back(value); }
	void pop() { c.pop_front(); }
};

template <class T, class Sequence>
bool operator==(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs){
	return lhs.c == rhs.c;
}

template <class T, class Sequence>
bool operator<(const queue<T, Sequence>& lhs, const queue<T, Sequence>& rhs){
	return lhs.c < rhs.c;
}