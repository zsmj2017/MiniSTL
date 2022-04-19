#pragma once

namespace MiniSTL {

class Deleter {
 public:
  template<class T>
  void operator()(T *ptr) const { delete ptr; }
};

template<class T, class D = Deleter>
class unique_ptr {
 public:// ctor && dtor
  unique_ptr(T *p = nullptr, const D &d = D()) : ptr(p), deleter(d) {}
  ~unique_ptr() { deleter(ptr); }

 public:// uncopyable
  unique_ptr(const unique_ptr &) = delete;
  unique_ptr &operator=(const unique_ptr &) = delete;

 public:// move-only
  unique_ptr(unique_ptr &&rhs) noexcept : ptr(rhs.ptr), deleter(std::move(rhs.deleter)) { rhs.ptr = nullptr; }
  unique_ptr &operator=(unique_ptr &&rhs) noexcept;

 public:// operator= for nullptr
  unique_ptr &operator=(std::nullptr_t) noexcept;

 public:// implicit cast interface to bool
  operator bool() const { return ptr; }

 public:// operator overload:* ->
  T &operator*() const { return *ptr; }
  T *operator->() const { return &(operator*()); }

 public:// getter and setter
  T *get() const noexcept { return ptr; }
  D &get_deleter() noexcept { return deleter; }
  const D &get_deleter() const noexcept { return deleter; }
  void reset(T *p = nullptr) noexcept;
  T *release() noexcept;

 public:// swap for operator=
  void swap(unique_ptr &) noexcept;

 private:// data member
  T *ptr;
  D deleter;
};

template<class T, class D>
inline void unique_ptr<T, D>::swap(unique_ptr<T, D> &rhs) noexcept {
  swap(ptr, rhs.ptr);
  swap(deleter, rhs.deleter);
}

template<class T, class D>
inline unique_ptr<T, D> &unique_ptr<T, D>::operator=(unique_ptr<T, D> &&rhs) noexcept {
  if (ptr != rhs.ptr) {
    reset();
    swap(*this, rhs);
  }
  return *this;
}

template<class T, class D>
inline unique_ptr<T, D> &unique_ptr<T, D>::operator=(std::nullptr_t) noexcept {
  reset();
  return *this;
}

template<class T, class D>
inline void unique_ptr<T, D>::reset(T *p) noexcept {
  if (ptr != p) {
    deleter(ptr);
    ptr = p;
  }
}

template<class T, class D>
inline T *unique_ptr<T, D>::release() noexcept {
  T *res = ptr;
  ptr = nullptr;
  return res;
}

}// namespace MiniSTL