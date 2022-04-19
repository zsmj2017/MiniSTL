#pragma once
#include "Algorithms/algobase/stl_algobase.h"// swap
#include <functional>                        // TODO::need implementation ministl function

// TODO::need add atomic_ref_count

namespace MiniSTL {
template<class T>
class shared_ptr {
  //alias declarations
  using del_t = std::function<void(T *)>;

 public:// ctor && dtor
  shared_ptr(
      T *p = nullptr, del_t del = [](T *p) { delete p; }) : ptr(p), ref_count(new size_t(p != nullptr)), deleter(del) {}
  ~shared_ptr() {
    decrementAndDestory();
  }

 public:// copy operation
  shared_ptr(const shared_ptr &rhs)
      : ptr(rhs.ptr), ref_count(rhs.ref_count), deleter(rhs.deleter) {
    ++*ref_count;
  }
  shared_ptr &operator=(const shared_ptr &rhs);

 public:// move operation
  shared_ptr(shared_ptr &&rhs) noexcept
      : ptr(rhs.ptr), ref_count(rhs.ref_count), deleter(MiniSTL::move(rhs.deleter)) {
    rhs.ptr = nullptr;
    rhs.ref_count = nullptr;
  }
  shared_ptr &operator=(shared_ptr &&rhs) noexcept;

 public:// operator overload
  T &operator*() const noexcept { return *ptr; }
  T *operator->() const noexcept { return &(operator*()); }

 public:// implicit cast for bool
  operator bool() const noexcept { return ptr; }

 public:// getter && setter
  T *get() const noexcept { return ptr; }
  size_t use_count() const noexcept { return *ref_count; }
  bool unique() const noexcept { return *ref_count == 1; }
  void reset(
      T *p = nullptr, const del_t &d = [](T *p) { delete p; });

 private:// aux interface
  void decrementAndDestory();
  void swap(shared_ptr &rhs) noexcept;

 private:// data member
  T *ptr;
  size_t *ref_count;
  del_t deleter;
};

template<class T>
inline void shared_ptr<T>::swap(shared_ptr &rhs) noexcept {
  MiniSTL::swap(ptr, rhs.ptr);
  MiniSTL::swap(ref_count, rhs.ref_count);
  MiniSTL::swap(deleter, rhs.deleter);
}

template<class T>
inline shared_ptr<T> &shared_ptr<T>::operator=(const shared_ptr<T> &rhs) {
  // increment first to ensure safty for self-assignment and avoid identity test
  ++*rhs.ref_count;
  decrementAndDestory();
  ptr = rhs.ptr, ref_count = rhs.ref_count, deleter = rhs.deleter;
  return *this;
}

template<class T>
inline shared_ptr<T> &shared_ptr<T>::operator=(shared_ptr<T> &&rhs) noexcept {
  swap(rhs);
  rhs.decrementAndDestory();
  return *this;
}

template<class T>
inline void shared_ptr<T>::reset(T *p, const del_t &d) {
  if (ptr != p) {
    decrementAndDestory();
    ptr = p;
    ref_count = new std::size_t(p != nullptr);
  }
  deleter = d;
}

template<class T>
inline void shared_ptr<T>::decrementAndDestory() {
  if (ptr && --*ref_count == 0) {
    delete ref_count;
    deleter(ptr);
  } else if (!ptr)
    delete ref_count;
  ptr = nullptr;
  ref_count = nullptr;
}

}// namespace MiniSTL