#pragma once

namespace MiniSTL{

using rb_tree_color_type = bool;
const rb_tree_color_type rb_tree_red = false;
const rb_tree_color_type rb_tree_black = true;

struct rb_tree_node_base {
	using color_type = rb_tree_color_type;
	using base_ptr = rb_tree_node_base* ;

	color_type color;
	base_ptr parent;
	base_ptr left;
	base_ptr right;

	static base_ptr minium(base_ptr root) {
		while (root->left) 
			root = root->left;
		return root;
	}

	static base_ptr maxium(base_ptr root) {
		while (root->right) 
			root = root->right;
		return root;
	}
};

template <class T>
struct rb_tree_node :public rb_tree_node_base {
	using link_type = rb_tree_node<T>*;
	T value_field;
};

} // end namespace::MiniSTL