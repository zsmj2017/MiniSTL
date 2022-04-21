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

TEST(TraitsTest, is_instantiation_of_v) {
  ASSERT_TRUE((is_instantiation_of_v<A, A<int>>) );
  ASSERT_FALSE((is_instantiation_of_v<A, B>) );
}

TEST(TraitsTest, is_instantiation_of) {
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

TEST(TraitsTest, integral_constant) {
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

  static_assert(std::is_same<my_e_e2, my_e_e2>::value,
                "my_e_e2 != my_e_e2");
}

TEST(TraitsTest, bool_constant) {
  ASSERT_TRUE(bool_constant<0>() == false);
  ASSERT_TRUE(bool_constant<1>() == true);
  // cannot compile
  // ASSERT_TRUE(bool_constant<-1>() == true);
  ASSERT_TRUE(bool_constant<false>() == false);
  ASSERT_TRUE(bool_constant<true>() == true);
}

TEST(TraitsTest, is_integer) {
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

TEST(TraitsTest, trivial_destructor) {
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

TEST(TraitsTest, is_POD) {
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