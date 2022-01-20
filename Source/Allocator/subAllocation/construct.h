#pragma once

#include "Iterator/typeTraits.h"
#include <new>// placement new

namespace MiniSTL {

template<class T1, class T2>
inline void construct(T1 *p, T2 value) {
  new (p) T1(value);
}

template<class T>
inline void destroy(T *p) {
  p->~T();
}

// 设法利用traits批量析构对象
template<class ForwardIterator>
inline void destroy(ForwardIterator beg, ForwardIterator end) {
  using is_POD_type = typename _type_traits<ForwardIterator>::is_POD_type;
  _destroy_aux(beg, end, is_POD_type());
}

// 如果元素的value_type不存在non—trivial destructor
template<class ForwardIterator>
inline void _destroy_aux(ForwardIterator beg, ForwardIterator end,
                         _false_type) {
  for (; beg != end; ++beg) destroy(&*beg);// 毕竟迭代器不是真正的地址
}

// 存在trivial destructor
// 如果对象的析构函数无关痛痒，那么反复调用它是一种效率上的巨大浪费
template<class ForwardIterator>
inline void _destroy_aux(ForwardIterator, ForwardIterator, _true_type) {}

// 针对char*、wchar_t*的特化
inline void destroy(char *, char *) {}
inline void destroy(wchar_t *, wchar_t *) {}

}// namespace MiniSTL