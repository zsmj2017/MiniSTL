#pragma once

namespace MiniSTL {

struct slist_node_base {
	slist_node_base* next;
};

template<class T>
struct slist_node :public slist_node_base {
	T data;
};


// insert
inline slist_node_base* slist_make_link(slist_node_base* prev_node,
	slist_node_base* new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

// size
inline size_t slist_size(slist_node_base* base) {
	size_t n = 0;
	while (base) {
		base = base->next;
		++n;
	}
	return n;
}

}// end namespace::MiniSTL