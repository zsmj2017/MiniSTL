#pragma once
#include "Algorithms/heap/heap_algorithm.h"
#include "Function/stl_function.h"
#include "SequenceContainers/Vector/stl_vector.h"

namespace MiniSTL {
template<class T, class Sequence = vector<T>,
         class Compare = less<typename Sequence::value_type>>
class priority_queue {
 public:// alias declarations
  using value_type = typename Sequence::value_type;
  using size_type = typename Sequence::size_type;
  using reference = typename Sequence::reference;
  using const_reference = typename Sequence::const_reference;
  // none iterators

 private:// data
  Sequence c;
  Compare comp;

 public:// ctor && dtor
  explicit priority_queue(const Compare &compare) : c(), comp(compare) {}
  explicit priority_queue(const Compare &compare = Compare(),
                          const Sequence &&seq = Sequence())
      : c(std::move(seq)), comp(compare) {
    make_heap(c.begin(), c.end(), comp);
  }
  priority_queue(std::initializer_list<T> il,
                 const Compare &compare = Compare())
      : c(il), comp(compare) {
    make_heap(c.begin(), c.end(), comp);
  }
  template<class InputIterator>
  priority_queue(InputIterator first, InputIterator last,
                 const Compare &compare, const Sequence &seq)
      : c(seq), comp(compare) {
    c.insert(first, last);
    make_heap(c.begin(), c.end(), comp);
  }
  template<class InputIterator>
  priority_queue(InputIterator first, InputIterator last,
                 const Compare &compare = Compare(),
                 const Sequence &&seq = Sequence())
      : c(std::move(seq)), comp(compare) {
    c.insert(c.end(), first, last);
    make_heap(c.begin(), c.end(), comp);
  }

  ~priority_queue() = default;

 public:// copy operation
  priority_queue(const priority_queue &rhs) : c(rhs.c), comp(rhs.comp) {}
  priority_queue &operator=(const priority_queue &rhs) {
    c = rhs.c;
    comp = rhs.comp;
    return *this;
  }

 public:// move operation
  priority_queue(priority_queue &&rhs) noexcept : c(std::move(rhs.c)),
                                                  comp(std::move(rhs.comp)) {}
  priority_queue &operator=(priority_queue &&rhs) noexcept {
    c = std::move(rhs.c);
    comp = std::move(rhs.comp);
    return *this;
  }

 public:// getter
  bool empty() const noexcept { return c.empty(); }
  size_type size() const noexcept { return c.size(); }
  const_reference top() const noexcept { return c.front(); }

 public:// push && pop
  void push(const value_type &value) {
    try {
      c.push_back(value);
      push_heap(c.begin(), c.end(), comp);
    } catch (std::exception &) {
      c.clear();
    }
  }
  void pop() {
    try {
      pop_heap(c.begin(), c.end(), comp);
      c.pop_back();
    } catch (std::exception &) {
      c.clear();
    }
  }

 public:// swap
  void swap(priority_queue &rhs) noexcept {
    MiniSTL::swap(c, rhs.c);
    MiniSTL::swap(comp, rhs.comp);
  }
};

// global swap for priority_
template<class T, class Sequence, class Compare>
inline void swap(priority_queue<T, Sequence, Compare> &lhs,
                 priority_queue<T, Sequence, Compare> &rhs) noexcept {
  lhs.swap(rhs);
}

}// namespace MiniSTL