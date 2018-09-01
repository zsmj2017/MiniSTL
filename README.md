MiniSTL
=======
本仓库原是《STL源码剖析》一书的代码合集，并在其基础上运用部分C++11特性进行了代码改写。最终希望根据SGI STL SOURCE完成一个简易STL标准库。

## 学习计划

针对SGI STL Source，有学习计划如下：

 - [x] 第一阶段：通读全文，了解数据结构的底层实现与算法流程，针对书本完成代码撰写与整理。
 - [ ] 第二阶段：补全各容器的实现（进行中）。
 - [ ] 第三阶段：针对容器类加入C++11新特性，如move语义，initialized_list构造等,撰写完备的单元测试。
 - [ ] 第四阶段：测试MiniSTL性能，并与std作对比，总结原因。

 
## 完成进度

 1. **空间适配器（allocator）**

 &emsp;构造析构工具（contruct.h):100%  

 &emsp;二级空间适配器(alloc.h):100%  
 
 &emsp;内存基本处理工具(uninitialized.h):100%  
 2. **迭代器与traits编程技法（iterator && traits)**

 &emsp;iterator_traits（iterator.h):100%  
 
 &emsp;type_traits(type_traits.h):100%  
 3. **序列式容器（sequence containers)**

 &emsp;vector（stl_vector.h):50%  
 
 &emsp;list(stl_list.h):50%   
 
 &emsp;deque(stl_deque.h):50%  
 
 &emsp;stack(stl_stack.h):50%  
 
 &emsp;queue(stl_queue.h):50%  
 
 &emsp;heap(heap_algorithm.h):100%  
 
 &emsp;priority_queue(priority_queue.h):50%  
 
 &emsp;slist(slist.h):50%
 4. **关联式容器(associattive containers)**

 &emsp;RB-TREE（rb_tree.h):50%  
 
 &emsp;set(stl_set.h):50%  
 
 &emsp;map(stl_map.h):50%  
 
 &emsp;multiset(stl_multiset.h):50%  
 
 &emsp;multimap(stl_multimap.h):50%  
 
 &emsp;hashtable(hashtable.h):50%  
 
 &emsp;hashset(hashset.h):50%  
 
 &emsp;hashmultiset(hash_multiset.h):50%  
 
 &emsp;hashmultimap(hash_multimap.h):50%  
 5. **算法（algorithm)**

 &emsp;数值算法（stl_numeric.h):100%  
 
 &emsp;基本算法(stl_algobase.h):100%  
 
 &emsp;set相关算法(stl_algoset.h):100%  
 
 &emsp;其他算法(stl_algo.h):100%
 6. **仿函数（function object)**

 &emsp;算术、关系运算、逻辑运算仿函数（stl_function.h):100%

 7. **配接器（adapter)**

 &emsp;容器类配接器（`stack`、`queue`):100%  

 &emsp;迭代器配接器（`insert_iterator`、`reverse_iterator`、`istream_iterator`):100%  
 
 &emsp;仿函数配接器（`not1`、`not2`、`bind1st`、`bind2nd`、`compose1`、`compose2`、`ptr_fun`...):100%

## 编译环境

本仓库代码编译环境为windows10，VS2017。

## LICENSE

MIT.

