#include "SequenceContainers/Vector/stl_vector.h"
#include "Utils/type_traits.h"
#include <gtest/gtest.h>

using namespace ::MiniSTL;
using std::string;

class TraitsTest : public testing::Test {
 protected:
  void SetUp() override {}
};

template<class>
struct A {};
struct B {};

TEST_F(TraitsTest, is_instantiation_of_v) {
  ASSERT_TRUE((is_instantiation_of_v<A, A<int>>) );
  ASSERT_FALSE((is_instantiation_of_v<A, B>) );
}

TEST_F(TraitsTest, is_instantiation_of) {
  ASSERT_TRUE((is_instantiation_of<A, A<int>>::value));
  ASSERT_FALSE((is_instantiation_of<A, B>::value));
}

int *int_pointer;
int const *int_const_pointer;
int volatile *int_volatile_pointer;
int const volatile *int_const_volatile_pointer;
int int_val = 0;
int const int_const_val = 0;
int volatile int_volatile_val = 0;
int &int_ref = int_val;
int const &int_const_ref = int_val;
int const volatile &int_const_volatile_ref = int_val;

//A type that represent any type:
struct any_type {
  //Dummy operations to forbid to compilers with intrinsic
  //type traits support to consider this type as a POD.
  any_type() : m_data(1) {}
  any_type(const any_type &) : m_data(2) {}
  any_type &operator=(const any_type &) {
    m_data = 3;
    return *this;
  }
  ~any_type() { m_data = 0; }

  bool operator==(const any_type &rhs) const { return m_data == rhs.m_data; }

  size_t m_data;
};

any_type any;
any_type *any_pointer;

//A type that represent any pod type
struct any_pod_type {};
any_pod_type any_pod;

template<>
struct type_traits<any_pod_type> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

TEST_F(TraitsTest, integral_constant) {
  typedef std::integral_constant<int, 2> two_t;
  typedef std::integral_constant<int, 4> four_t;

  ASSERT_TRUE(two_t::value * 2 == four_t::value);

  enum class my_e {
    e1,
    e2
  };

  typedef std::integral_constant<my_e, my_e::e1> my_e_e1;
  typedef std::integral_constant<my_e, my_e::e2> my_e_e2;

  ASSERT_TRUE(my_e_e1() == my_e::e1);
  ASSERT_TRUE((is_same<my_e_e2, my_e_e2>()));
}

TEST_F(TraitsTest, bool_constant) {
  ASSERT_TRUE(bool_constant<0>() == false);
  ASSERT_TRUE(bool_constant<1>() == true);
  // cannot compile
  // ASSERT_TRUE(bool_constant<-1>() == true);
  ASSERT_TRUE(bool_constant<false>() == false);
  ASSERT_TRUE(bool_constant<true>() == true);
}

TEST_F(TraitsTest, is_integer) {
  ASSERT_TRUE(is_integral<bool>());
  ASSERT_TRUE(is_integral<char>());
  typedef signed char signed_char;
  ASSERT_TRUE(is_integral<signed_char>());
  typedef unsigned char unsigned_char;
  ASSERT_TRUE(is_integral<unsigned_char>());
  ASSERT_TRUE(is_integral<wchar_t>());
  ASSERT_TRUE(is_integral<short>());
  typedef unsigned short unsigned_short;
  ASSERT_TRUE(is_integral<unsigned_short>());
  ASSERT_TRUE(is_integral<int>());
  typedef unsigned int unsigned_int;
  ASSERT_TRUE(is_integral<unsigned_int>());
  ASSERT_TRUE(is_integral<long>());
  typedef unsigned long unsigned_long;
  ASSERT_TRUE(is_integral<unsigned_long>());
  ASSERT_TRUE(is_integral<float>() == 0);
  ASSERT_TRUE(is_integral<double>() == 0);
  ASSERT_TRUE(is_integral<any_type>() == 0);
  ASSERT_TRUE(is_integral<any_type *>() == 0);
}

template<typename T>
bool has_trivial_destructor(T) {
  return has_trivial_destructor_t<T>();
}

struct DestructorMonitor {
  ~DestructorMonitor() { ++nb_destructor_call; }
  static size_t nb_destructor_call;
};

size_t DestructorMonitor::nb_destructor_call = 0;

template<>
struct type_traits<DestructorMonitor> {
  using has_trivial_default_constructor = true_type;
  using has_trivial_copy_constructor = true_type;
  using has_trivial_assignment_operator = true_type;
  using has_trivial_destructor = true_type;
  using is_POD_type = true_type;
};

TEST_F(TraitsTest, trivial_destructor) {
  ASSERT_TRUE(has_trivial_destructor(int_pointer) == 1);
  ASSERT_TRUE(has_trivial_destructor(int_const_pointer) == 1);
  ASSERT_TRUE(has_trivial_destructor(int_volatile_pointer) == 1);
  ASSERT_TRUE(has_trivial_destructor(int_const_volatile_pointer) == 1);
  ASSERT_TRUE(has_trivial_destructor(any_pointer) == 1);
  ASSERT_TRUE(has_trivial_destructor(any) == 0);
  ASSERT_TRUE(has_trivial_destructor(any_pointer) == 1);
  ASSERT_TRUE(has_trivial_destructor(any_pod) == 1);
  ASSERT_TRUE(has_trivial_destructor(string()) == 0);

  //Check of the meta information impact in a container implementation
  {
    vector<DestructorMonitor> v(10);
    DestructorMonitor::nb_destructor_call = 0;
  }
  ASSERT_TRUE(DestructorMonitor::nb_destructor_call == 0);
}

template<typename T>
int is_POD_type(T) {
  return is_POD_type_t<T>();
}

TEST_F(TraitsTest, is_POD) {
  ASSERT_TRUE(is_POD_type(int_pointer) == 1);
  ASSERT_TRUE(is_POD_type(int_const_pointer) == 1);
  ASSERT_TRUE(is_POD_type(int_volatile_pointer) == 1);
  ASSERT_TRUE(is_POD_type(int_const_volatile_pointer) == 1);
  ASSERT_TRUE(is_POD_type(any_pointer) == 1);
  ASSERT_TRUE(is_POD_type(any) == 0);
  ASSERT_TRUE(is_POD_type(any_pointer) == 1);
  ASSERT_TRUE(is_POD_type(any_pod) == 1);
  ASSERT_TRUE(is_POD_type(string()) == 0);
}

TEST_F(TraitsTest, is_same) {
  ASSERT_TRUE((is_same<int, int>()));
  // usually true if 'int' is 32 bit
  ASSERT_TRUE((is_same<int, std::int32_t>()));
  // possibly true if ILP64 data model is used
  ASSERT_FALSE((is_same<int, std::int64_t>()));

  ASSERT_TRUE((is_same_v<int, std::int32_t>) );
  ASSERT_FALSE((is_same_v<int, std::int64_t>) );

  // compare the types of a couple variables
  long double num1 = 1.0;
  long double num2 = 2.0;
  ASSERT_TRUE((is_same_v<decltype(num1), decltype(num2)>) );

  // 'float' is never an integral type
  ASSERT_FALSE((is_same<float, int>()));

  // 'int' is implicitly 'signed'
  ASSERT_FALSE((is_same<int, unsigned int>()));
  ASSERT_TRUE((is_same<int, signed int>()));

  // unlike other types, 'char' is neither 'unsigned' nor 'signed'
  ASSERT_TRUE((is_same<char, char>()));
  ASSERT_FALSE((is_same<char, signed char>()));
  ASSERT_FALSE((is_same<char, unsigned char>()));

  ASSERT_FALSE((is_same<const int, int>()));
}

TEST_F(TraitsTest, remove_pointer) {
  ASSERT_TRUE((is_same<int, int>()));
  ASSERT_FALSE((is_same<int, int *>()));
  ASSERT_FALSE((is_same<int, int **>()));

  ASSERT_TRUE((is_same<int, remove_pointer<int>::type>()));
  ASSERT_TRUE((is_same<int, remove_pointer<int *>::type>()));
  ASSERT_FALSE((is_same<int, remove_pointer<int **>::type>()));

  ASSERT_TRUE((is_same<int, remove_pointer_t<int *const>>()));
  ASSERT_TRUE((is_same<int, remove_pointer_t<int *volatile>>()));
  ASSERT_TRUE((is_same<int, remove_pointer_t<int *const volatile>>()));
}

TEST_F(TraitsTest, enable_if) {
  ASSERT_TRUE((is_same<int, enable_if_t<true, int>>()));
  // cannot compile
  //enable_if_t<is_same_v<int, float>, float>;
  ASSERT_TRUE((enable_if_t<is_same_v<int, int>, any_type>() == any_type()));
}

TEST_F(TraitsTest, void_t) {
  ASSERT_TRUE((is_same<void_t<>, void>::value));
  ASSERT_TRUE((is_same<void_t<int>, void>::value));
  ASSERT_TRUE((is_same<void_t<int, short>, void>::value));
  ASSERT_TRUE(
      (is_same<void_t<int, short, std::string>, void>::value));
  ASSERT_TRUE((has_value_type<std::string>::value));
  ASSERT_FALSE((has_value_type<int>::value));
}

TEST_F(TraitsTest, type_t) {
  ASSERT_TRUE((is_same<type_t<float>, float>::value));
  ASSERT_TRUE((is_same<type_t<float, int>, float>::value));
  ASSERT_TRUE((is_same<type_t<float, int, short>, float>::value));
  ASSERT_TRUE(
      (is_same<type_t<float, int, short, std::string>, float>::
           value));
}

template<typename T, typename V>
using detector_find = decltype(declval<T>().find(declval<V>()));

TEST_F(TraitsTest, detected_or_t) {
  // std::string str;
  // char c;
  // std::string::size_type size;
  // size = str.find(c)
  ASSERT_TRUE((
      std::is_same<
          detected_or_t<float, detector_find, std::string, char>,
          std::string::size_type>::value));
  // can not compile detector_find<double,char> , so detected_or_t return float
  ASSERT_TRUE((
      std::is_same<
          detected_or_t<float, detector_find, double, char>,
          float>::value));
}

TEST_F(TraitsTest, detected_t) {
  ASSERT_TRUE((
      std::is_same<
          detected_t<detector_find, std::string, char>,
          std::string::size_type>::value));
  // can not compile detector_find<double,char> , so detected_t return nonesuch
  ASSERT_TRUE((
      std::is_same<
          detected_t<detector_find, double, char>,
          nonesuch>::value));
}

TEST_F(TraitsTest, is_detected) {
  ASSERT_TRUE((is_detected<detector_find, std::string, char>::value));
  ASSERT_FALSE((is_detected<detector_find, double, char>::value));
}

TEST_F(TraitsTest, is_detected_v) {
  ASSERT_TRUE((is_detected_v<detector_find, std::string, char>) );
  ASSERT_FALSE((is_detected_v<detector_find, double, char>) );
}

TEST_F(TraitsTest, is_reference) {
  class A {};
  ASSERT_FALSE(is_reference<A>());
  static_assert(is_reference<A &>());
  static_assert(is_reference<A &&>());
  ASSERT_FALSE(is_reference<long>());
  static_assert(is_reference<long &>());
  static_assert(is_reference<long &&>());
  ASSERT_FALSE(is_reference<double *>());
  static_assert(is_reference<double *&>());
  static_assert(is_reference<double *&&>());
}