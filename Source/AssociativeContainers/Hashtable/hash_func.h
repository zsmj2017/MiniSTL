#pragma once

#include <cstddef>
#include <string>

namespace MiniSTL {

// hash function是计算元素位置的函数,其本质为取模
template<class Key>
struct hash {};

// 显然，hash并不能支持所有类型，针对某些自定义类型，应当自主撰写函数对象模板全特化版本

// 对const char* 提供字符串转换函数
inline size_t _stl_hash_string(const char *s) {
  unsigned long h = 0;
  for (; *s; ++s) h = 5 * h + *s;
  return static_cast<size_t>(h);
}

template<>
struct hash<char *> {
  size_t operator()(const char *s) const noexcept {
    return _stl_hash_string(s);
  }
};

template<>
struct hash<const char *> {
  size_t operator()(const char *s) const noexcept {
    return _stl_hash_string(s);
  }
};

// add hash() for std::string
template<>
struct hash<std::string> {
  size_t operator()(const std::string &str) const noexcept {
    return _stl_hash_string(str.c_str());
  }
};

template<>
struct hash<char> {
  size_t operator()(char x) const noexcept { return x; }
};

template<>
struct hash<unsigned char> {
  size_t operator()(unsigned char x) const noexcept { return x; }
};

template<>
struct hash<signed char> {
  size_t operator()(unsigned char x) const noexcept { return x; }
};

template<>
struct hash<short> {
  size_t operator()(short x) const noexcept { return x; }
};

template<>
struct hash<unsigned short> {
  size_t operator()(unsigned short x) const noexcept { return x; }
};

template<>
struct hash<int> {
  size_t operator()(int x) const noexcept { return x; }
};

template<>
struct hash<unsigned int> {
  size_t operator()(unsigned int x) const noexcept { return x; }
};

template<>
struct hash<long> {
  size_t operator()(long x) const noexcept { return x; }
};

template<>
struct hash<unsigned long> {
  size_t operator()(unsigned long x) const noexcept { return x; }
};

}// namespace MiniSTL
