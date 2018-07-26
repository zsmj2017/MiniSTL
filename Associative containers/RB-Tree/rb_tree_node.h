#pragma once

using __rb_tree_color_type = bool;
const __rb_tree_color_type __rb_tree_red = false;
const __rb_tree_color_type __rb_tree_black = true;

struct __rb_tree_node_base {
	using color_type = __rb_tree_color_type;
	using base_ptr = __rb_tree_node_base* ;

	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	//本红黑树最小值
	static base_ptr minium(base_ptr root) {
		while (root->left) 
			root = root->left;
		return root;
	}

	//本红黑树最大值
	static base_ptr maxium(base_ptr root) {
		while (root->right) root = root->right;
		return root;
	}
};

template <class T>
struct __rb_tree_node :public __rb_tree_node_base {
	using link_type = __rb_tree_node<T>*;
	T value_field;
};