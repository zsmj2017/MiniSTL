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
private:
	//以下函数指针用以处理内存不足的情况
	static void* oom_malloc(size_t);
	static void* oom_realloc(void*, size_t);
	static void* (*__malloc_alloc_oom_handler)();
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
	//set_malloc_handler是一个函数指针，其返回一个函数指针 void*（）
	//其形参是一个无返回值的函数指针，其无形参
	static void (*set_malloc_handler(void (*f)()))(){
		void (*old)() = __malloc_alloc_oom_handler;
		__malloc_alloc_oom_handler = f;
		return old;
	}
};

//初始handler
template<int inst>
void (*__malloc_alloc_template<inst>::__malloc_alloc_oom_handler)() = nullptr;

template<int inst>
void * __malloc_alloc_template<inst>::oom_malloc(size_t)
{
	void(*my_malloc_handler)();
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
inline void * __malloc_alloc_template<inst>::oom_realloc(void* p, size_t n){
	void(*my_malloc_handler)();
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



enum{__ALIGN=8};//小型区块的上调边界
enum{__MAX_BYTES=128};//区块上限
enum{__NFREELISTS=__MAX_BYTES/__ALIGN};//链表节点数

//第二级配置器
template <bool threads,int inst>
class __default_alloc_template {
private:
	//将bytes上调至8的倍数
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

};