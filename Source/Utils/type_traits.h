#pragma once

namespace MiniSTL {

namespace {
template<bool, class Ta, class Tb>
struct IfThenElse;
template<class Ta, class Tb>
struct IfThenElse<true, Ta, Tb> {
  using result = Ta;
};
template<class Ta, class Tb>
struct IfThenElse<false, Ta, Tb> {
  using result = Tb;
};
}// namespace

struct _true_type {};
struct _false_type {};

// 默认自定义类型均为non-POD类型
template<class T>
struct _type_traits {
  using has_trivial_default_constructor = _false_type;
  using has_trivial_copy_constructor = _false_type;
  using has_trivial_assignment_operator = _false_type;
  using has_trivial_destructor = _false_type;
  using is_POD_type = _false_type;
};

template<>
struct _type_traits<bool> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<char> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<unsigned char> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<signed char> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<wchar_t> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<short> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<unsigned short> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<int> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<unsigned int> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<long> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<unsigned long> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<long long> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<unsigned long long> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<float> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<double> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<long double> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<class T>
struct _type_traits<T *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<class T>
struct _type_traits<const T *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<char *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<unsigned char *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<signed char *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<const char *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<const unsigned char *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

template<>
struct _type_traits<const signed char *> {
  using has_trivial_default_constructor = _true_type;
  using has_trivial_copy_constructor = _true_type;
  using has_trivial_assignment_operator = _true_type;
  using has_trivial_destructor = _true_type;
  using is_POD_type = _true_type;
};

// is_integer(默认自定义类型为false)

template<class T>
struct _is_integer {
  using _integral = _false_type;
};

// simluate C++14
template<class T>
using _is_integer_t = typename _is_integer<T>::_integral;

template<>
struct _is_integer<bool> {
  using _integral = _true_type;
};

template<>
struct _is_integer<char> {
  using _integral = _true_type;
};

template<>
struct _is_integer<signed char> {
  using _integral = _true_type;
};

template<>
struct _is_integer<unsigned char> {
  using _integral = _true_type;
};

template<>
struct _is_integer<wchar_t> {
  using _integral = _true_type;
};

template<>
struct _is_integer<short> {
  using _integral = _true_type;
};

template<>
struct _is_integer<unsigned short> {
  using _integral = _true_type;
};

template<>
struct _is_integer<int> {
  using _integral = _true_type;
};

template<>
struct _is_integer<unsigned int> {
  using _integral = _true_type;
};

template<>
struct _is_integer<long> {
  using _integral = _true_type;
};

template<>
struct _is_integer<unsigned long> {
  using _integral = _true_type;
};

template<>
struct _is_integer<long long> {
  using _integral = _true_type;
};

template<>
struct _is_integer<unsigned long long> {
  using _integral = _true_type;
};

// remove reference
template<class T>
struct remove_reference {
  using type = T;
};

template<class T>
struct remove_reference<T &> {
  using type = T;
};

template<class T>
struct remove_reference<T &&> {
  using type = T;
};

template<class T>
using remove_reference_t = typename remove_reference<T>::type;

}// namespace MiniSTL
