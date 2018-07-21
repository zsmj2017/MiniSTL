#pragma once

#if 0
#include <new>
#define __THROW_BAD_ALLOC throw bad_alloc
#elif !defined(__THROW_BAD_ALLOC)//根据上一个if是否运行 运行则略过
#include <iostream>
# define __THROW_BAD_ALLOC cerr << "out of memory" << endl;exit(1)
#endif

//一级配置器
template<int inst>
class __malloc_alloc_template {
public:
	//定义类似于new_handler的malloc_handler 
	//alias declaration
	using malloc_handler = void(*)();
private:
	//以下函数指针用以处理内存不足的情况
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static malloc_handler __malloc_alloc_oom_handler;	
public:
	static void* allocate(size_t n) {
		void* result = malloc(n);
		if (!result) result = oom_malloc(n);
		return result;
	}
	static void deallocate(void* p, size_t /*n*/) {
		free(p);
	}
	static void* reallocate(void* p, size_t /*old_sz*/,size_t new_sz) {
		void* result = realloc(p, new_sz);
		if (!result) oom_realloc(p, new_sz);
		return result;
	}

	//模拟set_new_handler 原因在于并未使用new与delete配置内存，因此无法使用set_new_handler
	static malloc_handler set_malloc_handler(malloc_handler f){
		malloc_handler old = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

//初始化handler
template<int inst>
typename __malloc_alloc_template<inst>::malloc_handler __malloc_alloc_template<inst>::__malloc_alloc_oom_handler= nullptr;

template<int inst>
void* __malloc_alloc_template<inst>::oom_malloc(size_t){
	malloc_handler my_alloc_handler;
	void* result;
	for (;;) {//不断尝试释放、配置
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (!my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler)();//调用handler，试图释放内存
		result = malloc(n);
		if (result) return result;
	}
}

template<int inst>
inline void* __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n){
	malloc_handler my_alloc_handler;
	void* result;
	for (;;) {
		my_malloc_handler = __malloc_alloc_oom_handler;
		if (!my_malloc_handler) { __THROW_BAD_ALLOC; }
		(*my_malloc_handler());
		result = realloc(p, n);
		if (result) return result;
	}
}

//直接将inst指定为0
typedef __malloc_alloc_template<0> malloc_alloc;

//freelist参数设定
//区块上调边界，区块上限，freelist个数
//此处使用C++11 scoped enum反而由于强类型而导致下文将处处使用static_cast，因此不用
enum  __freelist_setting:std::size_t
{__ALIGN=8, __MAX_BYTES = 128, __NFREELISTS = __MAX_BYTES / __ALIGN};



//第二级配置器
template <bool threads,int inst>
class __default_alloc_template {
private:
	//将bytes上调至8的倍数
	//tips:将x扩大至y的整数倍，取 (x+y-1)&~(y-1)
	static size_t ROUND_UP(size_t bytes) {
		return (((bytes)+__ALIGN - 1)&~(__ALIGN - 1));
	}
private:
	//free_list节点
	//由于union特性，并不需要占用额外的内存
	union obj {
		union obj* free_list_link;
		char client_data[1];
	};
private:
	static obj* volatile free_list[__NFREELISTS];
	//决定使用第几号节点，从1起算
	static size_t FREELIST_INDEX(size_t bytes) {
		return (bytes + __ALIGN - 1) / __ALIGN - 1;
	}
	//传回一个大小为n的对象，并且可能加入大小为n的其它区块到free_list
	static void* refill(size_t n);
	//配置一大块空间，可容纳nobjs个大小为size的区块
	//如果不便配置 nobjs可能会降低
	static char* chunk_alloc(size_t n, int &objs);

	//chunk allocation state
	static char *start_free;//内存池起始位置，只在chunk_alloc()中变化
	static char *end_free;//内存池结束位置，只在chunk_alloc()中变化
	static size_t heap_size;
public:
	static void* allocate(size_t n);
	static void deallocate(void *p, size_t n);
	static void* reallocate(void *p, size_t old_sz, size_t new_sz);
};

//以下为static data member的定义与初值设定
template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::start_free = 0;

template <bool threads, int inst>
char* __default_alloc_template<threads, inst>::end_free = 0;

template <bool threads, int inst>
__default_alloc_template<threads, inst>::obj* volatile
__default_alloc_template<threads, inst>::free_list[__NFREELISTS] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template<bool threads, int inst>
inline void* __default_alloc_template<threads, inst>::allocate(size_t n){
	obj* volatile *my_free_list;
	obj* result;
	//若n大于128,则采用第一级适配器
	if (n >__MAX_BYTES)
		return(malloc_alloc::allocate(n));
	//选择采用第几区块
	my_free_list = free_list + FREELIST_INDEX(n);
	result = *my_free_list;
	if (!result) {
		//未找到可用free_list，准备填充free_list
		void *r = refill(ROUND_UP(n));
		return r;
	}
	//调整freelist
	*my_free_list = result->free_list_link;
	return(result);
}


template<bool threads, int inst>
inline void __default_alloc_template<threads, inst>::deallocate(void * p, size_t n){
	//p不可为nullptr
	obj* q = static_cast<obj*>(p);
	obj* volatile* my_free_list;
	//呼叫第一级适配器
	if (n >__MAX_BYTES) {
		malloc_alloc::deallocate(p, n);
	}
	//寻找对应的free list
	my_free_list = free_list + FREELIST_INDEX(n);
	//调整free list，回收区块
	q->free_list_link = *my_free_list;
	*my_free_list = q;
}
