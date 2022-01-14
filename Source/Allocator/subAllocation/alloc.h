#pragma once
#include <cstring>  // memcpy
#include <cstdlib>   // malloc and free
#include <new>       // bad_alloc

namespace MiniSTL {

//一级配置器
class _malloc_alloc {
public:
    // alias declaration
    using malloc_handler = void (*)();

private:
    //以下函数指针用以处理内存不足的情况
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
    static malloc_handler _malloc_alloc_oom_handler;

public:
    static void *allocate(size_t n) {
        void *result = malloc(n);
        if (result == nullptr) result = oom_malloc(n);
        return result;
    }

    static void deallocate(void *p, size_t /*n*/) { free(p); }

    static void *reallocate(void *p, size_t /*old_sz*/, size_t new_sz) {
        void *result = realloc(p, new_sz);
        if (result == nullptr) oom_realloc(p, new_sz);
        return result;
    }

    //模拟set_new_handler
    //原因在于并未使用new与delete配置内存，因此无法使用set_new_handler
    static malloc_handler set_malloc_handler(malloc_handler f) {
        malloc_handler old = _malloc_alloc_oom_handler;
        _malloc_alloc_oom_handler = f;
        return old;
    }
};



inline void *_malloc_alloc::oom_malloc(size_t n) {
    malloc_handler new_alloc_handler;
    void *result;
    for (;;) {  //不断尝试释放、配置
        new_alloc_handler = _malloc_alloc_oom_handler;
        if (!new_alloc_handler) throw std::bad_alloc();
        (*new_alloc_handler)();  //调用handler，试图释放内存
        result = malloc(n);
        if (result) return result;
    }
}

inline void *_malloc_alloc::oom_realloc(void *p, size_t n) {
    malloc_handler new_alloc_handler;
    void *result;
    for (;;) {
        new_alloc_handler = _malloc_alloc_oom_handler;
        if (!new_alloc_handler) throw std::bad_alloc();
        (*new_alloc_handler)();
        result = realloc(p, n);
        if (result) return result;
    }
}

using malloc_alloc = _malloc_alloc;

// freelist参数设定
//区块上调边界，区块上限，freelist个数
// Effective C++所述enum惯用法
enum _freelist_setting {
    ALIGN = 8,
    MAX_BYTES = 128,
    NFREELISTS = MAX_BYTES / ALIGN
};

// 第二级配置器
class _default_alloc {
private:
    // 将bytes上调至8的倍数
    static size_t ROUND_UP(size_t bytes) {
        return (((bytes) + static_cast<size_t>(ALIGN) - 1) &
                ~(static_cast<size_t>(ALIGN) - 1));
    }

private:
    // free_list节点
    // 由于union特性，并不需要占用额外的内存
    union obj {
        union obj *free_list_link;  //指向下一节点
        char client_data[1];        //指向资源
    };

private:
    static obj *volatile free_list[NFREELISTS];
    // 决定使用第几号节点，从1起算
    static size_t FREELIST_INDEX(size_t bytes) {
        return (bytes + static_cast<size_t>(ALIGN) - 1) /
                   static_cast<size_t>(ALIGN) -
               1;
    }
    // 传回一个大小为n的对象，并且可能加入大小为n的其它区块到free_list
    static void *refill(size_t n);
    // 配置一大块空间，可容纳nobjs个大小为size的区块
    // 如果不便配置 nobjs可能会降低
    static char *chunk_alloc(size_t size, int &nobjs);

    // chunk allocation state
    static char *start_free;  //内存池起始位置，只在chunk_alloc()中变化
    static char *end_free;  //内存池结束位置，只在chunk_alloc()中变化
    static size_t heap_size;

public:
    static void *allocate(size_t n);
    static void deallocate(void *p, size_t n);
    static void *reallocate(void *p, size_t old_sz, size_t new_sz);
};


// 当free_list无可用区块时，重新填充空间
// 新空间取自内存池，默认获取20个节点(区块）
// 若内存池不足，则获取的将小于20
inline void *_default_alloc::refill(size_t n) {
    int nobjs = 20;
    // 尝试调用chunk_alloc,注意nobjs以pass-by-reference传入
    char *chunk = chunk_alloc(n, nobjs);
    obj *volatile *my_free_list;
    obj *result;
    obj *current_obj, *next_obj;

    // 若只获取了一个区块则直接分配给调用者，不加入free_list
    if (1 == nobjs) return (chunk);
    my_free_list = free_list + FREELIST_INDEX(n);

    // 在chunk空间内建立free_list
    result = reinterpret_cast<obj *>(chunk);
    // 引导free_list指向内存池分配的空间
    // chunk指向的内存直接分给用户，free_list指向剩下（19或更少）的区块
    *my_free_list = next_obj = reinterpret_cast<obj *>(chunk + n);
    for (int i = 1;; ++i) {
        current_obj = next_obj;
        next_obj =
            reinterpret_cast<obj *>(reinterpret_cast<char *>(next_obj) + n);
        if (nobjs - 1 == i) {
            current_obj->free_list_link = nullptr;
            break;
        } else {
            current_obj->free_list_link = next_obj;
        }
    }
    return result;
}

// 默认size为8的整数倍
inline char *_default_alloc::chunk_alloc(size_t size, int &nobjs) {
    char *result;
    size_t total_bytes = size * nobjs;
    size_t bytes_left = end_free - start_free;  //内存池剩余空间
    if (bytes_left >= total_bytes) {
        // 容量满足需求
        result = start_free;
        start_free += total_bytes;
        return result;
    } else if (bytes_left > size) {
        // 容量至少满足一个区块需求
        nobjs = static_cast<int>(bytes_left / size);
        total_bytes = size * nobjs;
        result = start_free;
        start_free += total_bytes;
        return result;
    } else {
        // 内存池一个区块都无法提供
        size_t bytes_to_get =
            2 * total_bytes +
            ROUND_UP(
                heap_size >>
                4);  // 向heap申请注入的内存，heap_size将随着配置次数增加而增加
        if (bytes_left > 0) {
            // 当前内存池还有一部分内存，为了不浪费分配给free_list
            obj *volatile *my_free_list =
                free_list + FREELIST_INDEX(bytes_left);
            reinterpret_cast<obj *>(start_free)->free_list_link = *my_free_list;
            *my_free_list = reinterpret_cast<obj *>(start_free);
        }
        // 配置heap空间以补充内存池
        start_free = reinterpret_cast<char *>(malloc(bytes_to_get));
        if (!start_free) {
            // heap空间不足，malloc失败
            obj *volatile *my_free_list;
            obj *p;
            // 在free_list中检查是否有符合需求的区块
            for (size_t i = size; i <= static_cast<size_t>(MAX_BYTES);
                 i += static_cast<size_t>(ALIGN)) {
                my_free_list = free_list + FREELIST_INDEX(i);
                p = *my_free_list;
                if (p) {
                    // 存在足以分配的区块
                    *my_free_list = p->free_list_link;  // 抽离当前区块
                    start_free = reinterpret_cast<char *>(p);
                    end_free = start_free + i;
                    return (chunk_alloc(
                        size,
                        nobjs));  // 递归调用以修正nobjs，此时必然进入else_if分支
                }
            }
            end_free = nullptr;  // 到处都找不到内存
            // 调用第一级适配器观察其能否分配内存，或抛出异常
            start_free =
                reinterpret_cast<char *>(malloc_alloc::allocate(bytes_to_get));
        }
        heap_size += bytes_to_get;  // 已占用的堆内存
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);  // 调用自身以修正nobjs
    }
}

inline void *_default_alloc::allocate(size_t n) {
    obj *volatile *my_free_list;
    obj *result;
    // 若n大于128,则采用第一级适配器
    if (n > MAX_BYTES) return (malloc_alloc::allocate(n));
    // 选择采用第几区块
    my_free_list = free_list + FREELIST_INDEX(n);
    result = *my_free_list;
    if (result == nullptr) {
        // 未找到可用free_list，准备填充free_list
        void *r = refill(ROUND_UP(n));
        return r;
    }
    // 调整freelist
    *my_free_list = result->free_list_link;
    return result;
}

inline void _default_alloc::deallocate(void *p, size_t n) {
    // p不可为nullptr
    if (n > static_cast<size_t>(MAX_BYTES))
        malloc_alloc::deallocate(p, n);
    else {
        // 寻找对应的free list
        obj *volatile *my_free_list = free_list + FREELIST_INDEX(n);
        obj *q = reinterpret_cast<obj *>(p);
        // 调整free list，回收区块
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }
}

inline void *_default_alloc::reallocate(void *p, size_t old_sz, size_t new_sz) {
    void *result;
    size_t copy_sz;

    if (old_sz > static_cast<size_t>(MAX_BYTES) &&
        new_sz > static_cast<size_t>(MAX_BYTES))
        return realloc(p, new_sz);
    if (ROUND_UP(old_sz) == ROUND_UP(new_sz)) return p;
    result = allocate(new_sz);
    copy_sz = new_sz > old_sz ? old_sz : new_sz;
    memcpy(result, p, copy_sz);
    deallocate(p, old_sz);
    return result;
}

}  // namespace MiniSTL
