#pragma once

//默认以deque实现
//stack并非container，而是adapter

//Forward declarations of operators == and <, needed for friend declarations.

template <clas T,class Sequence>
class stack;

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>&, const stack<T, Sequence>&);

template<class T, class Sequence = deque<T> >
inline bool operator<(const stack<T, Sequence>&, const stack<Sequence>&)

template <class T,class Sequence = deque<T> >
class stack {
	//friend template
	friend bool operator== <> (const stack&, const stack&);
	friend bool operator< <> (const stack&, const stack&);
public:
	using value_type = Sequence::value_type;
	using size_type = Sequence::size_type;
	using reference = Sequence::reference;
	using const_reference = Sequence::const_reference;
protected:
	Sequence c;//底层容器
public:
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	reference top() { return c.back(); }
	const_reference top() const { return c.back(); }
	void push(const value_type& value) { c.push_back(value); }
	void pop() { c.pop_back(); }
};

template <class T, class Sequence>
bool operator==(const stack<T, Sequence>& lhs, const stack<T, Sequence>& rhs) {
	return lhs.c == rhs.c;
}

template<class T, class Sequence = deque<T> >
inline bool operator<(const stack<T, Sequence> &, const stack<Sequence> &) {
	return lhs.c < rhs.c;
}