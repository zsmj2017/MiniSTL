#pragma once

template <class T,class Sequence = vector<T>,class Compare = less<typename Sequence::value_type> >
class priority_queue {
public:
	using value_type = Sequence::value_type;
	using size_type = Sequence::size_type;
	using reference = Sequence::reference;
	using const_reference = Sequence::const_reference;

protected:
	Sequence c;
	Compare comp;//比较器

public:
	priority_queue() : c() {}
	explicit priority_queue(const Compare& compare) : c(), comp(compare) {}
	//此处make_heap为泛型算法，可接受比较器，在前一节撰写的heap算法中并不支持
	template <class InputIterator>
	priority_queue(InputIterator first, InputIterator last, const Compare& compare)
		: c(first, last), comp(compare) {
		make_heap(c.begin(), c.end(), comp);
	}
	template <class InputIterator>
	priority_queue(InputIterator first, InputIterator last)
		: c(first, last) {
		make_heap(c.begin(), c.end(), comp);
	}

public:
	bool empty() const { return c.empty(); }
	size_type size() const { return c.size(); }
	const_reference top() const { return c.front(); }
	void push(const value_type& value) {
		try{
			c.push_back(x);
			push_heap(c.begin(), c.end(), comp);
		}
		catch{
			c.clear();
		}
	}
	void pop() {
		try {
			pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
		catch{
			c.clear();
		}
	}
};