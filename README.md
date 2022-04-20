MiniSTL
=======
本仓库原是《STL源码剖析》一书的代码合集，并在其基础上运用部分C++11特性进行了代码改写。  
最终希望根据SGI STL SOURCE完成一个简易STL标准库。

## 学习计划

针对SGI STL Source，有学习计划如下：

 - [x] 第一阶段：通读全文，了解数据结构的底层实现与算法流程，针对书本完成代码撰写与整理。
 - [x] 第二阶段：补全各容器的实现。
 - [ ] 第三阶段：针对容器类加入C++11新特性，如move语义，initialized_list构造等,撰写完备的单元测试。(进行中)
 - [ ] 第四阶段：测试MiniSTL性能，并与std作对比，总结原因。

## 目录

 1. **空间适配器（allocator）**

&emsp;[构造析构工具(contruct.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Allocator/subAllocation/construct.h)

&emsp;[二级空间适配器(alloc.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Allocator/subAllocation/alloc.h)

&emsp;[内存基本处理工具(uninitialized.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Allocator/subAllocation/uninitialized.h)

2. **迭代器与traits编程技法（iterator && traits)**

&emsp;[iterator_traits(stl_iterator.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Iterator/stl_iterator.h)

&emsp;[type_traits(type_traits.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Utils/type_traits.h)

3. **序列式容器（sequence containers)**

&emsp;[vector(stl_vector.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Vector/stl_vector.h)

&emsp;[list(stl_list.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/List/stl_list.h)

&emsp;[deque(stl_deque.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Deque/stl_deque.h)

&emsp;[stack(stl_stack.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Stack/stl_stack.h)

&emsp;[queue(stl_queue.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Queue/stl_queue.h)

&emsp;[priority_queue(stl_priority_queue.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/PriorityQueue/stl_priority_queue.h)

&emsp;[slist(slist.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SequenceContainers/Slist/slist.h)

4. **关联式容器(associattive containers)**

&emsp;[rb_tree(rb_tree.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/RB-Tree/rb_tree.h)

&emsp;[set(stl_set.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Set/stl_set.h)

&emsp;[map(stl_map.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Map/stl_map.h)

&emsp;[multiset(stl_multiset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Mutiset/stl_mutiset.h)

&emsp;[multimap(stl_multimap.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Mutimap/stl_mutimap.h)

&emsp;[hashtable(hashtable.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Hashmap/hash_map.h)

&emsp;[hashset(hashset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/Hashset/hash_set.h)

&emsp;[hashmultiset(hash_multiset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/HashMultiset/hash_multiset.h)

&emsp;[hashmultimap(hash_multimap.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/AssociativeContainers/HashMultiset/hash_multimap.h)

5. **算法（algorithm)**

&emsp;[数值算法(stl_numeric.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/numeric/stl_numeric.h)

&emsp;[基本算法(stl_algobase.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/algobase/stl_algobase.h)

&emsp;[set相关算法(stl_algoset.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/algoset/stl_algoset.h)

&emsp;[heap算法(heap_algorithm.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/heap/heap_algorithm.h)

&emsp;[其他算法(stl_algo.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Algorithms/algo/stl_algo.h)

6. **仿函数（function object)**

&emsp;[算术、关系运算、逻辑运算仿函数(stl_function.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/Function/stl_function.h)

7. **配接器（adapter)**

&emsp;容器类配接器（`stack`、`queue`)

&emsp;迭代器配接器（`insert_iterator`、`reverse_iterator`、`istream_iterator`)

&emsp;仿函数配接器（`not1`、`not2`、`bind1st`、`bind2nd`、`compose1`、`compose2`、`ptr_fun`...)

8. **智能指针(smart_pointer)**

&emsp;[shared_ptr(shared_ptr.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SmartPointer/shared_ptr.h)  
&emsp;[unique_ptr(unique_ptr.h)](https://github.com/zsmj2017/MiniSTL/blob/master/Source/SmartPointer/unique_ptr.h)

## 编译环境

OS:macOS Monterey 12.1

CXX_Compiler:Apple clang version 13.0.0

Cmake:3.21.4

## 编译流程

1. git clone this repo
2. ```shell
   cd $work_path/MiniSTL && mkdir build && cd build;
   cmake .. && make;
   ```
3. `libSTL.a` is in `$work_path/MiniSTL/build/Source`

## 运行单测

0. **run unit_test need install gtest**
1. git clone this repo
2. ```shell
   cd $work_path/MiniSTL && mkdir build && cd build;
   cmake .. -DENABLE_MINISTL_TEST=ON && make;
   ```
3. run exec file `TEST` in `$work_path/MiniSTL/build/Test` 

## LICENSE

MIT.

