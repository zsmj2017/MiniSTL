#pragma once

#include "stl_iterator.h"
#include "stl_list_node.h"
#include <cstddef> // ptrdiff_t

namespace MiniSTL {

    template <class T> struct __list_iterator {
        // alias declarations
        using self = __list_iterator<T>;
        using link_type = __list_node<T> *;

        using iterator_category = bidirectional_iterator_tag;
        using value_type = T;
        using pointer = T *;
        using reference = T &;

        using difference_type = ptrdiff_t;

        // data member
        link_type node; // raw pointer link to list_node

        // ctor
        __list_iterator() {}
        explicit __list_iterator(link_type x) : node(x) {}

        // dtor(trivial)

        bool operator==(const self &rhs) const noexcept {
            return node == rhs.node;
        }
        bool operator!=(const self &rhs) const noexcept {
            return node != rhs.node;
        }

        // dererence
        reference operator*() const {
            return node->data;
        }

        // member access
        pointer operator->() const {
            return &(operator*());
        }

        // increasement
        self &operator++() {
            node = node->next;
            return *this;
        }

        self operator++(int i) {
            self temp = *this;
            ++(*this);
            return temp;
        }

        // decreasement
        self &operator--() {
            node = node->prev;
            return *this;
        }

        self operator--(int i) {
            self temp = *this;
            --(*this);
            return temp;
        }
    };

    template <class T> struct __list_const_iterator {
        // alias declarations
        using iterator = __list_iterator<T>;
        using self = __list_const_iterator<T>;
        using link_type = __list_node<T> *;

        using iterator_category = bidirectional_iterator_tag;
        using value_type = T;
        using pointer = const T *;
        using reference = const T &;
        using difference_type = ptrdiff_t;

        // data member
        link_type node; // raw pointer link to list_node

        // ctor
        __list_const_iterator() {}
        explicit __list_const_iterator(link_type x) : node(x) {}
        __list_const_iterator(const iterator &x) : node(x.node) {}

        // dtor(trivial)

        bool operator==(const self &rhs) const noexcept {
            return node == rhs.node;
        }
        bool operator!=(const self &rhs) const noexcept {
            return node != rhs.node;
        }

        // dererence
        reference operator*() const {
            return node->data;
        }

        // member access
        pointer operator->() const {
            return &(operator*());
        }

        // increasement
        self &operator++() {
            node = node->next;
            return *this;
        }

        self operator++(int i) {
            self temp = *this;
            ++(*this);
            return temp;
        }

        // decreasement
        self &operator--() {
            node = node->prev;
            return *this;
        }

        self operator--(int i) {
            self temp = *this;
            --(*this);
            return temp;
        }
    };

} // namespace MiniSTL