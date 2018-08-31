#pragma once
#include "construct.h"
#include "typeTraits.h"
#include "stl_iterator.h"
#include <cstring> // memove

template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
	using isPODType = typename _type_traits<value_type_t<InputIterator> >::is_POD_type;
	return __uninitialized_copy_aux(first, last, result, isPODType());
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator  __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _true_type) {
	memcpy(result, first, (last - first) * sizeof(*first));
	return result + (last - first);
}

template<class InputIterator, class ForwardIterator>
inline ForwardIterator  __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, _false_type) {
	ForwardIterator cur = result;
	for (; first != last; ++cur,++first)
		construct(&*cur, *first);
	return cur;
}

//针对char*、wchar_t*存在特化版本 memmove直接移动内存
inline char* uninitialized_copy(char* first, char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t* uninitialized_copy(wchar_t* first, wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}

template <class ForwardIterator, class T>
inline ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
	using isPODType = typename _type_traits<value_type_t<InputIterator> >::is_POD_type;
	return __uninitialized_fill_aux(first, last, result, isPODType());
}

template<class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _true_type) {
	return memset(first, value, (last - first) * sizeof(*first));
}

template<class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, _false_type) {
	ForwardIterator cur = first;
	for (;cur != last; ++cur)
		construct(&*cur, value);
	return cur;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T & value) {
	using isPODType = typename _type_traits<value_type_t<ForwardIterator> >::is_POD_type;
	return __uninitialized_fill_n_aux(first, n, value, isPODType());
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator  __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T & value,_true_type) {
	//POD类型可直接采用初值填写式手法
	memset(first, value, n);
	return first + n;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator  __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T & value, _false_type) {
	//忽略异常处理
	//明确明确的是一旦一个对象构造失败则需要析构所有对象
	ForwardIterator cur = first;
	for (; n > 0; --n, ++cur)
		construct(&*cur, value);
	return cur;
}

