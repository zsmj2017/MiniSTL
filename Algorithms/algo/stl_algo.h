#pragma once

//adjacent_find:找出一组满足条件的相邻元素
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
	if (first == last) retuen last;
	ForwardIterator next = first;
	while (++next != last) {
		if (*first == *next) return first;
		else first = next;
	}
	return last;
}

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
	if (first == last) retuen last;
	ForwardIterator next = first;
	while (++next != last) {
		if (binary_pred(*first,*next) return first;
		else first = next;
	}
	return last;
}

//count:运用equality判断有几个与value相等的元素
template <class InputIterator , class T>
typename iterator_traits<InputIterator>::difference_type
count(InputIterator first, InputIterator last,T value) {
	typename iterator_traits<InputIterator>::difference_type count = 0;
	for (; first != last; ++first)
		if (*first == value)
			count++;
	return count;
}

//count_if:将指定操作实施于整个区间，并将“令pred的计算结果为true”的元素个数返回
template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type
count_if(InputIterator first, InputIterator last, Predicate pred) {
	typename iterator_traits<InputIterator>::difference_type count = 0;
	for (; first != last; ++first)
		if (pred(*first)
			count++;
	return count;
}

//count:运用equality查找元素
template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, T value) {
	while (first != last && *first != value) ++first;
	return first;
}

//count_if:将指向“令pred的计算结果为true”的第一个元素的迭代器返回
template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
	while (first != last && pred(*first)) ++first;
	return first;
}

//find_end:在区间S1[first1,last1)中找出区间S2[first2,last2)的最后出现点，若不存在，返回last1
//显然，若迭代器支持逆向查找则较为便利
template <class ForwardIterator1,class ForwardIterator2>
inline ForwardIterator1
find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
	using category1 = typename iterator_traits<ForwardIterator1>::iterator_category;
	using category2 = typename iterator_traits<ForwardIterator2>::iterator_category;
	return __find_end(first1, last1, first2, last2, category1(), category2());
}

template <class ForwardIterator1, class ForwardIterator2>
ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2
							forward_iterator_tag,forward_iterator_tag) {
	if (first2 == last2)
		return last1;
	else {
		ForwardIterator1 result = last1;
		while (1) {
			ForwardIterator1 new_result = search(first1, last1, first2, last2);//利用search查找首次出现点
			if (new_result == last1)
				return result;
			else {
				result = new_result;//更新result
				first1 = new_result;
				++first1;//更换搜索位置
			}
		}
	}
}

template <class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator1 first2, BidirectionalIterator1 last2
						bidirectional_iterator_tag, bidirectional_iterator_tag) {
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
		advance(result, -distance(first2, last2));//回归子序列头部
		return result;
	}
}

//find_first_of:以S2区间内某些元素为目标，寻找它们在S1区间内首次出现地点。
template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
	for (; first1 != last1; ++first1)
		for (ForwardIterator iter = first2; iter != last2; ++iter)
			if (*first1 == *iter) return first1;
	return last1;
}

//for_each:将f作用于区间元素之上，不能改变元素，欲改变应当使用transform
//具备一个通常被忽略的返回值
template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last,Function f) {
	for (; first != last; ++first)
		f(*first);
	return f;
}

//generate:将gen的运算结果填写于[first,last)，采用assignment
template <class InputIterator, class Generator>
void generate(InputIterator first, InputIterator last, Generator gen) {
	for (; first != last; ++first)
		*first = gen();
}

//generate_n:将gen的运算结果填写于起点为first，长度为n的区间内，采用assignment
template <class OutputIterator, class Size, class Generator>
OutputIterator generate(OutputIterator first, Size n, Generator gen) {
	for (; n > 0; --n, ++first)
		*first = gen();
	return first;
}

//include：判断S2是否被S1所包含,要求有序，默认为升序
template <class InputIterator1,class InputIterator2>
bool include(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
	while (first1!=last1 && first2!=last2){
		if (*first2 < *first1)//一个不在则不可能包含
			return false;
		else if (*first1 < *first2)
			++first1;
		else
			++first1, ++first2;
	}
	return first2==last2;
}

//comp必须满足等价判定表达式（严格弱序或升序）
template <class InputIterator1, class InputIterator2, class Compare>
bool include(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2,*first1))
			return false;
		else if (comp(*first1,*first2))
			++first1;
		else
			++first1, ++first2;
	}
	return first2==last2;
}

//max_element:返回指向序列中数值最大元素的迭代器
template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*result < *first)
			result = first;
	return result;
}

template <class ForwardIterator,class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*result,*first)
			result = first;
	return result;
}

//min_element:返回指向序列中数值最小元素的迭代器
template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last)
		if (*first < *result)
			result = first;
	return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
	if (first == last)
		return first;
	ForwardIterator result = first;
	while (++first != last)
		if (comp(*first, *result))
			result = first;
	return result;
}

//merge:将有序的S1与S2合并置于另一段空间，合并结果仍然有序，返回一个迭代器指向新区间的last
template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
	while (first1 != last1 && first2 != last2) {
		if (*first2 < *first1)
			*result++ = *first2, ++first2;
		else
			*result++ = *first1, ++first1;
	}
	return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, 
					InputIterator2 last2, OutputIterator result,Compare comp) {
	while (first1 != last1 && first2 != last2) {
		if (comp(*first2,*first1))
			*result++ = *first2, ++first2;
		else
			*result++ = *first1, ++first1;
	}
	return copy(first2, last2, copy(first1, last1, result));
}