#pragma once

//两个形参表征vector头尾，当前元素已作为vector尾部元素
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
	__push_heap_aux(first, last, distance_type(first),value_type(first));
}

template <class RandomAccessIterator, class Distance, class T>
inline void __push_heap_aux(RandomAccessIterator first,RandomAccessIterator last, Distance*, T*) {
	//容器最尾端即为第一个洞号(last-first)-1
	__push_heap(first, Distance((last - first) - 1), Distance(0),T(*(last - 1)));
}

template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex,Distance topIndex, T value) {
	Distance parent = (holeIndex - 1) / 2;//找出父节点
	while (holeIndex > topIndex && *(first + parent) < value) {//尚未到达顶端且其值大于父节点
		*(first + holeIndex) = *(first + parent);//令洞值为父值
		holeIndex = parent;
		parent = (holeIndex - 1) / 2;//更新父节点
	}
	//结束循环时更新洞值
	*(first + holeIndex) = value;
}


//将heap的根节点（最大值）取走（其实放置于vector尾部）
template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,RandomAccessIterator last) {
	__pop_heap_aux(first, last, value_type(first));
}

template <class RandomAccessIterator, class T>
inline void __pop_heap_aux(RandomAccessIterator first,RandomAccessIterator last, T*) {
	//在[first,last-1)间执行__pop_heap
	__pop_heap(first, last - 1, last - 1, T(*(last - 1)), distance_type(first));
}

template <class RandomAccessIterator, class T, class Distance>
inline void __pop_heap(RandomAccessIterator first,RandomAccessIterator last,RandomAccessIterator result,T value, Distance*) {
	*result = *first;//将尾端设置为取出值,而原本的尾端值变为了value
	__adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex,Distance len, T value) {
	Distance topIndex = holeIndex;
	Distance secondChild = 2 * holeIndex + 2;//洞点右子
	while (secondChild < len) {//当前尚存在右子
		//自此secondChild代表较大子节点
		if (*(first + secondChild) < *(first + (secondChild - 1)))//右子小于左子
			secondChild--;
		*(first + holeIndex) = *(first + secondChild);//将洞点赋值为较大子值
		holeIndex = secondChild;//更新洞点
		secondChild = 2 * (holeIndex + 1);//再次指向洞点右子
	}
	if (secondChild == len) {//不存在右子，令其左子值为洞值，并将洞号传递至左子（左子必为除首节点外最大者）
		*(first + holeIndex) = *(first + (secondChild - 1));
		holeIndex = secondChild - 1;
	}
	//将原有的末端元素填入（更新那个洞
	__push_heap(first, holeIndex, topIndex, value);
}

//堆排序（不断调用pop_heap即可）
template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first,RandomAccessIterator last) {
	while (first!=last)
		pop_heap(first, last--);
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first,RandomAccessIterator last) {
	__make_heap(first, last, value_type(first), distance_type(first));
}

template <class RandomAccessIterator, class T, class Distance>
void __make_heap(RandomAccessIterator first,RandomAccessIterator last, T*,Distance*) {
	//遍历所有有资格为root的点
	if (last - first < 2) return;
	Distance len = last - first;
	//找到需要重排的头部（即为最后一个有资格为root的点）
	Distance holeIndex = (len - 2) / 2;
	while (true) {
		//重排以holeIndex为首的子树,len以防越界
		__adjust_heap(first, holeIndex, len, T(*(first + holeIndex)));
		if (holeIndex == 0) return;
		holeIndex--;
	}
}