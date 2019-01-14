#pragma once

namespace MiniSTL {

struct slist_node_base {
	slist_node_base* next;
};

// insert
inline slist_node_base* slist_make_link(slist_node_base* prev_node,slist_node_base* new_node) {
	new_node->next = prev_node->next;
	prev_node->next = new_node;
	return new_node;
}

// find previous
slist_node_base* slist_previous(slist_node_base* head,const slist_node_base* node){
	while(head && head->next != node)
		head = head->next;
	return head;
}

const slist_node_base* slist_previous(slist_node_base* head,const slist_node_base* node){
	while(head && head->next != node)
		head = head->next;
	return head;
}

inline void slist_splice_after(slist_node_base* pos,slist_node_base* before_first,slist_node_base* before_last){
	if(pos != before_first && pos != before_last){
		slist_node_base* first = before_first->next;
		slist_node_base* after = pos->next;
		before_first->next = before_last->next;
		pos->next = first;
		before_last->next = after;
	}
}

inline void slist_splice_after(slist_node_base* pos,slist_node_base* head){
	slist_node_base* before_last = slist_previous(head,nullptr);
	if(before_last != head){
		slist_node_base* after = pos->next;
		head->next = nullptr;
		pos->next = first;
		before_last->next = after;
	}
}

slist_node_base* slist_reverse(slist_node_base* node){
	slist_node_base* res = node;
	node = node->next;
	res->next = nullptr;
	while(node){
		slist_node_base* next = node->next;
		node->next = res;
		res = node;
		node = next;
	}
	return res;
}

// size
size_t slist_size(slist_node_base* node) {
	size_t res = 0;
	for(;node != nullptr;node = node->next)
		++res;
	return res;
}

template<class T>
struct slist_node :public slist_node_base {
	T data;
};

}// end namespace::MiniSTL