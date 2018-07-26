#pragma once

template <class T>
struct __list_node {
	using void_pointer = void*;
	void_pointer prev;
	void_pointer next;
	T data;
};