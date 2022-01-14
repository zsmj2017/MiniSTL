#pragma once

#include <functional>// TODO::for std::mutiplies

//累积算法
template<class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init = T()) {
  for (; first != last; ++first) init += *first;
  return init;
}

template<class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init,
             BinaryOperation binary_op) {//二元操作符,不必满足交换律、结合律
  for (; first != last; ++first) binary_op(init, *first);
  return init;
}

//保留#1号元素，#n=#n-#(n-1),当result==first时为就地质变算法
template<class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result) {
  if (first == last) return first;
  *result = *first;
  return _adjacent_difference(first, last, result, value_type(first));
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result, T *) {
  T value = *result;
  while (++first != last) {
    T temp = *first;
    *++result = temp - value;
    value = temp;
  }
  return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result,
                                   BinaryOperation binary_op) {
  if (first == last) return first;
  *result = *first;
  return _adjacent_difference(first, last, result, value_type(first),
                              binary_op);
}

template<class InputIterator, class OutputIterator, class T,
         class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last,
                                   OutputIterator result, T *,
                                   BinaryOperation binary_op) {
  T value = *result;
  while (++first != last) {
    T temp = *first;
    *++result = binary_op(temp, value);
    value = temp;
  }
  return ++result;
}

//序列内积
template<class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator2 last1,
                InputIterator2 first2, T init = T()) {
  for (; first1 != last1; ++first1, ++first2) init += (*first1) * (*first2);
  return init;
}

template<class InputIterator1, class IutputIterator2, class T,
         class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1,
                IutputIterator2 first2, T init, BinaryOperation1 binary_op1,
                BinaryOperation2 binary_op2) {
  for (; first1 != last1; ++first1, ++first2)
    init = binary_op1(init, binary_op2(*first1, *first2));
  return init;
}

//第n个元素是前n个旧元素的累积相加，与adjacent_difference互为逆运算
template<class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result) {
  if (first == last) return first;
  *result = *first;
  return _partial_sum(first, last, result, value_type(first));
}

template<class InputIterator, class OutputIterator, class T>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, T *) {
  T value = *first;
  while (++first != last) {
    value += *first;
    *++result = value;
  }
  return ++result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, BinaryOperation binary_op) {
  if (first == last) return first;
  *result = *first;
  return _partial_sum(first, last, result, value_type(first), binary_op);
}

template<class InputIterator, class OutputIterator, class T,
         class BinaryOperation>
OutputIterator partial_sum(InputIterator first, InputIterator last,
                           OutputIterator result, T *,
                           BinaryOperation binary_op) {
  T value = *result;
  while (++first != last) {
    value = binary_op(value, *first);
    *++result = value;
  }
  return ++result;
}

//对x反复执行n次算数操作
template<class T, class Integer>
inline T power(T x, Integer n) {
  return power(x, n, std::mutiplies<T>());
}

template<class T, class Integer, class MonoidOperation>
inline T power(T x, Integer n, MonoidOperation op) {
  if (n == 0)
    return identity_element(
        op);//取出证同元素（该元素与A作运算将得到A自身，例如加法的证同为0，乘法为1，等等）
  else {
    while ((n & 1) == 0) {//剥离偶数因子
      n >>= 1;
      x = op(x, x);
    }
    T result = x;
    n >>= 1;// n=(n-1)/2
    while (n != 0) {
      x = op(x, x);
      if ((n & 1) != 0)//依然存在奇数位
        result = op(result, x);
      n >>= 1;
    }
    return result;
  }
}

// SGI专属，令指定区间从value起呈现递增状态，质变算法
template<class ForwardIterator, class T>
void itoa(ForwardIterator first, ForwardIterator last, T value) {
  while (first != last) *first++ = value++;
}
