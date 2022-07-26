#pragma once

#include "Function/function_adapter.h"
#include "Iterator/stl_iterator.h"
#include "Utils/type_traits.h"
#include <cstddef>
#include <cstring>

namespace MiniSTL {

// 不能保证长度相等
// 接受自定义比较器
template<class InputIterator1, class InputIterator2, class BinaryPredicate = equal_to<value_type_t<InputIterator1>>>
inline bool equal(InputIterator1 first1, InputIterator1 last1,
                  InputIterator2 first2, const BinaryPredicate &binary_pred = BinaryPredicate()) {
  for (; first1 != last1; ++first1, ++first2) {
    if (!binary_pred(*first1, *first2)) {
      return false;
    }
  }
  return true;
}

template<class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T &value) {
  for (; first != last; ++first) *first = value;
}

// 显然，本算法执行覆写操作，因此通常配合inserter完成
template<class OutputIterator, class Size, class T>
OutputIterator fill_n(OutputIterator first, Size n, const T &value) {
  for (; n > 0; --n, ++first) *first = value;
  return first;
}

// 必须要知道迭代器指向的对象类型，才能够构造对象，因此本处使用了value_type
template<class ForwardIterator1, class ForwardIterator2, class T>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b, T) {
  T temp = *a;
  *a = *b;
  *b = temp;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
  return iter_swap(a, b, value_type_t<ForwardIterator1>());
}

template<class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (*first1 < *first2)
      return true;
    else if (*first1 > *first2)
      return false;
  }
  return first1 == last1 && first2 != last2;// 若第二序列有余，返回true，否则false
}

template<class T>
inline const T &max(const T &a, const T &b) {
  return a < b ? b : a;
}

template<class T, class Compare>
inline const T &max(const T &a, const T &b, Compare comp) {
  return comp(a, b) ? b : a;
}

template<class T>
inline const T &min(const T &a, const T &b) {
  return b < a ? b : a;
}

template<class T, class Compare>
inline const T &min(const T &a, const T &b, Compare comp) {
  return comp(b, a) ? b : a;
}

template<class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             Compare comp) {
  for (; first1 != last1 && first2 != last2; ++first1, ++first2) {
    if (comp(*first1, *first2))
      return true;
    else if (comp(*first2, *first1))
      return false;
  }
  return first1 == last1 && first2 != last2;// 若第二序列有余，返回true，否则false
}

// 针对原始指针const unsigned char*的全特化版本
inline bool lexicographical_compare(const unsigned char *first1,
                                    const unsigned char *last1,
                                    const unsigned char *first2,
                                    const unsigned char *last2) {
  const size_t len1 = last1 - first1;
  const size_t len2 = last2 - first2;
  // 先比较长度相同的段落
  const int result = memcmp(first1, first2, min(len1, len2));
  return result != 0 ? result < 0 : len1 < len2;
}

// 显然要求序列1长于序列2
template<class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2) {
  while (first1 != last1 && *first1 == *first2) {
    ++first1, ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1,
                                              InputIterator1 last1,
                                              InputIterator2 first2,
                                              BinaryPredicate binary_pred) {
  while (first1 != last1 && binary_pred(*first1, *first2)) {
    ++first1, ++first2;
  }
  return pair<InputIterator1, InputIterator2>(first1, first2);
}

template<class T>
inline void swap(T &a, T &b) {
  T temp = a;
  a = b;
  b = temp;
}

template<class InputIterator, class OutputIterator>
struct _copy_dispatch {// 仿函数对象
  OutputIterator operator()(InputIterator first, InputIterator last,
                            OutputIterator result) {
    return _copy(first, last, result,
                 iterator_category_t<InputIterator>());
  }
};

// 偏特化处理
template<class T>
struct _copy_dispatch<T *, T *> {
  T *operator()(T *first, T *last, T *result) {
    using t = typename type_traits<T>::has_trivial_assignment_operator;
    return _copy_t(first, last, result, t());
  }
};

// 偏特化处理
template<class T>
struct _copy_dispatch<const T *, T *> {
  T *operator()(const T *first, const T *last, T *result) {
    using t = typename type_traits<T>::has_trivial_assignment_operator;
    return _copy_t(first, last, result, t());
  }
};

template<class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last,
                           OutputIterator result) {
  return _copy_dispatch<InputIterator, OutputIterator>()(
      first, last, result);// _copy_dispatch是一个仿函数对象
}

// 针对指针的偏特化
inline char *copy(const char *first, const char *last, char *result) {
  memmove(result, first, last - first);
  return result + (last - first);
}

inline wchar_t *copy(const wchar_t *first, const wchar_t *last,
                     wchar_t *result) {
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

// InputIterator
template<class InputIterator, class OutputIterator>
inline OutputIterator _copy(InputIterator first, InputIterator last,
                            OutputIterator result, input_iterator_tag) {
  for (; first != last; ++first, ++result)// 迭代器判同，速度较慢
    *result = *first;
  return result;
}

template<class InputIterator, class OutputIterator, class Distance>
inline OutputIterator _copy_d(InputIterator first, InputIterator last,
                              OutputIterator result, Distance) {
  for (Distance n = last - first; n > 0;
       --n, ++first, ++result)// 以n决定循环次数，速度较快
    *result = *first;
  return result;
}

// RandomIterator
template<class InputIterator, class OutputIterator>
inline OutputIterator _copy(InputIterator first, InputIterator last,
                            OutputIterator result,
                            random_access_iterator_tag) {
  return _copy_d(first, last, result,
                 difference_type_t<InputIterator>());// 再细分函数以便复用
}

// 具备trivial copy assignment operator，可执行memmove
template<class T>
inline T *_copy_t(const T *first, const T *last, T *result, true_type) {
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}

// 原始指针是一种random_access_iterator
template<class T>
inline T *_copy_t(const T *first, const T *last, T *result, false_type) {
  return _copy_d(first, last, result, ptrdiff_t());
}

template<class BidirectionalIter1, class BidirectionalIter2, class Distance>
inline BidirectionalIter2 _copy_backward(BidirectionalIter1 first,
                                         BidirectionalIter1 last,
                                         BidirectionalIter2 result,
                                         bidirectional_iterator_tag,
                                         Distance) {
  while (first != last) *--result = *--last;
  return result;
}

template<class RandomAccessIter, class BidirectionalIter2, class Distance>
inline BidirectionalIter2 _copy_backward(RandomAccessIter first,
                                         RandomAccessIter last,
                                         BidirectionalIter2 result,
                                         random_access_iterator_tag,
                                         Distance) {
  for (Distance n = last - first; n > 0; --n) *--result = *--last;
  return result;
}

template<class BidirectionalIter1, class BidirectionalIter2, class BoolType>
struct _copy_backward_dispatch {
  BidirectionalIter2 operator()(BidirectionalIter1 first,
                                BidirectionalIter1 last,
                                BidirectionalIter2 result) {
    return _copy_backward(first, last, result,
                          iterator_category_t<BidirectionalIter1>(),
                          difference_type_t<BidirectionalIter1>());
  }
};

template<class T>
struct _copy_backward_dispatch<T *, T *, true_type> {
  T *operator()(const T *first, const T *last, T *result) {
    const ptrdiff_t n = last - first;
    memmove(result - n, first, sizeof(T) * n);
    return result - n;
  }
};

template<class T>
struct _copy_backward_dispatch<const T *, T *, true_type> {
  T *operator()(const T *first, const T *last, T *result) {
    return _copy_backward_dispatch<T *, T *, true_type>()(first, last,
                                                          result);
  }
};

template<class BI1, class BI2>
inline BI2 copy_backward(BI1 first, BI1 last, BI2 result) {
  using Trivial = typename type_traits<
      value_type_t<BI2>>::has_trivial_assignment_operator;
  return _copy_backward_dispatch<BI1, BI2, Trivial>()(first, last, result);
}

template<typename T>
decltype(auto) move(T &&param) {
  using ReturnType = remove_reference_t<T> &&;
  return static_cast<ReturnType>(param);
}

template<typename T>
T &&forward(remove_reference_t<T> &param) {
  return static_cast<T &&>(param);
}

}// namespace MiniSTL