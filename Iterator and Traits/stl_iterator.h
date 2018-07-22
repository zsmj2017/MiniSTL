#pragma once

#include <cstddef> //ptrdiff_t定义

//五种迭代器类型
struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag : public input_iterator_tag { };
struct bidirectional_iterator_tag : public forward_iterator_tag { };
struct random_access_iterator_tag : public bidirectional_iterator_tag { };

//为了避免遗忘记录traits，此基类以供自定义iterator继承之
template<class Category,class T,class Distance = ptrdiff_t,class Pointer = T*,class Reference =T&>
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
	using reference = typename Iterator::Reference;
};

//针对raw pointer设计的偏特化版本
template <class T>
struct iterator_traits<T*> {
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = T*;
	using reference = T&;
};

//针对pointer-to-const设计的偏特化版本
template <class T>
struct iterator_traits<const T*> {
	using iterator_category = random_access_iterator_tag;
	using value_type = T;
	using difference_type = ptrdiff_t;
	using pointer = const T* ;
	using reference = const T& ;
};

//以下为模仿C++14 type_traits_t而设定的别名模板
template<class T>
using iterator_category_t = typename iterator_traits<T>::iterator_category;

template<class T>
using value_type_t = typename iterator_traits<T>::value_type;

template<class T>
using difference_type_t = typename iterator_traits<T>::difference_type;

template<class T>
using pointer_t = typename iterator_traits<T>::pointer;

template<class T>
using reference_t = typename iterator_traits<T>::reference;

//迭代器属性获取函数
template<class Iterator>
inline iterator_category_t<Iterator> iterator_category(const Iterator&) {
	return iterator_category_t<Iterator>();
}

template<class Iterator>
inline difference_type_t<Iterator> distance_type(const Iterator&) {
	return  difference_type_t<Iterator>();
}

template<class Iterator>
inline value_type_t<Iterator> value_type(const Iterator&) {
	return  value_type_t<Iterator>();
}