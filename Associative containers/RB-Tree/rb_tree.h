#pragma once

#include "rb_tree_iterator.h"
#include <functional>
#include "allocator.h"

namespace MiniSTL{

template <class Key,class Value,class KeyOfValue,class Compare,class Alloc = simpleAlloc<Value> >
class rb_tree {
protected://Internal alias declarations
	using void_pointer = void*;
	using base_ptr = __rb_tree_node_base * ;
	using rb_tree_node = __rb_tree_node<Value>;
	using rb_tree_node_allocator = simpleAlloc<rb_tree_node>;
	using color_type = rb_tree_color_type;

public://Basic types
	using key_type = Key;
	using value_type = Value;
	using pointer = value_type * ;
	using const_pointer = const value_type*;
	using reference = value_type & ;
	using const_reference = const value_type&;
	using link_type = rb_tree_node * ;
	using size_type = size_t;
	using difference_type = ptrdiff_t;

protected://operations of node
	link_type get_node() { return rb_tree_node_allocator::allocate(); }
	void put_node(link_type p) { rb_tree_node_allocator::deallocate(p); }

	link_type create_node(const value_type &value) {
		link_type temp = get_node();
		try{
			construct(&temp->value_field, value);
		}
		catch(std::exception&){
			put_node(temp);
		}
		return temp;
	}

	link_type clone_node(link_type p) {
		link_type temp = create_node(p->value_field);
		temp->color = p->color;
		temp->left = nullptr;
		temp->right = nullptr;
		return temp;
	}

	void destory_node(link_type p) {
		destory(&p->value_field);
		put_node(p);
	}

protected:
	//RB-Tree仅以三组数据表现
	size_type node_count;//用节点数量表征树的大小
	link_type header;//root的父亲，实现技巧
	Compare key_compare;//比较器

	//获取header成员
	link_type& root() const { return reinterpret_cast<link_type&>(header->parent); }
	link_type& leftmost() const { return reinterpret_cast<link_type&>(header->left); }
	link_type& rightmost() const { return reinterpret_cast<link_type&>(header->right); }

	//普通node的快速访问与设定
	static link_type& left(link_type p) { return reinterpret_cast<link_type&>(p->left); }
	static link_type& right(link_type p) { return reinterpret_cast<link_type&>(p->right); }
	static link_type& parent(link_type p) { return reinterpret_cast<link_type&>(p->parent); }
	static reference& value(link_type p) { return p->value_field; }
	static const Key& key(link_type p) { return KeyOfValue()(value(p)); }//KeyofValue是一个函数对象
	static color_type& color(link_type p) { return static_cast<color_type&>(p->color); }

	//base_node的快速访问与设定
	static link_type& left(base_ptr p) { return reinterpret_cast<link_type&>(p->left); }
	static link_type& right(base_ptr p) { return reinterpret_cast<link_type&>(p->right); }
	static link_type& parent(base_ptr p) { return reinterpret_cast<link_type&>(p->parent); }
	static reference& value(base_ptr p) { return static_cast<link_type>(p)->value_field; }
	static const Key& key(base_ptr p) { return KeyOfValue()(value(static_cast<link_type>(p))); }//KeyofValue是一个函数对象
	static color_type& color(base_ptr p) { return static_cast<color_type&>(static_cast<link_type>(p)->color); }

	//求取极值（转交node_base)
	static link_type minimum(link_type p) {
		return static_cast<link_type>(__rb_tree_node_base::minimum(p));
	}

	static link_type maximum(link_type p) {
		return static_cast<link_type>(__rb_tree_node_base::maximum(p));
	}

public:
	using iterator = rb_tree_iterator<value_type, reference, pointer>;

private:
	iterator insert(base_ptr x, base_ptr y, const value_type& value);
	void rb_tree_rebalance(base_ptr x, base_ptr& root);
	void rb_tree_rotate_left(base_ptr x, base_ptr& root);
	void rb_tree_rotate_right(base_ptr x, base_ptr& root);
	iterator rb_tree_find_key (const key_type& k);
	link_type copy(link_type x, link_type y);
	void erase(link_type x);
	void init() {
		header = get_node();
		color(header) = rb_tree_red;
		root() = nullptr;
		leftmost() = header;
		rightmost() = header;
	}
	void clear();

public:
	rb_tree(const Compare& comp = Compare()) 
		:node_count(0), key_compare(comp) { init(); }

	~rb_tree() {
		//clear();
		put_node(header);
	}

	rb_tree& operator=(const rb_tree& rhs);

public:
	Compare key_comp() const { return key_compare; }
	iterator begin() { return leftmost(); }
	iterator end() { return rightmost(); }
	bool empty() { return node_count == 0; }
	size_type size() { return node_count; }
	size_type max_size() { return static_cast<size_type>(-1); }

public:
	//保持node值独一无二
	std::pair<iterator, bool> insert_unique(const value_type& value);
	//允许重复
	iterator insert_equal(const value_type& value);
	iterator find(const value_type& value);
};

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert(base_ptr x_, base_ptr y_, const value_type & value){

	link_type x = x_;
	link_type y = y_;
	link_type z;

	if (y == header || x || KeyOfValue()(value), key(y)) {
		//待插入节点之父为header||待插入节点自身并不为nullptr(何时触发？）||父节点明确大于待插入值
		z = create_node(value);
		left(y) = z;//若y为header，此时leftmost==z
		if (y == header) {
			root() = z;
			rightmost = z;
		}
		else if (y == leftmost()) {
			leftmost() = z;
		}
	}
	else {
		//此时必成为y右子
		z = create_node(value);
		right(y) = z;
		if (y == rightmost()) 
			rightmost() = z;
	}
	parent(z) = y;
	left(z) = nullptr;
	right(z) = nullptr;
	rb_tree_rebalance(z, header->parent);//重新调整红黑树（新增节点颜色在其中调整）
	++node_count;
	return iterator(z);
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rebalance(base_ptr x, base_ptr& root) {
	x->color = rb_tree_red;
	while (x != root && x->parent->color == rb_tree_red) {//当前父节点为红
		if (x->parent == x->parent->parent->left) {//父亲为爷爷的左子
			base_ptr y = x->parent->parent->right;//伯父
			if (y && y->color == rb_tree_red) {//伯父存在且为红色
				x->parent->color = rb_tree_black;
				y->color = rb_tree_black;
				x->parent->parent->color = rb_tree_red;
				x = x->parent->parent;//将x更新至祖父,准备上溯
			}
			else {//不存在伯父或其颜色为黑
				if (x == x->parent->right) {
					x = x->parent;
					rb_tree_rotate_left(x, root);//左旋
				}
				x->parent->color = rb_tree_black;
				x->parent->parent->color = rb_tree_red;
				rb_tree_rotate_right(x->parent->parent, root);
			}
		}
		else {//父为右子，与上文对称
			base_ptr y = x->parent->parent->left;//伯父
			if (y && y->color == rb_tree_red) {
				x->parent->color = rb_tree_black;
				y->color = rb_tree_black;
				x->parent->parent->color = rb_tree_red;
				x = x->parent->parent;//将x更新至祖父,准备上溯
			}
			else {
				if (x == x->parent->left) {
					x = x->parent;
					rb_tree_rotate_right(x, root);
				}
				x->parent->color = rb_tree_black;
				x->parent->parent->color = rb_tree_red;
				rb_tree_rotate_left(x->parent->parent, root);
			}
		}
	}
	root->color = rb_tree_black;//root永远为黑色
}


//将x的右子树绕x逆时针旋转，其右子的左子变为了x，而原本的左子变为了x的右子
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_left(base_ptr x, base_ptr & root){
	base_ptr y = x->right;//旋转点右子
	x->right = y->left;//将x的右子树替换为y的左子树
	if (y->left) //若存在，则确立新的父子关系
		y->left->parent = x;
	y->parent = x->parent;

	//令y顶替x
	if (x == root)
		root = y;
	else if (x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	y->left = x;
	x->parent = y;
}

//右旋与左旋相对称
template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline void rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_rotate_right(base_ptr x, base_ptr & root){
	base_ptr y = x->left;
	x->left = y->right;
	if (y->right)
		x->right->parent = x;
	y->parent = x->parent;
	if (x == root)
		y = root;
	else if (x == x->parent->left)
		y = x->parent->left;
	else
		y = x->parent->right;
	y->right = x;
	x->parent = y;
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::rb_tree_find_key(const key_type & k){
	link_type y = header;//最后一个不小于k的node
	link_type x = root();//当前node

	while (x) {
		if (!key_compare(key(x), k))//x的键值不小于k
			y = x, x = left(x);
		else
			x = right(x);
	}
	iterator j(y);
	//没找到存在两种可能
	//1.k比最大值还大，j已经指向了end
	//2.已经寻至叶子，但此时发现k仍然小于key(j.node) 若找到应有k==key(j.node)
	return (j == end()) || key_compare(k, key(j.node)) ? end() : j;
}


template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline std::pair<typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator,bool> 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_unique(const value_type & value){
	link_type y = header;
	link_type x = root();
	bool comp = true;
	while (x) {
		y = x;
		comp = key_compare(KeyOfValue()(value), key(x));//value是否小于x的键值
		x = comp ? left(x) : right(x);
	}
	//此时y必为待插入点的父节点（也必为叶节点）
	iterator j(y);
	if (comp)//y键值大于value键值，插于左侧
		if (j == begin())//待插入点之父为最左节点
			return std::pair<iterator, bool>(insert(x, y, value), true);
		else
			--j;//调整j准备完成测试（可能与某键值重复）
	if (key_compare(key(j.node), KeyOfValue(value)))
		//新键值不与旧有键值重复，放心插入
		return std::pair<iterator, bool>(insert(x, y, value), true);
	return std::pair<iterator, bool>(j, false);//当前value为重复值
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator 
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::insert_equal(const value_type & value){
	link_type y = header;
	link_type x = root();
	while (x) {
		y = x;
		x = key_compare(KeyOfValue()(value), key(x)) ? left(x) : right(x);//大则向左
	}
	return insert(x, y, value);//x为新值插入点，y为其父
}

template<class Key, class Value, class KeyOfValue, class Compare, class Alloc>
inline typename rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::iterator
rb_tree<Key, Value, KeyOfValue, Compare, Alloc>::find(const value_type & value){
	return rb_tree_find_key(KeyOfValue()(value));
}

} // end namesapce::MiniSTL
