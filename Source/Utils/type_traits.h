#pragma once

namespace MiniSTL {

template<bool b, class T>
struct enable_if {};

template<class T>
struct enable_if<true, T> {
  using type = T;
};

template<bool b, class T>
using enable_if_t = typename enable_if<b, T>::type;

template<class T, T val>
struct integral_constant {
  static constexpr T value = val;
  using value_type = T;
  using type = integral_constant;// using injected-class-name
  constexpr operator value_type() const noexcept { return value; }
  // enables integral_constant to serve as a source of compile-time function objects.
  constexpr value_type operator()() const noexcept { return value; }// since c++14
};

// static对象实例化在对象外
template<class T, T val>
constexpr T integral_constant<T, val>::value;

template<bool val>
using bool_constant = integral_constant<bool, val>;

struct true_type : bool_constant<true> {};
struct false_type : bool_constant<false> {};

template<class T, class U>
struct is_same : false_type {};

template<class T>
struct is_same<T, T> : true_type {};

template<class T, class U>
constexpr bool is_same_v = is_same<T, U>::value;

// 自定义类型默认为false
template<class T>
struct is_integral : false_type {};

template<class T>
constexpr bool is_integral_v = is_integral<T>::value;

template<>
struct is_integral<bool> : true_type {};

template<>
struct is_integral<char> : true_type {};

template<>
struct is_integral<signed char> : true_type {};

template<>
struct is_integral<unsigned char> : true_type {};

template<>
struct is_integral<wchar_t> : true_type {};

template<>
struct is_integral<short> : true_type {};

template<>
struct is_integral<unsigned short> : true_type {};

template<>
struct is_integral<int> : true_type {};

template<>
struct is_integral<unsigned int> : true_type {};

template<>
struct is_integral<long> : true_type {};

template<>
struct is_integral<unsigned long> : true_type {};

template<>
struct is_integral<long long> : true_type {};

template<>
struct is_integral<unsigned long long> : true_type {};

// 默认自定义类型均为non-POD类型
template<class T>
struct type_traits {
  using has_trivial_default_constructor = false_type;
  using has_trivial_copy_constructor = false_type;
  using has_trivial_assignment_operator = false_type;
  using has_trivial_destructor = false_type;
  using is_POD_type = false_type;
};

template<>
struct type_traits<bool> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<char> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned char> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<signed char> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<wchar_t> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<short> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned short> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<int> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned int> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<long long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned long long> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<float> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<double> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<long double> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<class T>
struct type_traits<T *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<class T>
struct type_traits<const T *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<unsigned char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<signed char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<const char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<const unsigned char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<>
struct type_traits<const signed char *> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

template<class T>
using has_trivial_default_constructor_t = typename type_traits<T>::has_trivial_default_constructor;

template<class T>
using has_trivial_copy_constructor_t = typename type_traits<T>::has_trivial_copy_constructor;

template<class T>
using has_trivial_assignment_operator_t = typename type_traits<T>::has_trivial_assignment_operator;

template<class T>
using has_trivial_destructor_t = typename type_traits<T>::has_trivial_destructor;

template<class T>
using is_POD_type_t = typename type_traits<T>::is_POD_type;

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

// add reference
template<class T>
struct add_lvalue_reference {
  using type = T &;
};

template<class T>
using add_lvalue_reference_t = typename add_lvalue_reference<T>::type;

template<class T>
struct add_rvalue_reference {
  using type = T &&;
};

template<class T>
using add_rvalue_reference_t = typename add_rvalue_reference<T>::type;

// usually use declval with decltype
// return T&&
template<class T>
add_rvalue_reference_t<T> declval() noexcept;

// remove pointer
template<class T>
struct remove_pointer { using type = T; };

template<class T>
struct remove_pointer<T *> { using type = T; };

template<class T>
struct remove_pointer<T *const> { using type = T; };

template<class T>
struct remove_pointer<T *volatile> { using type = T; };

template<class T>
struct remove_pointer<T *const volatile> { using type = T; };

template<class T>
using remove_pointer_t = typename remove_pointer<T>::type;

// 判断当前类型是否为某class template的实例化
// 使用方法可见UT
template<template<typename...> class, typename>
constexpr bool is_instantiation_of_v = false;
template<template<typename...> class C, typename... T>
constexpr bool is_instantiation_of_v<C, C<T...>> = true;
template<template<typename...> class C, typename... T>
struct is_instantiation_of : bool_constant<is_instantiation_of_v<C, T...>> {};

namespace {
// aux for void_t
template<class T, class...>
struct type_t_ {
  using type = T;
};

// 获取模板参数的第一个（以此实现void_t)
template<class T, class... Ts>
using type_t = typename type_t_<T, Ts...>::type;
}// namespace

// 获取void类型，常用于模板片特化，具体使用案例可见has_value_type
template<class... Ts>
using void_t = type_t<void, Ts...>;

// has_value_type<T>::value is true if T has a nested type `value_type`
template<class T, class = void>
struct has_value_type : false_type {};

template<class T>
struct has_value_type<T, void_t<typename T::value_type>> : true_type {};

namespace {
//  nonesuch
//  A tag type which traits may use to indicate lack of a result type.
//  Similar to void in that no values of this type may be constructed. Different
//  from void in that no functions may be defined with this return type and no
//  complete expressions may evaluate with this expression type.
struct nonesuch {
  ~nonesuch() = delete;
  nonesuch(nonesuch const &) = delete;
  void operator=(nonesuch const &) = delete;
};

// aux for detect_xxxx
template<typename Void, typename D, template<typename...> class, typename...>
struct detected_ {
  using value_t = false_type;
  using type = D;
};
template<typename D, template<typename...> class T, typename... A>
struct detected_<void_t<T<A...>>, D, T, A...> {
  using value_t = true_type;
  using type = T<A...>;
};
}// namespace

template<typename D, template<typename...> class T, typename... A>
using detected_or = detected_<void, D, T, A...>;

// return T<A...> if T<A...> is detected, otherwise return D
template<typename D, template<typename...> class T, typename... A>
using detected_or_t = typename detected_or<D, T, A...>::type;

template<template<typename...> class T, typename... A>
using detected_t = detected_or_t<nonesuch, T, A...>;
template<template<typename...> class T, typename... A>
constexpr bool is_detected_v = detected_or<nonesuch, T, A...>::value_t::value;

// 判断是否可以构成T<A...>
template<template<typename...> class T, typename... A>
struct is_detected : detected_or<nonesuch, T, A...>::value_t {};

}// namespace MiniSTL
