#pragma once

#include <stddef.h>

//hash function 是计算元素位置的函数
//散列函数的本质是取模

template <class Key> struct hash { };

//显然，hash并不能支持所有类型，针对某些自定义类型，应当自主撰写函数对象模板全特化版本

//对const char* 提供字符串转换函数
inline size_t __stl_hash_string(const char* s){
	unsigned long h = 0;
	for (; *s; ++s)
		h = 5 * h + *s;
	return size_t(h);
}

template <class Key>
struct hash<char*>{
	size_t operator()(const char* s) const { return __stl_hash_string(s); }
};

template <class Key> 
struct hash<const char*>{
	size_t operator()(const char* __s) const { return __stl_hash_string(s); }
};

//下面的hash函数都是直接返回原值

template <class Key> 
struct hash<char> {
	size_t operator()(char x) const { return x; }
};

template <class Key> 
struct hash<unsigned char> {
	size_t operator()(unsigned char x) const { return x; }
};

template <class Key> 
struct hash<signed char> {
	size_t operator()(unsigned char x) const { return x; }
};

template <class Key> 
struct hash<short> {
	size_t operator()(short x) const { return x; }
};

template <class Key> 
struct hash<unsigned short> {
	size_t operator()(unsigned short x) const { return x; }
};

template <class Key> 
struct hash<int> {
	size_t operator()(int x) const { return x; }
};
template <class Key> 
struct hash<unsigned int> {
	size_t operator()(unsigned int x) const { return x; }
};

template <class Key> 
struct hash<long> {
	size_t operator()(long x) const { return x; }
};

template <class Key> 
struct hash<unsigned long> {
	size_t operator()(unsigned long x) const { return x; }
};
