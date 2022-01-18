MiniSTL
=======
本仓库原是《STL源码剖析》一书的代码合集，并在其基础上运用部分C++11特性进行了代码改写。最终希望根据SGI STL SOURCE完成一个简易STL标准库。

## 学习计划

针对SGI STL Source，有学习计划如下：

 - [x] 第一阶段：通读全文，了解数据结构的底层实现与算法流程，针对书本完成代码撰写与整理。
 - [x] 第二阶段：补全各容器的实现。
 - [ ] 第三阶段：针对容器类加入C++11新特性，如move语义，initialized_list构造等,撰写完备的单元测试。(进行中)
 - [ ] 第四阶段：测试MiniSTL性能，并与std作对比，总结原因。


## 完成进度

 1. **空间适配器（allocator）**

 &emsp;[构造析构工具(contruct.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Allocator/subAllocation/construct.h):100%  

 &emsp;[二级空间适配器(alloc.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Allocator/subAllocation/alloc.h):100%  

 &emsp;[内存基本处理工具(uninitialized.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Allocator/subAllocation/uninitialized.h):100%  

 2. **迭代器与traits编程技法（iterator && traits)**

 &emsp;[iterator_traits(stl_iterator.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Iterator/stl_iterator.h):100%  

 &emsp;[type_traits(type_traits.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Iterator/typeTraits.h):100%  

 3. **序列式容器（sequence containers)**

 &emsp;[vector(stl_vector.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Vector/stl_vector.h):100%  

 &emsp;[list(stl_list.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/List/stl_list.h):100%   

 &emsp;[deque(stl_deque.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Deque/stl_deque.h):100%  

 &emsp;[stack(stl_stack.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Stack/stl_stack.h):100%  

 &emsp;[queue(stl_queue.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Queue/stl_queue.h):100%   

 &emsp;[priority_queue(stl_priority_queue.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/PriorityQueue/stl_priority_queue.h):100%  

 &emsp;[slist(slist.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Slist/slist.h):100%  

 4. **关联式容器(associattive containers)**

 &emsp;[rb_tree(rb_tree.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/RB-Tree/rb_tree.h):100%  

 &emsp;[set(stl_set.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Set/stl_set.h):100%  

 &emsp;[map(stl_map.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Map/stl_map.h):100%  

 &emsp;[multiset(stl_multiset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Mutiset/stl_mutiset.h):100%  

 &emsp;[multimap(stl_multimap.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Mutimap/stl_mutimap.h):100%  

 &emsp;[hashtable(hashtable.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Hashmap/hash_map.h):100%  

 &emsp;[hashset(hashset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Hashset/hash_set.h):100%  

 &emsp;[hashmultiset(hash_multiset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/HashMultiset/hash_multiset.h):100%  

 &emsp;[hashmultimap(hash_multimap.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/HashMultiset/hash_multimap.h):100%  

 5. **算法（algorithm)**

 &emsp;[数值算法(stl_numeric.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/numeric/stl_numeric.h):100%  

 &emsp;[基本算法(stl_algobase.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/algobase/stl_algobase.h):100%  

 &emsp;[set相关算法(stl_algoset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/algoset/stl_algoset.h):100%  

 &emsp;[heap算法(heap_algorithm.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/heap/heap_algorithm.h):100% 

 &emsp;[其他算法(stl_algo.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/algo/stl_algo.h):100%  

 6. **仿函数（function object)**

 &emsp;[算术、关系运算、逻辑运算仿函数(stl_function.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Function/stl_function.h):100%

 7. **配接器（adapter)**

 &emsp;容器类配接器（`stack`、`queue`):100%  

 &emsp;迭代器配接器（`insert_iterator`、`reverse_iterator`、`istream_iterator`):100%  

 &emsp;仿函数配接器（`not1`、`not2`、`bind1st`、`bind2nd`、`compose1`、`compose2`、`ptr_fun`...):100%

## 编译环境

本仓库代码编译环境为macOS Big Sur 11.1，Apple clang version 12.0.0。

## LICENSE

MIT.

