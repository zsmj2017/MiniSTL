#pragma once

#include "stl_tempbuf.h"

namespace MiniSTL {

// adjacent_find:找出一组满足条件的相邻元素
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
  if (first == last) return last;
  ForwardIterator next = first;
  while (++next != last) {
    if (*first == *next)
      return first;
    else
      first = next;
  }
  return last;
}

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last,
                              BinaryPredicate binary_pred) {
  if (first == last) return last;
  ForwardIterator next = first;
  while (++next != last) {
    if (binary_pred(*first, *next))
      return first;
    else
      first = next;
  }
  return last;
}

// count:运用equality判断有几个与value相等的元素
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type count(
    InputIterator first, InputIterator last, T value) {
  typename iterator_traits<InputIterator>::difference_type count = 0;
  for (; first != last; ++first)
    if (*first == value) count++;
  return count;
}

// count_if:将指定操作实施于整个区间，并将“令pred的计算结果为true”的元素个数返回
template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type count_if(
    InputIterator first, InputIterator last, Predicate pred) {
  typename iterator_traits<InputIterator>::difference_type count = 0;
  for (; first != last; ++first)
    if (pred(*first)) count++;
  return count;
}

// count:运用equality查找元素
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, T value) {
  while (first != last && *first != value) ++first;
  return first;
}

// find_if:将指向“令pred的计算结果为true”的第一个元素的迭代器返回
template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
  while (first != last && pred(*first)) ++first;
  return first;
}

// find_end:在区间S1[first1,last1)中找出区间S2[first2,last2)的最后出现点，若不存在，返回last1
//显然，若迭代器支持逆向查找则较为便利
template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1,
                                 ForwardIterator1 last1,
                                 ForwardIterator2 first2,
                                 ForwardIterator2 last2) {
  using category1 =
      typename iterator_traits<ForwardIterator1>::iterator_category;
  using category2 =
      typename iterator_traits<ForwardIterator2>::iterator_category;
  return __find_end(first1, last1, first2, last2, category1(), category2());
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1,
                            ForwardIterator2 first2, ForwardIterator2 last2,
                            forward_iterator_tag, forward_iterator_tag) {
  if (first2 == last2)
    return last1;
  else {
    ForwardIterator1 result = last1;
    while (1) {
      ForwardIterator1 new_result =
          search(first1, last1, first2, last2);  //利用search查找首次出现点
      if (new_result == last1)
        return result;
      else {
        result = new_result;  //更新result
        first1 = new_result;
        ++first1;  //更换搜索位置
      }
    }
  }
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1,
                                  BidirectionalIterator1 last1,
                                  BidirectionalIterator1 first2,
                                  BidirectionalIterator1 last2,
                                  bidirectional_iterator_tag,
                                  bidirectional_iterator_tag) {
  //采用反向迭代器
  using reviter1 = reverse_iterator<BidirectionalIterator1>;
  using reviter2 = reverse_iterator<BidirectionalIterator2>;

  reviter1 rlast1(first1);
  reviter2 rlast2(first2);

  reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);

  if (rresult == rlast1)
    return last1;
  else {
    BidirectionalIterator1 result = rresult.base();
    advance(result, -distance(first2, last2));  //回归子序列头部
    return result;
  }
}

// find_first_of:以S2区间内某些元素为目标，寻找它们在S1区间内首次出现地点。
template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1,
                            ForwardIterator first2, ForwardIterator last2) {
  for (; first1 != last1; ++first1)
    for (ForwardIterator iter = first2; iter != last2; ++iter)
      if (*first1 == *iter) return first1;
  return last1;
}

// for_each:将f作用于区间元素之上，不能改变元素，欲改变应当使用transform
//具备一个通常被忽略的返回值
template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
  for (; first != last; ++first) f(*first);
  return f;
}

// generate:将gen的运算结果填写于[first,last)，采用assignment
template <class InputIterator, class Generator>
void generate(InputIterator first, InputIterator last, Generator gen) {
  for (; first != last; ++first) *first = gen();
}

// generate_n:将gen的运算结果填写于起点为first，长度为n的区间内，采用assignment
template <class OutputIterator, class Size, class Generator>
OutputIterator generate(OutputIterator first, Size n, Generator gen) {
  for (; n > 0; --n, ++first) *first = gen();
  return first;
}

// include：判断S2是否被S1所包含,要求有序，默认为升序
template <class InputIterator1, class InputIterator2>
bool include(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
             InputIterator2 last2) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1)  //一个不在则不可能包含
      return false;
    else if (*first1 < *first2)
      ++first1;
    else
      ++first1, ++first2;
  }
  return first2 == last2;
}

// comp必须满足等价判定表达式（严格弱序或升序）
template <class InputIterator1, class InputIterator2, class Compare>
bool include(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2,
             InputIterator2 last2, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first2, *first1))
      return false;
    else if (comp(*first1, *first2))
      ++first1;
    else
      ++first1, ++first2;
  }
  return first2 == last2;
}

// max_element:返回指向序列中数值最大元素的迭代器
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last)
    if (*result < *first) result = first;
  return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last,
                            Compare comp) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last)
    if (comp(*result, *first)) result = first;
  return result;
}

// min_element:返回指向序列中数值最小元素的迭代器
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last)
    if (*first < *result) result = first;
  return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last,
                            Compare comp) {
  if (first == last) return first;
  ForwardIterator result = first;
  while (++first != last)
    if (comp(*first, *result)) result = first;
  return result;
}

// merge:将有序的S1与S2合并置于另一段空间，合并结果仍然有序，返回一个迭代器指向新区间的last
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result) {
  while (first1 != last1 && first2 != last2) {
    if (*first2 < *first1)
      *result++ = *first2, ++first2;
    else
      *result++ = *first1, ++first1;
  }
  return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1,
                     InputIterator2 first2, InputIterator2 last2,
                     OutputIterator result, Compare comp) {
  while (first1 != last1 && first2 != last2) {
    if (comp(*first2, *first1))
      *result++ = *first2, ++first2;
    else
      *result++ = *first1, ++first1;
  }
  return copy(first2, last2, copy(first1, last1, result));
}

// partition:将被pred判定为true的移动到前列，unstable
//存在stable_partition
template <class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first,
                                BidirectionalIterator last, Predicate pred) {
  while (true) {
    //双指针找到交换点，交换
    while (true)
      if (first == last)
        return first;
      else if (pred(*first))  //头指针所指元素为true
        ++first;              //拒绝移动，头指针前进
      else
        break;  //找到了需要被移动的元素
    --last;     //尾指针回溯
    while (true)
      if (first == last)
        return first;
      else if (!pred(*last))  //尾指针指向元素为false
        --last;               //拒绝移动
      else
        break;  //找到了需要被移动的元素
    iter_swap(first, last);
    ++first;
  }
}

// remove:本身并不做remove操作，只是将元素后移
template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, T value) {
  first = find(first, last, value);  //循序找出第一个相等元素
  ForwardIterator next = first;
  return first == last ? first
                       : remove_copy(++next, last,
                                     value);  //利用remove_copy（会不会低效?）
}

// remove_copy:将结果拷贝到result
template <class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last,
                           OutputIterator result, T value) {
  for (; first != last; ++first)
    if (*first != value) *result++ = *first;
  return result;
}

// remove_if:在remove的基础上加入了谓词
template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last,
                          Predicate pred) {
  first = find_if(first, last, pred);
  ForwardIterator next = first;
  return first == last ? first : remove_copy_if(++next, last, pred);
}

// remove_copy_if:在remove_copy的基础上加入了谓词
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred) {
  for (; first != last; ++first)
    if (!pred(*first)) *result++ = *first;
  return result;
}

// replace:将[first,last)区间内所有old_value替换为new_value
template <class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_value,
             const T& new_value) {
  for (; first != last; ++first)
    if (*first == old_value) *first = new_value;
}

// replace_copy:将结果拷贝到result
template <class InputIterator, class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,
                            OutputIterator result, const T& old_value,
                            const T& new_value) {
  for (; first != last; ++first, ++result)
    *result = *first == old_value ? new_value : *first;
  return result;
}

// replace_if:所有被pred判定为true的元素将被取代为new_value
template <class ForwardIterator, class Predicate, class T>
ForwardIterator replace_if(ForwardIterator first, ForwardIterator last,
                           Predicate pred, const T& new_value) {
  for (; first != last; ++first)
    if (pred(*first)) *first = new_value;
}

// replace_copy_if:在replace_copy的基础上加入了谓词
template <class InputIterator, class OutputIterator, class Predicate, class T>
OutputIterator remove_copy_if(InputIterator first, InputIterator last,
                              OutputIterator result, Predicate pred,
                              const T& new_value) {
  for (; first != last; ++first, ++result)
    *result = pred(*first) ? new_value : *first;
  return result;
}

// reverse:将指定区间颠倒重排，迭代器的性质会对效率产生影响
template <class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
  __reverse(first, last, iterator_category(last));
}

template <class BidirectionalIterator>
void __reverse(BidirectionalIterator first, BidirectionalIterator last,
               bidirectional_iterator_tag) {
  while (true)
    if (first == last || first == --last)  //空集或仅有一个，注意此时last已自减
      return;
    else
      iter_swap(first++, last);
}

template <class RandomAccessIterator>
void __reverse(RandomAccessIterator first, RandomAccessIterator last,
               random_access_iterator_tag) {
  while (first < last)  //只有random支持operator<
    iter_swap(first++, last--);
}

// reverse_copy:将指定区间颠倒重排，迭代器的性质会对效率产生影响
template <class BidirectionalIterator, class OutputIterator>
void reverse_copy(BidirectionalIterator first, BidirectionalIterator last,
                  OutputIterator result) {
  while (first != last) {
    --last;
    *result = last;
    ++result;
  }
  return result;
}

// rotate:将区间[first,middle)与[middle,last)置换，二者的长度可不相等
//根据迭代器性质演变出不同的算法
template <class ForwardIterator>
inline void rotate(ForwardIterator first, ForwardIterator middle,
                   ForwardIterator last) {
  if (first == middle || middle == last) return;
  __rotate(first, middle, last, difference_type_t<ForwardIterator>(),
           iterator_category_t<ForwardIterator>());
}

// rotate-forward
template <class ForwardIterator, class Distance>
void __rotate(ForwardIterator first, ForwardIterator middle,
              ForwardIterator last, Distance, forward_iterator_tag) {
  for (ForwardIterator i = middle;;) {
    iter_swap(first, i);  //一一交换前端与后端元素
    ++first;
    ++i;
    //判断前后端谁先结束
    if (first == middle) {
      if (i == last)  //如果前端结束的同时后端也结束
        return;
      middle = i;          //更新区间
    } else if (i == last)  //后端先结束
      i = middle;          //更新区间
  }
}

// rotate-bidrectional
template <class BidrectionalIterator, class Distance>
void __rotate(BidrectionalIterator first, BidrectionalIterator middle,
              BidrectionalIterator last, Distance, bidirectional_iterator_tag) {
  reverse(first, middle);
  reverse(middle, last);
  reverse(first, last);
}

// rotate-randomaccess
template <class RandomAccesslIterator, class Distance>
void __rotate(RandomAccesslIterator first, RandomAccesslIterator middle,
              RandomAccesslIterator last, Distance,
              random_access_iterator_tag) {
  //取全长于前端的最大公因子
  Distance n = __gcd(last - first, middle - first);
  while (n--)
    __rotate_cycle(first, last, first + n, middle - first,
                   value_type_t<RandomAccesslIterator>());
}

// gcd:求取最大公约数（效率不如减损术）
template <class EuclideanRingElement>
EuclideanRingElement __gcd(EuclideanRingElement m, EuclideanRingElement n) {
  while (n != 0) {
    EuclideanRingElement t = m % n;
    m = n;
    n = t;
  }
  return m;
}

template <class RandomAccesslIterator, class Distance, class T>
void __rotate(RandomAccesslIterator first, RandomAccesslIterator last,
              RandomAccesslIterator initial, Distance shift, T*) {
  T value = *initial;
  RandomAccesslIterator ptr1 = initial;
  RandomAccesslIterator ptr2 = ptr1 + shift;
  while (ptr2 != initial) {
    *ptr1 = *ptr2;
    ptr1 = ptr2;
    if (last - ptr2 > shift)
      ptr2 += shift;
    else
      ptr2 = first + (shift - (last - ptr2));
  }
  *ptr1 = value;
}

// rotate_copy:并不需要执行rotate，只需要copy的时候注意次序即可
template <class ForwardIterator, class OutputIterator>
OutputIterator rotate_copy(ForwardIterator first, ForwardIterator middle,
                           ForwardIterator last, OutputIterator result) {
  return copy(first, middle, copy(middle, last, result));
}

// search:在区间S1中查找区间S2首次出现的位置，若不匹配则返回last
template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2,
                               ForwardIterator2 last2) {
  return __search(first1, last1, first2, last2, distance_type(first1),
                  dustance_type(first2));
}

template <class ForwardIterator1, class ForwardIterator2, class Distance1,
          class Distance2>
inline ForwardIterator1 search(ForwardIterator1 first1, ForwardIterator1 last1,
                               ForwardIterator2 first2, ForwardIterator2 last2,
                               Distance1*, Distance2*) {
  Distance1 d1 = 0;
  distance(first1, last1, d1);
  Distance2 d2 = 0;
  distance(first2, last2, d2);
  if (d1 < d2)  //若S2长度大于S1，不可能属于
    return last1;
  ForwardIterator1 cur1 = first1;
  ForwardIterator2 cur2 = first2;
  while (cur2 != last2)  //遍历S2
    if (*cur1 == *cur2)  //当前相同，
      ++cur1, ++cur2;    //对比下一个
    else {               //当前不同
      if (d1 == d2)
        return last1;  //若序列长度一致，则不可能成功
      else {
        cur1 = ++first1;  //调整序列，再来一次
        cur2 = first2;
        --d1;  //排除了一个元素
      }
    }
  return first1;
}

// search_n:查找“连续count个符合条件之元素”所形成的子序列，并返回该子序列起点
template <class ForwardIterator, class Integer, class T>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value) {
  if (count < 0)
    return first;
  else {
    first = find(first, last, value);  //找出下一次出现点
    while (first != last) {
      Integer n = count - 1;  // value应当再出现n次
      ForwardIterator i = first;
      ++i;
      while (i != last && n != 0 && *i == value)  //接下来也是value
        ++i, --n;
      if (n == 0)
        return first;
      else
        first = find(i, last,
                     value);  //以i为起点开始接着寻找，在i之前均没有正确起点
    }
    return last;
  }
}

// search_n:谓词版本
template <class ForwardIterator, class Integer, class T, class BinaryPredicate>
ForwardIterator search_n(ForwardIterator first, ForwardIterator last,
                         Integer count, const T& value,
                         BinaryPredicate binary_pred) {
  if (count < 0)
    return first;
  else {
    while (first != last) {
      if (binary_pred(*first, value)) break;
      ++first;
    }
    while (first != last) {
      Integer n = count - 1;  // value应当再出现n次
      ForwardIterator i = first;
      ++i;
      while (i != last && n != 0 && *i == value)  //接下来也是value
        ++i, --n;
      if (n == 0)
        return first;
      else {
        while (i != last) {
          if (binary_pred(*i, value)) break;
          ++i;
        }
        first = i;
      }
    }
    return last;
  }
}

// swap_ranges:将区间S1[first,last)内的元素与以first2为起点，长度与S1相同的区间作交换,返回指向S2中最后一个交换的元素的下一位置
//若S2实际长度较小或S1、S2重叠，执行结果未可预期
template <class ForwardIterato1, class ForwardIterator2>
ForwardIterator2 swap_ranges(ForwardIterato1 first1, ForwardIterato1 last1,
                             ForwardIterator2 first2) {
  for (; first1 != last1; ++first1, ++first2) iter_swap(first1, first2);
  return first2;
}

// transform:
//第一版本：以仿函数op作用于[first,last),并以其结果产生一个新序列
//第二版本：以仿函数binary_op作用于一双元素之上，其中一个来自[first1,last)，另一个来自[first2,...)
template <class InputIterator, class OutputIterator, class UnaryOperator>
OutputIterator transform(InputIterator first1, InputIterator last1,
                         OutputIterator result, UnaryOperator op) {
  for (; first1 != last1; ++first1, ++result) *result = op(*first1);
  return result;
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class BinaryOperator>
OutputIterator transform(InputIterator1 first1, InputIterator1 last1,
                         InputIterator1 first2, OutputIterator result,
                         BinaryOperator binary_op) {
  for (; first1 != last1; ++first1, ++first2, ++result)
    *result = binary_op(*first1, *first2);
  return result;
}

// unique:只移处相邻重复元素的去重操作
template <class ForwardIterator>
ForwardIterator unique(ForwardIterator first, ForwardIterator last) {
  first = adjacent_find(first, last);  //找到相邻重复元素的起点
  return unique_copy(first, last, first);
}

// unique_copy:提供copy操作，返回新序列的尾端
//根据输出迭代器性质作不同的处理
template <class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result) {
  if (first == last) return result;
  return __unique_copy(first, last, result, iterator_category(result));
}

// forward
template <class InputIterator, class ForwardIterator>
ForwardIterator unique_copy(InputIterator first, InputIterator last,
                            ForwardIterator result, forward_iterator_tag) {
  *result = *first;  //记录第一个元素
  while (++first != last)
    if (*result != *first)
      //不同则记录
      *++result = *first;
  return ++result;
}

// OutputIterator存在限制，必须了解value_type
template <class InputIterator, class OutputIterator>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result, output_iterator_tag) {
  return __unique_copy(first, last, result, value_type(first));
}

template <class InputIterator, class OutputIterator, class T>
OutputIterator unique_copy(InputIterator first, InputIterator last,
                           OutputIterator result, T*) {
  T value = *first;
  *result = value;
  while (++first != last)
    if (*result != *first) *++result = *first;
  return ++result;
}

// lower_bound:二分查找的一个版本，返回指向第一个不小于value的元素的迭代器。亦可认为是第一个可插入位置
//存在接受二元比较符的版本二，出于时间原因，略去不表
template <class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value) {
  return __lower_bound(first, last, value, difference_type_t<ForwardIterator>(),
                       iterator_category_t<ForwardIterator>());
}

template <class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance, forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);  //求取长度
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);  //令middle指向中间位置
    if (*middle < value) {
      first = middle;
      ++first;  //令first指向middle下一位置
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first,
                                   RandomAccessIterator last, const T& value,
                                   Distance, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

// upper_bound:二分查找的一个版本，返回可插入的最后一个位置
//存在接受二元比较符的版本二，出于时间原因，略去不表
template <class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last,
                                   const T& value) {
  return __upper_bound(first, last, value, distance_type(first),
                       iterator_category(first));
}

template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last,
                              const T& value, Distance*, forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);  //求取长度
  Distance half;
  ForwardIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);  //令middle指向中间位置
    if (value < *middle)
      len = half;
    else {
      first = middle;
      ++first;
      len = len - half - 1;
    }
  }
  return first;
}

template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first,
                                   RandomAccessIterator last, const T& value,
                                   Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    middle = first + half;
    if (value < *middle)
      len = half;
    else {
      first = middle + 1;
      len = len - half - 1;
    }
  }
  return first;
}

// binary_search:在区间内查找元素，存在则返回true，否则返回false
//本质上只需要调用lower_bound即可
template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last,
                   const T& value) {
  ForwardIterator i = lower_bound(first, last, value);
  return i != last && !(value < *i);  //这里用的是等价判定而非相等判定
}

// next_permutation:字典序下的下一个排列
//算法精要：从后向前，找出一组相邻元素，记第一元素为*i,第二元素为*ii,此二者满足*i<*ii
//再次从后向前，找出第一个大于*i的元素,记为*j，将i与j对调，再将ii之后的元素颠倒重排即可
template <class BidirectionIterator>
bool next_permutation(BidirectionIterator first, BidirectionIterator last) {
  //以下为边界判定
  if (first == last) return false;
  BidirectionIterator i = first;
  ++i;
  if (i == last)  //仅有一个元素
    return false;

  i = last;
  --i;
  for (;;) {
    BidirectionIterator ii = i;
    --i;
    if (*i < *ii) {
      BidirectionIterator j = last;
      while (!(*i < *--j))
        ;  //此时j必然存在，最不济也是ii
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {  // i已移动至队首且尚未找到ii，直接颠倒全部
      reverse(first, last);
      return false;
    }
  }
}

// pre_permutation:字典序下的上一个排列
//算法精要：从后向前，找出一组相邻元素，记第一元素为*i,第二元素为*ii,此二者满足*i>*ii
//再次从后向前，找出第一个小于*i的元素,记为*j，将i与j对调，再将ii之后的元素颠倒重排即可
template <class BidirectionIterator>
bool pre_permutation(BidirectionIterator first, BidirectionIterator last) {
  //以下为边界判定
  if (first == last) return false;
  BidirectionIterator i = first;
  ++i;
  if (i == last)  //仅有一个元素
    return false;

  i = last;
  --i;
  for (;;) {
    BidirectionIterator ii = i;
    --i;
    if (*i > *ii) {
      BidirectionIterator j = last;
      while (!(*j-- < *i))
        ;  //此时j必然存在，最不济也是ii
      iter_swap(i, j);
      reverse(ii, last);
      return true;
    }
    if (i == first) {  // i已移动至队首且尚未找到ii，直接颠倒全部
      reverse(first, last);
      return false;
    }
  }
}

// random_shuffle:将区间[first,last)内的元素随机重排，即对于存在N个元素的区间，从N！的可能性中挑出一种
//存在两个版本：一个采用内部随机数生成器，另一个则是产生随机数的仿函数，值得注意的是仿函数pass-by-reference而非value，因为该仿函数存在局部状态
template <class RandomAccessIterator>
inline void random_shuffle(RandomAccessIterator first,
                           RandomAccessIterator last) {
  __random_shuffle(first, last, distance_type(first));
}

template <class RandomAccessIterator, class Distance>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                      Distance*) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    iter_swap(i, first + Distance(rand() % ((i - first) + 1)));
}

template <class RandomAccessIterator, class RandomNumberGenerator>
void __random_shuffle(RandomAccessIterator first, RandomAccessIterator last,
                      RandomNumberGenerator& rand) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i)
    iter_swap(i, first + rand(i - first + 1));
}

// partial_sort:接收迭代器first,middle,last，使序列中的middle-first个元素以递增序置于[first,middle)中
//算法精要：将[first,middle)做成最大堆，然后将[middle，last)中的元素与max-heap中的元素比较
//若小于最大值，交换位置，并重新维持max-heap （在算法中的直接体现为pop_heap)
//因此当走完[first,last)时较大元素已被抽离[first,middle),最后再次以sort_heap对[first,middle)作出排序
template <class RandomAccessIterator>
inline void partial_sort(RandomAccessIterator first,
                         RandomAccessIterator middle,
                         RandomAccessIterator last) {
  __partial_sort(first, middle, last);
}

template <class RandomAccessIterator, class T>
inline void partial_sort(RandomAccessIterator first,
                         RandomAccessIterator middle, RandomAccessIterator last,
                         T*) {
  make_heap(first, middle);  //见heap_algorithm.h
  for (RandomAccessIterator i = middle; i != last; ++i)
    if (*i < *first) __pop_heap(first, middle, i, T(*i), distance_type(first));
  sort_heap(first, middle);
}

// partial_sort_copy：其行为类似于partial_sort,此处略过不表

// insertion_sort:插入排序
template <class RandomAccessIterator>
void __insertion_sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first == last) return;
  for (RandomAccessIterator i = first + 1; i != last; ++i)  //外循环
    __linear_insert(first, i, value_type(first));  //[first,i)形成一个子区间
}

template <class RandomAccessIterator, class T>
inline void __linear_insert(RandomAccessIterator first,
                            RandomAccessIterator last, T*) {
  T value = *last;       //记录尾元素
  if (value < *first) {  //尾元素小于头元素（头必为最小元素）
    copy_backward(first, last, last + 1);  //整个区间右移一位
    *first = value;
  } else
    __unguarded_linear_insert(last, value);
}

template <class RandomAccessIterator, class T>
void __unguarded_linear_insert(RandomAccessIterator last, T value) {
  RandomAccessIterator next = last;
  --next;
  while (*value < *next) {
    *last = *next;
    last = next;
    --next;
  }
  *last = value;
}

// QuickSort算法精要：假设S代表被处理的序列
// 1.若S中的元素个数为0或1，结束
// 2.任取S中的一个元素v作为轴（pivot)
// 3.将S分割为L,R两段，使L内的每一个元素都小于等于v，R内的任何一个元素都大于等于v
// 4.对LR递归执行Qucik sort

// median:求取三点中值
template <class T>
inline const T& __median(const T& a, const T& b, const T& c) {
  if (a < b)
    if (b < c)
      return b;  // a<b<c
    else if (a < c)
      return c;  // a<b,b>=c,a<c
    else
      return a;  // c>a>=b
  else if (a < c)
    return a;  // c>a>=b
  else if (b < c)
    return c;  // a>=b,a>=c,b<c
  else
    return b;
}

// partitioining:分割，其核心思想类似于前文算法partition
template <class RandomAccessIterator, class T>
RandomAccessIterator __unguarded_partition(RandomAccessIterator first,
                                           RandomAccessIterator last, T pivot) {
  while (true) {
    while (*first < pivot) ++first;
    --last;
    while (pivot < *last) --last;
    if (!(first < last)) return first;
    iter_swap(first, last);
    ++first;
  }
}

// threshold:对于规模较小的序列，quick sort在效率上反而低于insertion
// sort等简单算法（quick sort为了一些小序列产生了大量递归调用）
//一般来说，实际阈值选取与设想相关
//此外，quick_sort并不将所有子序列排序完毕，而是仅仅差不多即可，最终再施行一次insertion_sort，因为后者在这方面表现优异

// introsort：在快排表现良好时使用快排，在其表现不佳（分割导致了问题的恶化）时则转向使用heapsort，从而确保O(NlogN）

template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
  if (first != last) {
    __introsort_loop(first, last, value_type(first), __lg(last - first) * 2);
    __final_insertion_sort(first, last);
  }
}

//__lg:用以控制分割恶化的情况，找出2^k<=n的最大值k
//举例而言，当size=40时，__introsort_loop的最后一个参数为10，即最多允许分割10层
template <class Size>
inline Size __lg(Size n) {
  Size k;
  for (k = 0; n > 1; n >>= 1) ++k;
  return k;
}

//__introsort_loop：intosort的具体实现
//结束排序后，[first,last)内有多个“元素个数少于16”的子序列，每个子序列有一定程序的排序，但尚未完全排序
template <class RandomAccessIterator, class T, class Size>
void __introsort_loop(RandomAccessIterator first, RandomAccessIterator last, T*,
                      Size depth_limit) {
  //__STL_threshold是一个定义为16的全局常数
  while (last - first > /*__stl_threshold*/ 16) {
    if (depth_limit == 0) {             //已经产生了分割恶化
      partial_sort(first, last, last);  //改用heap-sort
      return;
    }
    --depth_limit;
    RandomAccessIterator cut = __unguarded_partition(
        first, last,
        T(__median(*first, *(first + (last - first) / 2), *(last - 1))));
    __introsort_loop(cut, last, value_type(first), depth_limit);
    last = cut;  //回归while，执行左侧排序
  }
}

//__finial_insertion_sort:最终的插入排序
//首先判断元素个数是否大于16，若答案为是，则将其分为两部分
template <class RandomAccessIterator>
void __final_insertion_sort(RandomAccessIterator first,
                            RandomAccessIterator last) {
  if (last - first > /*__stl_threshold*/ 16) {
    __insertion_sort(first, first + /*__stl_threshold*/ 16);
    __unguarded_insertion_sort(first + /*__stl_threshold, last*/ 16);
  } else
    __insertion_sort(first, last);
}

template <class RandomAccessIterator>
inline void __unguarded_insertion_sort(RandomAccessIterator first,
                                       RandomAccessIterator last) {
  __unguarded_insertion_sort_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class T>
void __unguarded_insertion_sort_aux(RandomAccessIterator first,
                                    RandomAccessIterator last, T*) {
  for (RandomAccessIterator i = first; i != last; ++i)
    __unguarded_linear_insert(i, T(*i));
}

/*
// 以下为RW STL sort,并不设立阈值而是采用纯粹的快排
template <class RandomAccessIterator>
inline void sort(RandomAccessIterator first, RandomAccessIterator last) {
        if (!(first == last)) {
                __quick_sort_loop(first, last);
                __final_insertion_sort(first, last);
        }
}

template <class RandomAccessIterator>
inline void __quick_sort_loop(RandomAccessIterator first, RandomAccessIterator
last) {
        __quick_sort_loop_uax(first, last, value_type(first));
}

template <class RandomAccessIterator, class T>
inline void __quick_sort_loop_aux(RandomAccessIterator first,
RandomAccessIterator last) { while (last - first > __stl_thresold) {
                RandomAccessIterator cut = __unguarded_partition(first, last,
T(__median(*first, *(first + (last - first) / 2), *(last - 1)))); if (cut -
first >= last - cut) {
                        __quick_sort_loop(cut, last);
                        last = cut;
                }
                else {
                        __quick_sort_loop(first, cut);
                        first = cut;
                }
        }
}
*/

// equal_range:本质上返回了lower_bound与upper_bound组成的pair
template <class ForwardIterator, class T>
inline pair<ForwardIterator, ForwardIterator> equal_range(ForwardIterator first,
                                                          ForwardIterator last,
                                                          const T& value) {
  return __equal_range(first, last, value, distance_type(first),
                       iterator_category(first));
}

template <class RandomAccessIterator, class T, class Distance>
inline pair<RandomAccessIterator, RandomAccessIterator> __equal_range(
    RandomAccessIterator first, RandomAccessIterator last, const T& value,
    Distance*, random_access_iterator_tag) {
  Distance len = last - first;
  Distance half;
  RandomAccessIterator middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first + half;
    if (*middle < value) {
      first = middle + 1;
      len = len - half - 1;
    } else if (value < *middle)
      len = half;
    else {  //中央元素等于指定值
      left = lower_bound(first, middle, value);
      right = upper_bound(++middle, first + len, value);
      return pair<RandomAccessIterator, RandomAccessIterator>(left, right);
    }
  }
  //并未找到该元素,指向第一个大于value的元素
  return pair<RandomAccessIterator, RandomAccessIterator>(first, first);
}

template <class ForwardIterator, class T, class Distance>
inline pair<ForwardIterator, ForwardIterator> __equal_range(
    ForwardIterator first, ForwardIterator last, const T& value, Distance*,
    forward_iterator_tag) {
  Distance len = 0;
  distance(first, last, len);
  Distance half;
  ForwardIterator middle, left, right;

  while (len > 0) {
    half = len >> 1;
    middle = first;
    advance(middle, half);
    if (*middle < value) {
      first = middle;
      ++first;
      len = len - half - 1;
    } else if (value < *middle)
      len = half;
    else {  //中央元素等于指定值
      left = lower_bound(first, middle, value);
      advance(first, len);
      right = upper_bound(++middle, first, value);
      return pair<ForwardIterator, ForwardIterator>(left, right);
    }
  }
  //并未找到该元素,指向第一个大于value的元素
  return pair<ForwardIterator, ForwardIterator>(first, first);
}

// inplace_merge:stable，在存在缓冲区的情况下性能较优
template <class BidirectionalIterator>
inline void inplace_merge(BidirectionalIterator first,
                          BidirectionalIterator middle,
                          BidirectionalIterator last) {
  if (first == middle || middle == last) return;
  __inplace_merge_aux(first, middle, last, value_type(first),
                      distance_type(first));
}

template <class BidirectionalIterator, class T, class Distance>
inline void __inplace_merge_aux(BidirectionalIterator first,
                                BidirectionalIterator middle,
                                BidirectionalIterator last, T*, Distance*) {
  Distance len1 = 0;
  distance(first, middle, len1);
  Distance len2 = 0;
  distance(middle, last, len2);

  temporary_buffer<BidirectionalIterator, T> buf(first, last);  //临时缓冲区
  if (buf.begin() == 0)
    __merge_without_buffer(first, middle, last, len1, len2);
  else
    __merge_adaptive(first, middle, last, len1, len2, buf.begin(),
                     Distance(buf.size()));
}

template <class BidirectionalIterator, class Distance, class Pointer>
void __merge_adaptive(BidirectionalIterator first, BidirectionalIterator middle,
                      BidirectionalIterator last, Distance len1, Distance len2,
                      Pointer buffer, Distance buffer_size) {
  if (len1 <= len2 && len1 <= buffer.size()) {
    //缓冲区足以安置序列一
    Pointer end_buffer = copy(first, middle, buffer);
    merge(buffer, end_buffer, middle, last, first);
  } else if (len2 <= buffer_size) {
    //缓冲区足以安置序列二
    Pointer end_buffer = copy(first, middle, buffer);
    __merge_backward(first, middle, buffer, end_buffer, last);
  } else {
    //缓冲区不足以安置任何一个序列
    BidirectionalIterator first_cut = first;
    BidirectionalIterator second_cut = middle;
    Distance len11 = 0;
    Distance len22 = 0;
    if (len1 > len2) {  //哪个序列长就分割哪个
      len11 = len1 / 2;
      advance(first_cut, len11);
      second_cut = lower_bound(middle, last, *first_cut);
      distance(middle, second_cut, len22);
    } else {
      len22 = len2 / 2;
      advance(second_cut, len22);
      first_cut = upper_bound(first, middle, *second_cut);
      distance(first, first_cut, len11);
    }
    BidirectionalIterator new_middle =
        __rotate_adaptive(first_cut, middle, second_cut, len1 - len11, len22,
                          buffer, buffer, buffer_size);
    //针对左端递归
    __merge_adaptive(first, first_cut, new_middle, len11, len22, buffer,
                     buffer_size);
    //针对右端递归
    __merge_adaptive(new_middle, second_cut, last, len1 - len11, len2 - len22,
                     buffer, buffer_size);
  }
}

template <class BidirectionalIterator1, class BidirectionalIterator2,
          class Distance>
BidirectionalIterator1 __rotate_adaptive(BidirectionalIterator1 first,
                                         BidirectionalIterator1 middle,
                                         BidirectionalIterator1 last,
                                         Distance len1, Distance len2,
                                         BidirectionalIterator2 buffer,
                                         Distance buffer_size) {
  BidirectionalIterator2 buffer_end;
  if (len1 > len2 && len2 <= buffer_size) {
    //缓冲区足以安置序列2
    buffer_end = copy(middle, last, buffer);
    copy_backward(first, middle, last);
    return copy(buffer, buffer_end, first);
  } else if (len1 <= buffer_size) {
    buffer_end = copy(first, middle, buffer);
    copy(middle, last, first);
    return copy_backward(buffer, buffer_end, last);
  } else {
    //缓冲区仍然不足
    rotate(first, middle, first);
    advance(first, len2);
    return first;
  }
}

// nth_element:重新排序[first,last)，使得nth指向的元素与完全排列后同一位置的元素同值
// nth_element还保证[nth,last）内的元素必然不大于nth，但对于[first,nth)与[nth,last)中的序列则毫无保证
//由此看来，nth_element更类似于partition而非partial_sort(后者采用heap_sort)
template <class RandomAccessIterator>
inline void nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                        RandomAccessIterator last) {
  __nth_element(first, nth, last, value_type(first));
}

template <class RandomAccessIterator, class T>
void __nth_element(RandomAccessIterator first, RandomAccessIterator nth,
                   RandomAccessIterator last, T*) {
  while (last - first > 3) {
    //三点中值法分割
    //返回一个迭代器，指向分割后右侧的第一个元素
    RandomAccessIterator cut = __unguarded_partition(
        first, last,
        T(__median(*first, *(first + (last - first) / 2, *(last - 1)))));
    if (cut <= nth)
      first = cut;  //右端起点<=nth,再次对右侧分割
    else
      last = cut;  //对左侧分割
  }
  __insertion_sort(first, last);
}

// mergesort::调用inplace_merge完成归并排序，需要额外的缓冲区，此外在内存见不断移动（复制）元素亦需要较高成本，弱于quick_sort
template <class BidirectionalIter>
void mergesort(BidirectionalIter first, BidirectionalIter last) {
  typename iterator_traits<BidirectionalIter>::difference_type n =
      distance(first, last);
  if (n == 0 || n == 1)
    return;
  else {
    BidirectionalIter mid = first + n / 2;
    mergesort(first, mid);
    mergesort(mid, last);
    inplace_merge(first, mid, last);
  }
}

}  // namespace MiniSTL