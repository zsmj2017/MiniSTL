#pragma once

//本头文件收录了4个set算法（此set并非STL set容器）
//四大算法分别为：并集、交集、差集、对称差集

//要求S1、S2有序
//不要求S1、S2元素唯一，对于元素x，其若在S1内出现n次，在S2内出现m次，则将在输出中出现max(m,n)次，其中n个来自S1，其余来自S2
// Stable
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1;
        } else if (*first1 > *first2) {
            *result = *first2;
            ++first2;
        } else {
            *result = *first1;
            ++first1, ++first2;
        }
        ++result;
    }
    //此时[first1,last1)与[first2,last2)必存在空白区间
    return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_union(InputIterator1 first1, InputIterator1 last1,
                         InputIterator2 first2, InputIterator2 last2,
                         OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
                if (comp(*first1,*first2) {
            *result = *first1;
            ++first1;
        }
        else if (comp(*first2,*first1) {
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1, ++first2;
        }
        ++result;
    }
    //此时[first1,last1)与[first2,last2)必存在空白区间
    return copy(first2, last2, copy(first1, last1, result));
}

//要求S1、S2有序
//不要求S1、S2元素唯一，对于元素x，其若在S1内出现n次，在S2内出现m次，则将在输出中出现min(m,n)次，其中n个全部来自S1
// Stable
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            ++first1;
        } else if (*first1 > *first2) {
            ++first2;
        } else {
            *result = *first1;
            ++first1, ++first2, ++result;
        }
    }
    return result;
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
                if (comp(*first1, *first2) {
            ++first1;
        }
        else if (comp(*first2, *first1) {
            ++first2;
        }
        else {
            *result = *first1;
            ++first1, ++first2, ++result;
        }
    }
    return result;
}

//求差集 即S1-S2 出现于S1但并不出现于S2的所有元素集合
// Stable
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_intersection(InputIterator1 first1, InputIterator1 last1,
                                InputIterator2 first2, InputIterator2 last2,
                                OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1, ++result;
        } else if (*first1 > *first2)
            ++first2;
        else
            ++first1, ++first2;
    }
    //若存在剩余，拷贝之
    return copy(first1, last1, result);
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_difference(InputIterator1 first1, InputIterator1 last1,
                              InputIterator2 first2, InputIterator2 last2,
                              OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1, ++result;
        } else if (comp(*first2, *first1))
            ++first2;
        else
            ++first1, ++first2;
    }
    return copy(first1, last1, result);
}

//求对称差集，即（S1-S2）U（S2-S1)
// Stable
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator set_symmetric_difference(InputIterator1 first1,
                                        InputIterator1 last1,
                                        InputIterator2 first2,
                                        InputIterator2 last2,
                                        OutputIterator result) {
    while (first1 != last1 && first2 != last2) {
        if (*first1 < *first2) {
            *result = *first1;
            ++first1, ++result;
        } else if (*first1 > *first2) {
            *result = *first2;
            ++first2, ++result;
        } else
            ++first1, ++first2;
    }
        return copy(first2, last2, copy(first1,last1,result);
}

template <class InputIterator1, class InputIterator2, class OutputIterator,
          class Compare>
OutputIterator set_symmetric(InputIterator1 first1, InputIterator1 last1,
                             InputIterator2 first2, InputIterator2 last2,
                             OutputIterator result, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if (comp(*first1, *first2)) {
            *result = *first1;
            ++first1, ++result;
        } else if (comp(*first2, *first1)) {
            *result = *first2;
            ++first2, ++result;
        } else
            ++first1, ++first2;
    }
        return copy(first2, last2, copy(first1, last1, result);
}
