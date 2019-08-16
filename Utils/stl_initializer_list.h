#pragma once

#include <cstddef>

namespace MiniSTL {

    template <class T> class initializer_list {
    public: // alias declarations
        using value_type = T;
        using reference = const T &;
        using const_reference = const T &;
        using size_type = size_t;
        using iterator = const T *;
        using const_iterator = const T *;

    private: // data member
        iterator array;
        size_type len;

    private: // set private ctor so only complier can call it
        constexpr initializer_list(const_iterator a, size_type l) : array(a), len(l) {}

    public: // default ctor
        constexpr initializer_list() noexcept : array(nullptr), len(0) {}

    public: // getter
        constexpr size_type size() const noexcept {
            return len;
        }
        constexpr const_iterator begin() const noexcept {
            return array;
        }
        constexpr const_iterator end() const noexcept {
            return array + size();
        }
    };

    template <class T> constexpr const T *begin(initializer_list<T> ils) noexcept {
        return ils.begin();
    }

    template <class T> constexpr const T *end(initializer_list<T> ils) noexcept {
        return ils.end();
    }

} // namespace MiniSTL