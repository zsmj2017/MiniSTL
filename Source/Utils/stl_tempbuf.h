#include "Allocator/subAllocation/uninitialized.h"
#include "Function/stl_function/stl_function.h"
#include "stl_iterator.h"

namespace MiniSTL {

// temporary_buffer
template<class T>
pair<T, ptrdiff_t> __get_temporary_buffer(ptrdiff_t len, T *) {
  if (len > static_cast<ptrdiff_t>(INTPTR_MAX / sizeof(T)))
    len = INTPTR_MAX / sizeof(T);
  while (len > 0) {
    T *tmp =
        reinterpret_cast<T *>(malloc(static_cast<size_t>(len) * sizeof(T)));
    if (tmp) return pair<T *, ptrdiff_t>(tmp, len);
    len /= 2;
  }
  return pair<T *, ptrdiff_t>(reinterpret_cast<T *>(nullptr), 0);
}

template<class T>
inline pair<T, ptrdiff_t> get_temporary_buffer(ptrdiff_t len) {
  return __get_temporary_buffer(len, reinterpret_cast<T *>(nullptr));
}

template<class T>
inline pair<T, ptrdiff_t> get_temporary_buffer(ptrdiff_t len, T *) {
  return __get_temporary_buffer(len, reinterpret_cast<T *>(nullptr));
}

template<class T>
void return_temporary_buffer(T *p) {
  free(p);
}

template<class ForwardIterator, class T>
class __temporary_buffer {
 private:// data member
  ptrdiff_t original_len;
  ptrdiff_t len;
  T *buffer;

 private:// allocate && init
  void allocate_buffer() {
    original_len = len;
    buffer = nullptr;
    if (len > static_cast<ptrdiff_t>(INTPTR_MAX / sizeof(T)))
      len = INTPTR_MAX / sizeof(T);
    while (len > 0) {
      buffer = reinterpret_cast<T *>(
          malloc(static_cast<size_t>(len) * sizeof(T)));
      if (buffer) break;
      len /= 2;
    }
  }

  // use type_traits
  void initialize_buffer(const T &, _true_type) {}
  void initialize_buffer(const T &val, _false_type) {
    uninitialized_fill_n(buffer, len, val);
  }

 public:// getter
  ptrdiff_t size() const noexcept { return len; }
  ptrdiff_t requested_size() const noexcept { return original_len; }

 public:// setter
  T *begin() { return buffer; }
  T *end() { return buffer + len; }

 public:// ctor && dtor
  __temporary_buffer(ForwardIterator first, ForwardIterator last) {
    using trivial = typename _type_traits<T>::is_POD_type;
    try {
      len = distance(first, last);
      allocate_buffer();
      if (len > 0) initialize_buffer(*first, trivial());
    } catch (std::exception &) {
      free(buffer);
      buffer = nullptr;
      len = 0;
    }
  }

  ~__temporary_buffer() {
    destroy(buffer, buffer + len);
    free(buffer);
  }

 public:// disable copy operations
  __temporary_buffer(const __temporary_buffer &) = delete;
  void operator=(const __temporary_buffer &) = delete;
};

template<class ForwardIterator, class T>
struct temporary_buffer : public __temporary_buffer<ForwardIterator, T> {
  temporary_buffer(ForwardIterator first, ForwardIterator last)
      : __temporary_buffer<ForwardIterator, T>(first, last) {}
  ~temporary_buffer() = default;
};

}// namespace MiniSTL