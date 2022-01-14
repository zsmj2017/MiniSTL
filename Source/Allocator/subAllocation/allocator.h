#pragma once

#include "alloc.h"
#include "construct.h"

namespace MiniSTL {
//默认二级适配器
template <class T, class Alloc = _default_alloc>
class simpleAlloc {
public:
    using value_type = T;
    using pointer = T *;
    using const_pointer = const T *;
    using reference = T &;
    using const_refernce = const T &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public:
    static T *allocate();
    static T *allocate(size_t n);
    static void deallocate(T *ptr);
    static void deallocate(T *ptr, size_t n);

    static void construct(T *ptr);
    static void construct(T *ptr, const T &value);
    static void destroy(T *ptr);
    static void destroy(T *first, T *last);
};

template <class T, class Alloc>
T *simpleAlloc<T, Alloc>::allocate() {
    return reinterpret_cast<T *>(Alloc::allocate(sizeof(T)));
}

template <class T, class Alloc>
T *simpleAlloc<T, Alloc>::allocate(size_t n) {
    if (n == 0) return 0;
    return reinterpret_cast<T *>(Alloc::allocate(sizeof(T) * n));
}

template <class T, class Alloc>
void simpleAlloc<T, Alloc>::deallocate(T *ptr) {
    Alloc::deallocate(reinterpret_cast<void *>(ptr), sizeof(T));
}

template <class T, class Alloc>
void simpleAlloc<T, Alloc>::deallocate(T *ptr, size_t n) {
    if (n == 0) return;
    Alloc::deallocate(reinterpret_cast<void *>(ptr), sizeof(T) * n);
}

template <class T, class Alloc>
void simpleAlloc<T, Alloc>::construct(T *ptr) {
    new (ptr) T();
}

template <class T, class Alloc>
void simpleAlloc<T, Alloc>::construct(T *ptr, const T &value) {
    new (ptr) T(value);
}

template <class T, class Alloc>
void simpleAlloc<T, Alloc>::destroy(T *ptr) {
    ptr->~T();
}

template <class T, class Alloc>
void simpleAlloc<T, Alloc>::destroy(T *first, T *last) {
    for (; first != last; ++first) first->~T();
}
}  // namespace MiniSTL