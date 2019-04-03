#pragma once

#include <cstddef>
#include "rb_tree_node.h"
#include "stl_iterator.h"

namespace MiniSTL {

struct rb_tree_base_iterator {
  using base_ptr = __rb_tree_node_base::base_ptr;
  using iterator_category = bidirectional_iterator_tag;
  using difference_type = ptrdiff_t;

  base_ptr node;

  void increment() {
    if (node->right) {  // 存在右子，则下一节点必为右子树的最左下角
      node = node->right;
      while (node->left) node = node->left;
    } else {  // 当前不存在右子
      base_ptr p = node->parent;
      while (node == p->right) {  // 不断上溯，直至找到第一个不为右子的祖先
        node = p;
        p = p->parent;
      }
      // 正常情况下该祖先之父即为结果
      if (node->right != p) node = p;
      // 若右子等于父节点，node即为自增结果（此为特殊情况，适用于迭代器指向root而后者无right）
    }
  }

  void decrement() {
    if (node->color == rb_tree_red && node->parent->parent == node)
      // 此为特例，当迭代器指向end()将触发此情况
      // root存在一个父节点header，其颜色为红色，左右子均为root
      node = node->left;
    else if (node->left) {  // 存在左子，前一节点必然是左子树的最右下角
      base_ptr l = node->left;
      while (l->right) l = l->right;
      node = l;
    } else {  // 既非root，亦无左子
      base_ptr p = node->parent;
      while (node == p->left) {  // 不断上溯，直至找到第一个不为左子的祖先
        node = p;
        p = p->parent;
      }
      node = p;  // 该祖先的父亲即为答案
    }
  }
};

template <class T, class Ref, class Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator {
  using value_type = T;
  using reference = Ref;
  using pointer = Ptr;
  using iterator = rb_tree_iterator<T, T&, T*>;
  using const_iterator = rb_tree_iterator<T, const T&, const T*>;
  using self = rb_tree_iterator<T, Ref, Ptr>;
  using link_type = __rb_tree_node<T>*;

  rb_tree_iterator() {}
  rb_tree_iterator(link_type x) { node = x; }
  rb_tree_iterator(const iterator& it) { node = it.node; }

  reference operator*() {
    return reinterpret_cast<link_type>(node)->value_field;
  }
  pointer operator->() { return &(operator*()); }

  self& operator++() {
    increment();
    return *this;
  }

  self operator++(int) {
    self temp = *this;
    increment();
    return temp;
  }

  self& operator--() {
    decrement();
    return *this;
  }

  self operator--(int) {
    self temp = *this;
    decrement();
    return temp;
  }
};

inline bool operator==(const rb_tree_base_iterator& lhs,
                       const rb_tree_base_iterator& rhs) {
  return lhs.node == rhs.node;
}

inline bool operator!=(const rb_tree_base_iterator& lhs,
                       const rb_tree_base_iterator& rhs) {
  return lhs.node != rhs.node;
}

}  // namespace MiniSTL