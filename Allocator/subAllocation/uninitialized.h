#pragma once
#include"my_construct.h"

template <class InputIterator, class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
	return __uninitialized_copy(first, last, result, value_type(first));
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T *) {
	using is_POD = typename  __type_traits<T>::is_POD_type;
	return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
	return copy(first, last, result);
}

template<class InputIterator, class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
	ForwardIterator cur = result;
	for (; first != last; ++cur,++first)
		construct(&*cur, *first);
	return cur;
}

//针对char*、wchar_t*存在特化版本 memmove直接移动内存
inline char*  uninitialized_copy(char* first, char* last, char* result) {
	memmove(result, first, last - first);
	return result + (last - first);
}

inline wchar_t*  uninitialized_copy(wchar_t* first, wchar_t* last, wchar_t* result) {
	memmove(result, first, sizeof(wchar_t)*(last - first));
	return result + (last - first);
}

template <class ForwardIterator, class T>
inline ForwardIterator uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
	return __uninitialized_fill(first, last, value, value_type(first));
}

template<class ForwardIterator, class T, class T1>
inline ForwardIterator  __uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value, T1*) {
	using is_POD = typename __type_traits<T1>::is_POD_type;
	return __uninitialized_fill_aux(first, last, value, is_POD());
}

template<class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __true_type) {
	return fill(first, last, value);
}

template<class ForwardIterator, class T>
inline ForwardIterator  __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& value, __false_type) {
	ForwardIterator cur = first;
	for (:cur != last; ++cur)
		construct(&*cur, value);
	return cur;
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T & x) {
	//根据value_type取出first的value_type(详见迭代器一章）
	return __uninitialized_fill_n(first, n, x, value_type(first));
}

template<class ForwardIterator, class Size, class T, class T1>
inline ForwardIterator  __uninitialized_fill_n(ForwardIterator first, Size n, const T & x, T1*) {
	//POD means Plain Old Data
	//必然拥有trivial ctor/dtot/copy/assignment函数
	using is_POD = typename __type_traits<T1>::is_POD_type;
	return __uninitialized_fill_n_aux(first, n, x, is_POD());
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator  __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T & x,__true_type) {
	//POD类型可直接采用初值填写式手法
	return fill(first, n, x);//转交高阶函数执行
}

template<class ForwardIterator, class Size, class T>
inline ForwardIterator  __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T & x, __false_type) {
	//忽略异常处理
	//明确明确的是一旦一个对象构造失败则需要析构所有对象
	ForwardIterator cur = first;
	for (; n > 0; --n, ++cur)
		construct(&*cur, x);
	return cur;
}

