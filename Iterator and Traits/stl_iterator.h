#pragma once

#include <cstddef> //ptrdiff_t定义
#include <iostream>

namespace MiniSTL {

//五种迭代器类型
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag :public input_iterator_tag {};
struct bidirectional_iterator_tag :public forward_iterator_tag {};
struct random_access_iterator_tag :public bidirectional_iterator_tag {};

//为了避免遗忘记录traits，此基类以供自定义iterator继承之
template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T * , class Reference = T & >
struct iterator {
	using iterator_category = Category;
	using value_type = T;
	using difference_type = Distance;
	using pointer = Pointer;
	using reference = Reference;
};

//traits 展现Iterator所有特性
//之所以不直接使用别名模板完全替代是因为存在偏特化版本，（似乎别名模板无法偏特化）
template <class Iterator>
struct iterator_traits {
	using iterator_category = typename Iterator::iterator_category;
	using value_type = typename Iterator::value_type;
	using difference_type = typename Iterator::difference_type;
	using pointer = typename Iterator::pointer;
	using reference = typename Iterator::reference;
};

//针对raw pointer设计的偏特化版本
template <class T>
struct iterator_traits<T*> {
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = T * ;
	using reference = T & ;
};

//针对pointer-to-const设计的偏特化版本
template <class T>
struct iterator_traits<const T*> {
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = const T*;
	using reference = const T&;
};

//以下为模仿C++14 type_traits_t而设定的别名模板
template<class Iterator>
using iterator_category_t = typename iterator_traits<Iterator>::iterator_category;

template<class Iterator>
using value_type_t = typename iterator_traits<Iterator>::value_type;

template<class Iterator>
using difference_type_t = typename iterator_traits<Iterator>::difference_type;

template<class Iterator>
using pointer_t = typename iterator_traits<Iterator>::pointer;

template<class Iterator>
using reference_t = typename iterator_traits<Iterator>::reference;


//以下为整组distance函数
template<class InputIterator>
inline difference_type_t<InputIterator> __distance(InputIterator first, InputIterator last, input_iterator_tag) {
	difference_type_t<InputIterator> n = 0;
	while (first != last)
		++first, ++n;
	return n;
}

template<class InputIterator>
inline difference_type_t<InputIterator> __distance(InputIterator first, InputIterator last, random_access_iterator_tag) {
	return first - last;
}

template<class InputIterator>
inline difference_type_t<InputIterator> distance(InputIterator first, InputIterator last) {
	return __distance(first, last, iterator_category_t<InputIterator>());
}

//以下为整组advance函数
template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_iterator_tag) {
	while (n--) ++i;
}

template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, bidirectional_iterator_tag) {
	if (n >= 0)
		while (n--) ++i;
	else
		while (n++) --i;
}

template<class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, random_access_iterator_tag) {
	i += n;
}

template<class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
	__advance(i, n, iterator_category_t<InputIterator>());
}

//以下为三种迭代器适配器
//insert,reverse,stream

//insert:back_insert,fornt_insert,insert
template <class Container>
class back_insert_iterator {
protected:
	Container * container;//底层容器

public:
	using iterator_category = output_iterator_tag;
	using value_tyep = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

public:
	explicit back_insert_iterator(Container& value) :container(value) {}
	back_insert_iterator& operator=(const typename Container::value_type& value) {
		container->push_back(value);//本质上是调用了push_back
		return *this;
	}

	//以下三个接口对back_insert_iterator无用，关闭功能(为什么不直接设为private？）
	back_insert_iterator operator*() { return *this; }
	back_insert_iterator operator++() { return *this; }
	back_insert_iterator operator++(int) { return *this; }
};

//以下是一个辅助函数，便于快速使用back_insert_iterator
template <class Container>
inline back_insert_iterator<Container> back_inserter(Container &x) {
	return back_insert_iterator<Container>(x);
}

//front_insert_iterator无法用于vector，因为后者不具备push_front
template <class Container>
class front_insert_iterator {
protected:
	Container * container;//底层容器

public:
	using iterator_category = output_iterator_tag;
	using value_tyep = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

public:
	explicit front_insert_iterator(Container& value) :container(value) {}
	front_insert_iterator& operator=(const typename Container::value_type& value) {
		container->push_front(value);
		return *this;
	}

	//以下三个接口对back_insert_iterator无用，关闭功能(为什么不直接设为private？）
	front_insert_iterator operator*() { return *this; }
	front_insert_iterator operator++() { return *this; }
	front_insert_iterator operator++(int) { return *this; }
};

template <class Container>
inline front_insert_iterator<Container> front_inserter(Container &x) {
	return front_insert_iterator<Container>(x);
}

template <class Container>
class insert_iterator {
protected:
	Container* container;//底层容器
	typename Container::iterator iter;

public:
	using iterator_category = output_iterator_tag;
	using value_tyep = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

public:
	insert_iterator(Container& value, typename Container::iterator i) :container(value), iter(i) {}
	insert_iterator& operator=(const typename Container::value_type& value) {
		container->insert(iter, value);//调用insert
		++iter;//保证insert_iterator永远与目标贴合
		return *this;
	}

	//以下三个接口对back_insert_iterator无用，关闭功能(为什么不直接设为private？）
	insert_iterator operator*() { return *this; }
	insert_iterator operator++() { return *this; }
	insert_iterator operator++(int) { return *this; }
};

template <class Container, class Iterator>
inline insert_iterator<Container> inserter(Container &x, Iterator i) {
	return insert_iterator<Container>(x, i);
}

template <class Iterator>
class reverse_iterator {
	template<class Iterator>
	friend bool operator == (const reverse_iterator<Iterator>&, const reverse_iterator<Iterator>&);
	template<class Iterator>
	friend bool operator != (const reverse_iterator<Iterator>&, const reverse_iterator<Iterator>&);

protected:
	Iterator current;//与之对应的正向迭代器

public:
	using iterator_category = iterator_category_t<Iterator>;
	using value_type = value_type_t<Iterator>;
	using difference_type = difference_type_t<Iterator>;
	using pointer = pointer_t<Iterator>;
	using reference = reference_t<Iterator>;

	using iterator_type = Iterator;//正向迭代器
	using self = reverse_iterator;//反向迭代器

public:
	reverse_iterator() {}
	explicit reverse_iterator(iterator_type value) :current(value) {}
	reverse_iterator(const self& value) :current(value.current) {}

	iterator_type base() const { return current; }
	reference operator*() const {
		Iterator temp = current;
		return *--temp;//对逆向迭代器的取值等价于对应的正向迭代器后退一个单位取值
	}

	pointer operator->() const { return &(operator*()); }

	self& operator++() {
		--current;
		return *this;
	}

	self operator++(int) {
		self temp = *this;
		--current;
		return temp;
	}

	self& operator--() {
		++current;
		return *this;
	}

	self operator--(int) {
		self temp = *this;
		++current;
		return temp;
	}

	self operator+(difference_type n) const {
		return self(current - n);
	}

	self& operator+=(difference_type n) const {
		current -= n;
		return *this;
	}

	self operator-(difference_type n) const {
		return self(current + n);
	}

	self& operator-=(difference_type n) const {
		current += n;
		return *this;
	}

	reference operator[](difference_type n) const { return *(*this + n); }

	bool operator==(const self& rhs) const { return current == rhs.current;}
	bool operator!=(const self& rhs) const { return !((*this) == rhs); }
};


template<class Iterator>
inline bool operator==(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs){
	return lhs.operator==(rhs);
}

template<class Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return !(lhs == rhs);
}

//stream:input_stream,output_stream

template <class T, class Distance = ptrdiff_t>
class istream_iterator {
protected:
	std::istream * stream;
	T value;
	bool end_marker;
	void read() {
		end_marker = (*stream) ? true : false;
		if (end_marker) *stream >> value;
		//完成输入后，stream状态可能发生了改变，再次判定
		end_marker = (*stream) ? true : false;
	}

public:
	using iterator_category = input_iterator_tag;
	using value_tyep = T;
	using difference_type = Distance;
	using pointer = const T*;//由于身为input_iterator，采用const较为保险
	using reference = const T&;

	istream_iterator() :stream(&std::cin), end_marker(false) {}
	istream_iterator(std::istream &s) :stream(&s) { read(); }

	reference operator*() const { return value; }
	pointer operator->() const { return &(operator*()); }

	istream_iterator& operator++() {
		read();
		return *this;
	}

	istream_iterator operator++(int) {
		istream_iterator temp = *this;
		read();
		return temp;
	}
};

template <class T>
class ostream_iterator {
protected:
	std::ostream * stream;
	const char* interval;//输出间隔符

public:
	using iterator_category = output_iterator_tag;
	using value_tyep = void;
	using difference_type = void;
	using pointer = void;
	using reference = void;

	ostream_iterator() :stream(&std::cin), interval(nullptr) {}
	ostream_iterator(std::ostream& s, const char* c) :stream(&s), interval(c) {}

	ostream_iterator& operator=(const T& value) {
		*stream << value;
		if (interval) *stream << interval;
		return *this;
	}

	ostream_iterator& operator*() { return *this; }
	ostream_iterator& operator++() { return *this; }
	ostream_iterator operator++(int) { return *this; }
};

}// end namespace::MiniSTL
