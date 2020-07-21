#pragma once

#include <cstddef>  // ptrdiff_t
#include "allocator.h"
#include "uninitialized.h"

namespace MiniSTL {
// use sub_allocator as default allocator
template <class T, class Alloc = simpleAlloc<T>>
class vector {
public:  // alias declarartions
    using value_type = T;
    using pointer = value_type *;
    using iterator = value_type *;  // iterator is raw pointer
    using const_iterator = const value_type *;
    using reverse_iterator = __reverse_iterator<iterator>;
    using const_reverse_iterator = __reverse_iterator<const_iterator>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

private:  // data member
    // iterator to indicate the vector's memory location
    iterator start;
    iterator finish;
    iterator end_of_storage;

private:  // allocate and construct aux functions
    using data_allocator = Alloc;

    void fill_initialize(size_type n, const value_type &value) {
        start = allocate_and_fill(n, value);
        finish = start + n;
        end_of_storage = finish;
    }
    template <class Integer>
    void initialize_aux(Integer n, Integer val, _true_type) {
        fill_initialize(static_cast<size_type>(n),
                        static_cast<value_type>(val));
    }
    template <class InputIterator>
    void initialize_aux(InputIterator first, InputIterator last, _false_type) {
        start = allocate_and_copy(first, last);
        finish = end_of_storage = start + MiniSTL::distance(first, last);
    }
    iterator allocate_and_fill(size_type n, const value_type &value) {
        iterator result = data_allocator::allocate(n);
        MiniSTL::uninitialized_fill_n(result, n, value);
        return result;
    }
    template <class InputIterator>
    iterator allocate_and_copy(InputIterator first, InputIterator last) {
        size_type n = MiniSTL::distance(first, last);
        iterator result = data_allocator::allocate(n);
        MiniSTL::uninitialized_copy(first, last, result);
        return result;
    }
    void deallocate() noexcept {
        if (start) data_allocator::deallocate(start, end_of_storage - start);
    }
    void destroy_and_deallocate() noexcept {
        destroy(start, finish);  // destroy in "construct.h"
        deallocate();
    }

public:  // swap
    void swap(vector &) noexcept;

public:  // ctor && dtor
    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}
    explicit vector(size_type n) { fill_initialize(n, value_type()); }
    vector(size_type n, const value_type &value) { fill_initialize(n, value); }
    template <class InputIterator>
    vector(InputIterator first, InputIterator last) {
        initialize_aux(first, last, _is_integer_t<InputIterator>());
    }
    vector(std::initializer_list<T>);
    vector(const vector &);
    vector(vector &&) noexcept;

    ~vector() {
        destroy(start, finish);  // destory in "construct.h"
        deallocate();
    }

public:  // copy assignment operator
    vector &operator=(const vector &);

public:  // move assignment
    vector &operator=(vector &&) noexcept;

public:  // getter
    const_iterator begin() const noexcept { return start; }
    const_iterator end() const noexcept { return finish; }
    const_iterator cbegin() const noexcept { return start; }
    const_iterator cend() const noexcept { return finish; }
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(finish);
    }
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(start);
    }
    // TODO:
    // have no empty check
    const_reference front() const noexcept { return *begin(); }
    const_reference back() const noexcept { return *(end() - 1); }
    // TODO:
    // have no boundary check and don't use proxy
    const_reference operator[](const size_type n) const noexcept {
        return *(start + n);
    }
    size_type size() const noexcept {
        return static_cast<size_type>(finish - start);
    }
    size_type capacity() const noexcept {
        return static_cast<size_type>(end_of_storage - start);
    }
    bool empty() const noexcept { return start == finish; }

public:  // setter
    iterator begin() noexcept { return start; }
    iterator end() noexcept { return finish; }
    reverse_iterator rbegin() noexcept { return reverse_iterator(finish); }
    reverse_iterator rend() noexcept { return reverse_iterator(start); }
    reference operator[](const size_type n) noexcept { return *(start + n); }
    reference front() noexcept { return *begin(); }
    reference back() noexcept { return *(end() - 1); }

public:  // interface for size and capacity
    void resize(size_type, const value_type &);
    void resize(size_type new_size) { resize(new_size, value_type()); }
    void reserve(size_type);
    void shrink_to_fit() noexcept {
        vector temp(*this);
        swap(temp);
    }

public:  // compare operator(member function)
    bool operator==(const vector &) const noexcept;
    bool operator!=(const vector &rhs) const noexcept {
        return !(*this == rhs);
    }

public:  // push && pop
    void push_back(const value_type &);
    void pop_back() {
        --finish;
        destroy(finish);
    }

public:  // erase
    iterator erase(iterator, iterator);
    iterator erase(iterator position) { return erase(position, position + 1); }
    void clear() { erase(begin(), end()); }

private:  // aux_interface for insert
    void insert_aux(iterator, const value_type &);
    void fill_insert(iterator, size_type, const value_type &);
    template <class InputIterator>
    void range_insert(iterator pos, InputIterator first, InputIterator last,
                      input_iterator_tag);
    template <class ForwardIterator>
    void range_insert(iterator pos, ForwardIterator first, ForwardIterator last,
                      forward_iterator_tag);
    template <class Integer>
    void insert_dispatch(iterator pos, Integer n, Integer value, _true_type) {
        fill_insert(pos, static_cast<int>(n), value_type(value));
    }
    template <class InputIterator>
    void insert_dispatch(iterator pos, InputIterator first, InputIterator last,
                         _false_type) {
        range_insert(pos, first, last, iterator_category_t<InputIterator>());
    }

public:  // insert
    iterator insert(iterator);
    iterator insert(iterator, const value_type &);
    template <class InputIterator>
    void insert(iterator pos, InputIterator first, InputIterator last) {
        insert_dispatch(pos, first, last, _is_integer_t<InputIterator>());
    }

private:  // aux_interface for assign
    void fill_assign(size_type, const value_type &);
    template <class Integer>
    void assign_dispatch(Integer n, Integer val, _true_type) {
        fill_assign(static_cast<size_type>(n), static_cast<value_type>(val));
    }
    template <class InputIterator>
    void assign_dispatch(InputIterator first, InputIterator last, _false_type) {
        assign_aux(first, last, iterator_category_t<InputIterator>());
    }
    template <class InputIterator>
    void assign_aux(InputIterator first, InputIterator last,
                    input_iterator_tag);
    template <class ForwardIterator>
    void assign_aux(ForwardIterator first, ForwardIterator last,
                    forward_iterator_tag);

public:  // assign
    void assign(size_type n, const value_type &val) { fill_assign(n, val); }
    template <class InputIterator>
    void assign(InputIterator first, InputIterator last) {
        assign_dispatch(first, last, _is_integer_t<InputIterator>());
    }
    void assign(std::initializer_list<value_type> ils) {
        assign(ils.begin(), ils.end());
    }
    vector &operator=(std::initializer_list<value_type> ils) {
        assign(ils);
        return *this;
    }
};

template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator position, const value_type &value) {
    if (finish != end_of_storage) {  // needn't expand
        construct(finish, *(finish - 1));
        ++finish;
        value_type value_copy = value;  // STL copy in copy out
        MiniSTL::copy_backward(position, finish - 2, finish - 1);
        *position = value_copy;
    } else {  // expand
        const size_type old_size = size();
        const size_type new_size =
            old_size ? 2 * old_size : 1;  // new_cap = 2 * old_cap
        iterator new_start = data_allocator::allocate(new_size);
        iterator new_finish = new_start;
        try {
            new_finish = MiniSTL::uninitialized_copy(
                start, position, new_start);  // Copy the first segment
            construct(new_finish, value);
            ++new_finish;
            new_finish = MiniSTL::uninitialized_copy(
                position, finish, new_finish);  // Copy the second segment
        } catch (std::exception &) {
            // commit or rollback
            destroy(new_start, new_finish);
            data_allocator::deallocate(new_start, new_size);
            throw;
        }
        destroy_and_deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + new_size;
    }
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::range_insert(iterator pos, InputIterator first,
                                    InputIterator last, input_iterator_tag) {
    for (; first != last; ++first) {
        pos = insert(pos, *first);
        ++pos;
    }
}

template <class T, class Alloc>
template <class ForwardIterator>
void vector<T, Alloc>::range_insert(iterator position, ForwardIterator first,
                                    ForwardIterator last,
                                    forward_iterator_tag) {
    if (first != last) {
        size_type n = MiniSTL::distance(first, last);
        if (static_cast<size_type>(end_of_storage - finish) >= n) {
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            if (elems_after > n) {
                MiniSTL::uninitialized_copy(finish - n, finish, finish);
                finish += n;
                MiniSTL::copy_backward(position, old_finish - n, old_finish);
                MiniSTL::copy(position, position + n, position);
            } else {
                ForwardIterator mid = first;
                advance(mid, elems_after);
                MiniSTL::uninitialized_copy(mid, last, finish);
                finish += n - elems_after;
                MiniSTL::uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                MiniSTL::copy(first, mid, position);
            }
        } else {  // expand
            const size_type old_size = size();
            const size_type new_size = old_size + MiniSTL::max(old_size, n);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try {
                new_finish =
                    MiniSTL::uninitialized_copy(start, position, new_start);
                new_finish =
                    MiniSTL::uninitialized_copy(first, last, new_finish);
                new_finish =
                    MiniSTL::uninitialized_copy(position, finish, new_finish);
            } catch (std::exception &) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            destroy_and_deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }
}

template <class T, class Alloc>
inline void vector<T, Alloc>::swap(vector &rhs) noexcept {
    MiniSTL::swap(start, rhs.start);
    MiniSTL::swap(finish, rhs.finish);
    MiniSTL::swap(end_of_storage, rhs.end_of_storage);
}

template <class T, class Alloc>
inline vector<T, Alloc>::vector(vector &&rhs) noexcept {
    start = rhs.start;
    finish = rhs.finish;
    end_of_storage = rhs.end_of_storage;
    rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
}

template <class T, class Alloc>
inline vector<T, Alloc> &vector<T, Alloc>::operator=(const vector &rhs) {
    vector temp(rhs);
    swap(temp);
    return *this;
}

template <class T, class Alloc>
inline vector<T, Alloc> &vector<T, Alloc>::operator=(vector &&rhs) noexcept {
    if (this != &rhs) {
        destroy_and_deallocate();
        start = rhs.start;
        finish = rhs.finish;
        end_of_storage = rhs.end_of_storage;
        rhs.start = rhs.finish = rhs.end_of_storage = nullptr;
    }
    return *this;
}

template <class T, class Alloc>
inline void vector<T, Alloc>::resize(size_type new_size,
                                     const value_type &value) {
    if (new_size < size())
        erase(begin() + new_size, end());
    else
        fill_insert(end(), new_size - size(), value);
}

template <class T, class Alloc>
inline void vector<T, Alloc>::reserve(size_type new_capacity) {
    if (new_capacity <= capacity()) return;
    T *new_start = data_allocator::allocate(new_capacity);
    T *new_finish = MiniSTL::uninitialized_copy(start, finish, new_start);
    destroy_and_deallocate();
    start = new_start;
    finish = new_finish;
    end_of_storage = start + new_capacity;
}

template <class T, class Alloc>
bool vector<T, Alloc>::operator==(const vector &rhs) const noexcept {
    if (size() != rhs.size()) {
        return false;
    } else {
        iterator ptr1 = start;
        iterator ptr2 = rhs.start;
        for (; ptr1 != finish && ptr2 != rhs.finish; ++ptr1, ++ptr2)
            if (*ptr1 != *ptr2) return false;
        return true;
    }
}

template <class T, class Alloc>
inline void vector<T, Alloc>::push_back(const value_type &value) {
    if (finish != end_of_storage) {
        construct(finish, value);
        ++finish;
    } else
        insert_aux(end(), value);
}

template <class T, class Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(
    iterator first, iterator last) {
    iterator i = MiniSTL::copy(last, finish, first);
    destroy(i, finish);
    finish -= (last - first);
    return first;
}

template <class T, class Alloc>
void vector<T, Alloc>::fill_insert(iterator position, size_type n,
                                   const value_type &value) {
    if (n) {
        if (static_cast<size_type>(end_of_storage - finish) >=
            n) {  // needn't expand
            value_type value_copy = value;
            const size_type elems_after = finish - position;
            iterator old_finish = finish;
            if (elems_after > n) {
                MiniSTL::uninitialized_copy(finish - n, finish, finish);
                finish += n;
                MiniSTL::copy_backward(position, old_finish - n, old_finish);
                MiniSTL::fill(position, position + n, value_copy);
            } else {
                MiniSTL::uninitialized_fill_n(finish, n - elems_after,
                                              value_copy);
                finish += n - elems_after;
                MiniSTL::uninitialized_copy(position, old_finish, finish);
                finish += elems_after;
                MiniSTL::fill(position, old_finish, value_copy);  // complement
            }
        } else {  // expand
            const size_type old_size = size();
            const size_type new_size = old_size + MiniSTL::max(old_size, n);
            iterator new_start = data_allocator::allocate(new_size);
            iterator new_finish = new_start;
            try {
                new_finish =
                    MiniSTL::uninitialized_copy(start, position, new_start);
                new_finish =
                    MiniSTL::uninitialized_fill_n(new_finish, n, value);
                new_finish =
                    MiniSTL::uninitialized_copy(position, finish, new_finish);
            } catch (std::exception &) {
                destroy(new_start, new_finish);
                data_allocator::deallocate(new_start, new_size);
                throw;
            }
            destroy_and_deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + new_size;
        }
    }
}

template <class T, class Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    iterator position) {
    return insert(position, value_type());
}

template <class T, class Alloc>
inline typename vector<T, Alloc>::iterator vector<T, Alloc>::insert(
    iterator position, const value_type &value) {
    size_type n = position - begin();
    if (finish != end_of_storage && position == end()) {
        construct(finish, value);
        ++finish;
    } else
        insert_aux(position, value);
    return begin() + n;
}

template <class T, class Alloc>
void vector<T, Alloc>::fill_assign(size_type n, const value_type &val) {
    if (n > capacity()) {
        vector<T, Alloc> temp(n, val);
        temp.swap(*this);
    } else if (n > size()) {
        MiniSTL::fill(begin(), end(), val);
        finish = MiniSTL::uninitialized_fill_n(finish, n - size(), val);
    } else
        erase(MiniSTL::fill_n(begin(), n, val), end());
}

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::assign_aux(InputIterator first, InputIterator last,
                                  input_iterator_tag) {
    iterator cur = begin();
    for (; first != last && cur != end(); ++cur, ++first) *cur = *first;
    if (first == last)
        erase(cur, end());
    else
        insert(end(), first, last);
}

template <class T, class Alloc>
template <class ForwardIterator>
void vector<T, Alloc>::assign_aux(ForwardIterator first, ForwardIterator last,
                                  forward_iterator_tag) {
    size_type len = MiniSTL::distance(first, last);
    if (len > capacity()) {
        iterator temp = allocate_and_copy(first, last);
        destroy_and_deallocate();
        start = temp;
        end_of_storage = finish = start + len;
    } else if (size() >= len) {
        iterator new_finish = MiniSTL::copy(first, last, start);
        destroy(new_finish, finish);
        finish = new_finish;
    } else {
        ForwardIterator mid = first;
        MiniSTL::advance(mid, size());
        MiniSTL::copy(first, mid, start);
        finish = MiniSTL::uninitialized_copy(mid, last, finish);
    }
}

template <class T, class Alloc>
inline vector<T, Alloc>::vector(const vector &rhs) {
    start = allocate_and_copy(rhs.begin(), rhs.end());
    finish = end_of_storage = start + rhs.size();
}

template <class T, class Alloc>
inline vector<T, Alloc>::vector(std::initializer_list<T> il) {
    start = allocate_and_copy(il.begin(), il.end());
    finish = end_of_storage = start + il.size();
}

template <class T, class Alloc>
inline bool operator==(const vector<T, Alloc> &lhs,
                       const vector<T, Alloc> &rhs) {
    return lhs.operator==(rhs);
}

template <class T, class Alloc>
inline bool operator!=(const vector<T, Alloc> &lhs,
                       const vector<T, Alloc> &rhs) {
    return !(lhs == rhs);
}

template <class T>
inline bool operator<(const vector<T> &lhs, const vector<T> &rhs) {
    return MiniSTL::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(),
                                            rhs.end());  // in stl_algobase.h
}

template <class T>
inline bool operator>(const vector<T> &lhs, const vector<T> &rhs) {
    return rhs < lhs;
}

template <class T>
inline bool operator<=(const vector<T> &lhs, const vector<T> &rhs) {
    return !(rhs < lhs);
}

template <class T>
inline bool operator>=(const vector<T> &lhs, const vector<T> &rhs) {
    return !(lhs < rhs);
}

template <class T, class Alloc>
inline void swap(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs) {
    lhs.swap(rhs);
}

}  // namespace MiniSTL
