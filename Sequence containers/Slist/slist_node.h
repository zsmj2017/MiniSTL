#pragma once

struct __slist_node_base {//是否是因为这种设计更突出了逻辑性？
	__slist_node_base* next;
};

template<class T>
struct __slist_node:public __slist_node_base {
	T data;
};


//全局函数：已知某一节点，将新节点插于其后
inline __slist_node_base* __slist_make_link(__slist_node_base* prev_node,
	__slist_node_base* new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

//全局函数：获取单链表长度
inline size_t __slist_size(__slist_node_base* base) {
	size_t n = 0;
	while (base) {
		base = base->next;
		++n;
	}
	return n;
}