#pragma once

#include "Allocator/subAllocation/allocator.h"
#include "rb_tree_iterator.h"
#include "Algorithms/algobase/stl_algobase.h"
#include "Function/stl_function/stl_function.h"

namespace MiniSTL {

template <class Key, class Value, class KeyOfValue, class Compare,
          class Alloc = simpleAlloc<Value>>
class rb_tree {
private:  // Internal alias declarations
    using base_ptr = __rb_tree_node_base *;
    using rb_tree_node = __rb_tree_node<Value>;
    using rb_tree_node_allocator = simpleAlloc<rb_tree_node>;
    using color_type = rb_tree_color_type;

public:  // Basic type declarations
    using key_type = Key;
    using value_type = Value;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using reference = value_type &;
    using const_reference = const value_type &;
    using link_type = rb_tree_node *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

public:  // Iterator declarations
    using iterator = rb_tree_iterator<value_type, reference, pointer>;
    using const_iterator =
        rb_tree_iterator<value_type, const_reference, const_pointer>;
    using reverse_iterator = __reverse_iterator<iterator>;
    using const_reverse_iterator = __reverse_iterator<const_iterator>;

private:  // operations of node
    link_type get_node() { return rb_tree_node_allocator::allocate(); }
    void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

    link_type create_node(const value_type &value) {
        link_type temp = get_node();
        try {
            construct(&temp->value_field, value);
        } catch (std::exception &) {
            put_node(temp);
        }
        return temp;
    }

    link_type clone_node(link_type p) {
        link_type temp = create_node(p->value_field);
        temp->color = p->color;
        temp->left = nullptr;
        temp->right = nullptr;
        return temp;
    }

    void destroy_node(link_type p) {
        destroy(&p->value_field);
        put_node(p);
    }

private:                   // data member
    size_type node_count;  // 用节点数量表征树的大小
    link_type header;      // root的父亲，实现技巧
    Compare key_compare;

private:  // data member getter && setter
    // header成员
    link_type &root() const noexcept {
        return reinterpret_cast<link_type &>(header->parent);
    }
    link_type &leftmost() const noexcept {
        return reinterpret_cast<link_type &>(header->left);
    }
    link_type &rightmost() const noexcept {
        return reinterpret_cast<link_type &>(header->right);
    }

    // 普通node的快速访问与设定
    static link_type &left(link_type p) {
        return reinterpret_cast<link_type &>(p->left);
    }
    static link_type &right(link_type p) {
        return reinterpret_cast<link_type &>(p->right);
    }
    static link_type &parent(link_type p) {
        return reinterpret_cast<link_type &>(p->parent);
    }
    static reference &value(link_type p) { return p->value_field; }
    static const Key &key(link_type p) { return KeyOfValue()(value(p)); }
    static color_type &color(link_type p) { return p->color; }

    // base_node的快速访问与设定
    static link_type &left(base_ptr p) {
        return reinterpret_cast<link_type &>(p->left);
    }
    static link_type &right(base_ptr p) {
        return reinterpret_cast<link_type &>(p->right);
    }
    static link_type &parent(base_ptr p) {
        return reinterpret_cast<link_type &>(p->parent);
    }
    static reference &value(base_ptr p) {
        return reinterpret_cast<link_type>(p)->value_field;
    }
    static const Key &key(base_ptr p) {
        return KeyOfValue()(value(reinterpret_cast<link_type>(p)));
    }
    static color_type &color(base_ptr p) {
        return reinterpret_cast<link_type>(p)->color;
    }

    // 求取极值（转交node_base)
    static link_type minimum(link_type p) {
        return reinterpret_cast<link_type>(__rb_tree_node_base::minimum(p));
    }
    static link_type maximum(link_type p) {
        return reinterpret_cast<link_type>(__rb_tree_node_base::maximum(p));
    }

private:  // aux interface
    link_type copy(link_type, link_type);
    void empty_initialize() {
        header = get_node();
        color(header) = rb_tree_red;
        root() = nullptr;
        leftmost() = header;
        rightmost() = header;
    }

private:  // rotate && rebalance
    void rb_tree_rotate_left(base_ptr, base_ptr &);
    void rb_tree_rotate_right(base_ptr, base_ptr &);
    void rb_tree_rebalance(base_ptr, base_ptr &);
    base_ptr rb_tree_rebalance_for_erase(base_ptr, base_ptr &, base_ptr &,
                                         base_ptr &);

public:  // ctor && dtor
    rb_tree() : node_count(0), key_compare() { empty_initialize(); }
    explicit rb_tree(const Compare &comp) : node_count(0), key_compare(comp) {
        empty_initialize();
    }
    ~rb_tree() {
        clear();
        put_node(header);
    }

public:  // copy operation
    rb_tree(const rb_tree &rhs) : node_count(0), key_compare(rhs.key_compare) {
        if (!rhs.root())
            empty_initialize();
        else {
            header = get_node();
            color(header) = rb_tree_red;
            root() = copy(rhs.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
        }
        node_count = rhs.node_count;
    }
    rb_tree &operator=(const rb_tree &);

public:  // move operation
    rb_tree(rb_tree &&rhs) noexcept {
        empty_initialize();
        swap(rhs);
    }

    rb_tree &operator=(rb_tree &&rhs) noexcept {
        clear();
        swap(rhs);
        return *this;
    }

public:  // getter
    const_iterator begin() const noexcept { return leftmost(); }
    const_iterator end() const noexcept { return header; }
    const_iterator cbegin() const noexcept { return leftmost(); }
    const_iterator cend() const noexcept { return header; }
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }
    bool empty() const noexcept { return node_count == 0; }
    size_type size() const noexcept { return node_count; }

public:  // setter
    iterator begin() noexcept { return leftmost(); }
    iterator end() noexcept { return header; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(end()); }
    reverse_iterator rend() noexcept { return reverse_iterator(begin()); }

private:  // aux interface for inset
    iterator insert_aux(base_ptr, base_ptr, const value_type &);

public:  // insert
    pair<iterator, bool> insert_unique(const value_type &);
    iterator insert_unique(iterator, const value_type &);
    template <class InputIterator>
    void insert_unique(InputIterator, InputIterator);
    iterator insert_equal(iterator, const value_type &);
    iterator insert_equal(const value_type &);
    template <class InputIterator>
    void insert_equal(InputIterator, InputIterator);

private:  // aux interface for erase
    void erase_aux(link_type) noexcept;

public:  // erase
    void erase(iterator);
    size_type erase(const key_type &);
    void erase(iterator, iterator);
    void clear() noexcept;

public:  // find
    iterator find(const key_type &) noexcept;
    const_iterator find(const key_type &) const noexcept;
    size_type count(const key_type &) const noexcept;
    iterator lower_bound(const key_type &) noexcept;
    const_iterator lower_bound(const key_type &) const noexcept;
    iterator upper_bound(const key_type &) noexcept;
    const_iterator upper_bound(const key_type &) const noexcept;
    pair<iterator, iterator> equal_range(const key_type &) noexcept;
    pair<const_iterator, const_iterator> equal_range(const key_type &) const
        noexcept;

public:  // swap
    void swap(rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs) noexcept {
        // swap data members
        MiniSTL::swap(header, lhs.header);
        MiniSTL::swap(node_count, lhs.node_count);
        MiniSTL::swap(key_compare, lhs.key_compare);
    }
};

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_aux(
    base_ptr x_, base_ptr y_, const value_type &val) {
    link_type x = reinterpret_cast<link_type>(x_);
    link_type y = reinterpret_cast<link_type>(y_);
    link_type z;
    if (y == header || x || key_compare(KeyOfValue()(val), key(y))) {
        // 待插入节点之父为header||待插入节点自身并不为nullptr(何时触发？）||父节点明确大于待插入值
        z = create_node(val);
        left(y) = z;  // 若y为header，此时leftmost==z
        if (y == header) {
            root() = z;
            rightmost() = z;
        } else if (y == leftmost()) {
            leftmost() = z;
        }
    } else {
        // 此时必成为y右子
        z = create_node(val);
        right(y) = z;
        if (y == rightmost()) rightmost() = z;
    }
    parent(z) = y;
    left(z) = nullptr;
    right(z) = nullptr;
    rb_tree_rebalance(z, header->parent);
    ++node_count;
    return iterator(z);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase_aux(
    link_type x) noexcept {
    while (x) {
        // 递归式删除
        erase_aux(right(x));
        link_type y = left(x);
        destroy_node(x);
        x = y;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance(
    base_ptr x, base_ptr &root) {
    x->color = rb_tree_red;
    while (x != root && x->parent->color == rb_tree_red) {  // 当前父节点为红
        if (x->parent == x->parent->parent->left) {  // 父亲为爷爷的左子
            base_ptr y = x->parent->parent->right;   // 伯父
            if (y && y->color == rb_tree_red) {      // 伯父存在且为红色
                x->parent->color = rb_tree_black;
                y->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                x = x->parent->parent;  // 将x更新至祖父,准备上溯
            } else {                    // 不存在伯父或其颜色为黑
                if (x == x->parent->right) {
                    x = x->parent;
                    rb_tree_rotate_left(x, root);  // 左旋
                }
                x->parent->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                rb_tree_rotate_right(x->parent->parent, root);
            }
        } else {  // 父为右子，与上文对称
            base_ptr y = x->parent->parent->left;  // 伯父
            if (y && y->color == rb_tree_red) {
                x->parent->color = rb_tree_black;
                y->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                x = x->parent->parent;  // 将x更新至祖父,准备上溯
            } else {
                if (x == x->parent->left) {
                    x = x->parent;
                    rb_tree_rotate_right(x, root);
                }
                x->parent->color = rb_tree_black;
                x->parent->parent->color = rb_tree_red;
                rb_tree_rotate_left(x->parent->parent, root);
            }
        }
    }
    root->color = rb_tree_black;  // root永远为黑色
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::base_ptr
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance_for_erase(
    base_ptr z, base_ptr &root, base_ptr &leftmost, base_ptr &rightmost) {
    base_ptr y = z;
    base_ptr x = nullptr;
    base_ptr x_parent = nullptr;
    if (!y->left)  // z至多存在一个孩子
        x = y->right;
    else  // z至少存在一个孩子
        if (!y->right)
        x = y->left;
    else {
        y = y->right;
        while (y->left) y = y->left;
        x = y->right;
    }
    if (y != z) {  // 若条件成立，此时y为z的后代
        z->left->parent = y;
        y->left = z->left;
        if (y != z->right) {
            x_parent = y->parent;
            if (x) x->parent = y->parent;
            y->parent->left = x;
            y->right = z->right;
            z->right->parent = y;
        } else
            x_parent = y;
        if (root == z)
            root = y;
        else if (z->parent->left == z)
            z->parent->left = y;
        else
            z->parent->right = y;
        y->parent = z->parent;
        MiniSTL::swap(y->color, z->color);
        y = z;
    } else {
        x_parent = y->parent;
        if (x) x->parent = y->parent;
        if (root == z)
            root = x;
        else if (z->parent->left == z)
            z->parent->left = x;
        else
            z->parent->right = x;
        if (leftmost == z)
            if (!z->right)
                leftmost = z->parent;
            else
                leftmost = __rb_tree_node_base::minimum(x);
        if (rightmost == z)
            if (!z->left)
                rightmost = z->parent;
            else
                rightmost = __rb_tree_node_base::maximum(x);
    }
    if (y->color != rb_tree_red) {
        while (x != root && (!x || x->color == rb_tree_black))
            if (x == x->parent->left) {
                base_ptr w = x_parent->right;
                if (w->color == rb_tree_red) {
                    w->color = rb_tree_black;
                    x_parent->color = rb_tree_red;
                    rb_tree_rotate_left(x_parent, root);
                    w = x_parent->right;
                }
                if ((!w->left || w->left->color == rb_tree_black) &&
                    (!w->right || w->right->color == rb_tree_black)) {
                    w->color = rb_tree_red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if (!w->right || w->right->color == rb_tree_black) {
                        if (w->left) w->left->color = rb_tree_black;
                        w->color = rb_tree_red;
                        rb_tree_rotate_right(w, root);
                        w = x_parent->right;
                    }
                    w->color = x_parent->color;
                    x_parent->color = rb_tree_black;
                    if (x->right) w->right->color = rb_tree_black;
                    rb_tree_rotate_left(x_parent, root);
                    break;
                }
            } else {
                base_ptr w = x_parent->left;
                if (w->color == rb_tree_red) {
                    w->color = rb_tree_black;
                    x_parent->color = rb_tree_red;
                    rb_tree_rotate_right(x_parent, root);
                    w = x_parent->left;
                }
                if ((!w->right || w->right->color == rb_tree_black) &&
                    (!w->left || w->left->color == rb_tree_black)) {
                    w->color = rb_tree_red;
                    x = x_parent;
                    x_parent = x_parent->parent;
                } else {
                    if (!w->left || w->left->color == rb_tree_black) {
                        if (w->right) w->right->color = rb_tree_black;
                        w->color = rb_tree_red;
                        rb_tree_rotate_left(w, root);
                        w = x_parent->left;
                    }
                    w->color = x_parent->color;
                    x_parent->color = rb_tree_black;
                    if (w->left) w->left->color = rb_tree_black;
                    rb_tree_rotate_right(x_parent, root);
                    break;
                }
            }
        if (x) x->color = rb_tree_black;
    }
    return y;
}

// 将x的右子树绕x逆时针旋转，其右子的左子变为了x，而原本的左子变为了x的右子
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare,
                    Alloc>::rb_tree_rotate_left(base_ptr x, base_ptr &root) {
    base_ptr y = x->right;  // 旋转点右子
    x->right = y->left;     // 将x的右子树替换为y的左子树
    if (y->left)            // 若存在，则确立新的父子关系
        y->left->parent = x;
    y->parent = x->parent;

    // 令y顶替x
    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

// 右旋与左旋相对称
template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare,
                    Alloc>::rb_tree_rotate_right(base_ptr x, base_ptr &root) {
    base_ptr y = x->left;
    x->left = y->right;
    if (y->right) y->right->parent = x;
    y->parent = x->parent;
    if (x == root)
        root = y;
    else if (x == x->parent->left)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right = x;
    x->parent = y;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator rb_tree<
    Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) noexcept {
    link_type y = header;  // 最后一个不小于k的node
    link_type x = root();  // 当前node
    while (x)
        if (!key_compare(key(x), k))  // x的键值不小于k
            y = x, x = left(x);
        else
            x = right(x);
    iterator j = iterator(y);
    // 没找到存在两种可能
    // 1.k比最大值还大，j已经指向了end
    // 2.已经寻至叶子，但此时发现k仍然小于key(j.node) 若找到应有k==key(j.node)
    return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const key_type &k) const
    noexcept {
    link_type y = header;  // 最后一个不小于k的node
    link_type x = root();  // 当前node
    while (x)
        if (!key_compare(key(x), k))  // x的键值不小于k
            y = x, x = left(x);
        else
            x = right(x);
    const_iterator j = const_iterator(y);
    // 没找到存在两种可能
    // 1.k比最大值还大，j已经指向了end
    // 2.已经寻至叶子，但此时发现k仍然小于key(j.node) 若找到应有k==key(j.node)
    return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::count(const key_type &k) const
    noexcept {
    pair<const_iterator, const_iterator> p = equal_range(k);
    return MiniSTL::distance(p.first, p.second);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(
    const key_type &k) noexcept {
    link_type y = header;
    link_type x = root();
    while (x)
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::lower_bound(
    const key_type &k) const noexcept {
    link_type y = header;
    link_type x = root();
    while (x)
        if (!key_compare(key(x), k))
            y = x, x = left(x);
        else
            x = right(x);
    return const_iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(
    const key_type &k) noexcept {
    link_type y = header;
    link_type x = root();
    while (x)
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    return iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::upper_bound(
    const key_type &k) const noexcept {
    link_type y = header;
    link_type x = root();
    while (x)
        if (key_compare(k, key(x)))
            y = x, x = left(x);
        else
            x = right(x);
    return const_iterator(y);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,
            typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(
    const key_type &k) noexcept {
    return pair<iterator, iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline pair<
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator,
    typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::const_iterator>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::equal_range(
    const key_type &k) const noexcept {
    return pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k));
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::operator=(const rb_tree &rhs) {
    if (this != &rhs) {
        clear();
        node_count = 0;
        key_compare = rhs.key_compare;
        if (!rhs.root()) {
            root() = nullptr;
            leftmost() = header;
            rightmost() = header;
        } else {
            root() = copy(rhs.root(), header);
            leftmost() = minimum(root());
            rightmost() = maximum(root());
            node_count = rhs.node_count;
        }
    }
    return *this;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator, bool>
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(
    const value_type &val) {
    link_type y = header;
    link_type x = root();
    bool comp = true;
    while (x) {
        y = x;
        comp = key_compare(KeyOfValue()(val), key(x));  // value是否小于x的键值
        x = comp ? left(x) : right(x);
    }
    // 此时y必为待插入点的父节点（也必为叶节点）
    iterator j(y);
    if (comp)              // y键值大于value键值，插于左侧
        if (j == begin())  //待插入点之父为最左节点
            return pair<iterator, bool>(insert_aux(x, y, val), true);
        else
            --j;  // 调整j准备完成测试（可能与某键值重复）
    if (key_compare(key(j.node), KeyOfValue()(val)))
        // 新键值不与旧有键值重复，放心插入
        return pair<iterator, bool>(insert_aux(x, y, val), true);
    return pair<iterator, bool>(j, false);  // 当前value为重复值
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(
    iterator pos, const value_type &val) {
    if (pos.node == header->left) {  // begin()
        if (size() > 0 && key_compare(KeyOfValue()(val), key(pos.node)))
            return insert_aux(pos.node, pos.node, val);
        else
            return insert_unique(val).first;
    } else if (pos.node == header) {  // end()
        if (key_compare(key(rightmost()), KeyOfValue()(val)))
            return insert_aux(nullptr, rightmost(), val);
        else
            return insert_unique(val).first;
    } else {
        iterator before = pos;
        --before;
        if (key_compare(key(before.node), KeyOfValue()(val)) &&
            key_compare(KeyOfValue()(val), key(pos.node))) {
            if (!right(before.node))
                return insert_aux(nullptr, before.node, val);
            else
                return insert_aux(pos.node, pos.node, val);
        } else
            return insert_unique(val).first;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(
    InputIterator first, InputIterator last) {
    for (; first != last; ++first) insert_unique(*first);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(
    const value_type &val) {
    link_type y = header;
    link_type x = root();
    while (x) {
        y = x;
        x = key_compare(KeyOfValue()(val), key(x)) ? left(x)
                                                   : right(x);  // 大则向左
    }
    return insert_aux(x, y, val);  // x为新值插入点，y为其父
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(
    iterator pos, const value_type &val) {
    if (pos.node == header->left) {  // begin()
        if (size() > 0 && !key_compare(key(pos.node), KeyOfValue()(val)))
            return insert_aux(pos.node, pos.node, value);
        else
            return insert_equal(val);
    } else if (pos.node == header) {  // end()
        if (!key_compare(KeyOfValue()(val)), key(rightmost()))
            return insert_aux(nullptr, rightmost(), val);
        else
            return insert_equal(val);
    } else {
        iterator before = pos;
        --before;
        if (!key_compare(KeyOfValue()(val), key(before.node)) &&
            !key_compare(key(pos.node), KeyOfValue()(val))) {
            if (!right(before.node))
                return insert_aux(nullptr, before.node, val);
            else
                return insert_aux(pos.node, pos.node, val);
        } else
            return insert_equal(val);
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
template <class InputIterator>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(
    InputIterator first, InputIterator last) {
    for (; first != last; ++first) insert_equal(*first);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator pos) {
    link_type y = reinterpret_cast<link_type>(rb_tree_rebalance_for_erase(
        pos.node, header->parent, header->left, header->right));
    destroy_node(y);
    --node_count;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::size_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(const key_type &k) {
    pair<iterator, iterator> p = equal_range(k);
    size_type n = MiniSTL::distance(p.first, p.second);
    erase(p.first, p.second);
    return n;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::erase(iterator first,
                                                            iterator last) {
    if (first == begin() && last == end())
        clear();
    else
        while (first != last) erase(first++);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::clear() noexcept {
    if (node_count) {
        erase_aux(root());
        leftmost() = header;
        root() = nullptr;
        rightmost() = header;
        node_count = 0;
    }
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::link_type
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::copy(link_type x,
                                                      link_type y) {
    link_type top = clone_node(x);
    top->parent = y;
    try {
        if (x->right) top->right = copy(right(x), top);
        y = top;
        x = left(x);
        while (x) {
            link_type p = clone_node(x);
            y->left = p;
            p->parent = y;
            if (x->right) p->right = copy(right(x), p);
            y = p;
            x = left(x);
        }
    } catch (std::exception &) {
        erase_aux(top);
    }
    return top;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator==(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
    return lhs.size() == rhs.size() &&
           MiniSTL::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator!=(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
    return !(lhs == rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
    return MiniSTL::lexicographical_compare(lhs.cbegin(), lhs.cend(),
                                            rhs.cbegin(), rhs.cend());
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
    return rhs < lhs;
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator<=(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
    return !(rhs < lhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline bool operator>=(
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    const rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) {
    return !(lhs < rhs);
}

template <class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void swap(
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &lhs,
    rb_tree<Key, Value, KeyOfValue, Compare, Alloc> &rhs) noexcept {
    lhs.swap(rhs);
}

}  // namespace MiniSTL