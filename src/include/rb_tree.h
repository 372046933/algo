/*
 * r_b_tree.h
 *
 *  Created on: May 19, 2015
 *      Author: tx
 */

#ifndef INCLUDE_RB_TREE_H_
#define INCLUDE_RB_TREE_H_

#include <stack>
#include "binary_search_tree.h"
enum class color
	:bool {black = false, red = true
};
class rb_tree_node_base: public bst_node_base {
public:
	color clr;
	rb_tree_node_base * get_parent()
	{
		return (rb_tree_node_base*) parent;
	}
	rb_tree_node_base * get_lchild()
	{
		return (rb_tree_node_base*) lchild;
	}
	rb_tree_node_base * get_rchild()
	{
		return (rb_tree_node_base*) rchild;
	}
};
template<typename T>
struct rb_tree_node: public rb_tree_node_base {
	T data;
};

template<typename T>
class rb_tree {
public:
	rb_tree()
	{
		nil.clr = color::black;
		nil.lchild = nullptr;
		nil.rchild = nullptr;
		nil.parent = nullptr;
		root = &nil;
	}
	template<typename U>
	void add(U d)
	{
		auto *parent = &nil;
		auto leaf = root;
		while (leaf != &nil) {
			parent = leaf;
			if (leaf->data < d)
				leaf = (rb_tree_node<T> *) leaf->rchild;
			else
				leaf = (rb_tree_node<T> *) leaf->lchild;
		}
		auto *node = new rb_tree_node<U>;
		node->data = d;
		node->clr = color::red;
		node->lchild = &nil;
		node->rchild = &nil;
		node->parent = parent;
		if (root == &nil)
			root = node;
		else {
			if (parent->data < d)
				parent->rchild = node;
			else
				parent->lchild = node;
		}
		rebalance(node);
	}
	void traverse()
	{
		std::stack<rb_tree_node<T>*> stack;
		rb_tree_node<T>* r = root;
		while (!stack.empty() || r != &nil) {
			if (r != &nil) {
				stack.push(r);
				r = (rb_tree_node<T>*) r->lchild;
			} else {
				//pop out
				r = stack.top();
				std::cout << r->data << " ";
				stack.pop();
				r = (rb_tree_node<T>*) r->rchild;
			}
		}
	}
	template<typename U>
	rb_tree_node<T>* find(const U&d)
	{
		rb_tree_node<T>* r = root;
		while (r) {
			if (r->data == d)
				return r;
			if (r->data < d)
				r = r->rchild;
			else
				r = r->lchild;
		}
		return r;
	}
	~rb_tree()
	{
		if (root == &nil)
			return;
		rb_tree_node<T>* r = root;
		std::stack<rb_tree_node<T>*> stack;
		stack.push(r);
		r = (rb_tree_node<T>*) r->lchild;
		while (!stack.empty() || r != &nil) {
			if (r != &nil) {
				stack.push(r);
				r = (rb_tree_node<T>*) r->lchild;
			} else {
				r = stack.top();
				stack.pop();
				auto rc = r->rchild;
				delete r;
				r = (rb_tree_node<T>*) rc;
			}
		}
	}
private:
	rb_tree_node<T> *root;
	rb_tree_node<T> nil;
	void left_rotate(rb_tree_node_base * x)
	{
		auto y = x->rchild;
		x->rchild = y->lchild;
		if (y->lchild != &nil)
			y->lchild->parent = x;
		y->lchild = x;
		if (x->parent == &nil)
			root = (rb_tree_node<T> *) y;
		else {
			if (x == x->parent->lchild)
				x->parent->lchild = y;
			else
				x->parent->rchild = y;
		}
		y->parent = x->parent;
		x->parent = y;
	}
	void right_rotate(rb_tree_node_base *x)
	{
		auto y = x->lchild;
		x->lchild = y->rchild;
		if (y->rchild != &nil)
			y->rchild->parent = x;
		y->rchild = x;
		if (x->parent == &nil)
			root = (rb_tree_node<T>*) y;
		else {
			if (x == x->parent->lchild)
				x->parent->lchild = y;
			else
				x->parent->rchild = y;
		}
		y->parent = x->parent;
		x->parent = y;
	}
	void rebalance(rb_tree_node_base * node)
	{
		bool right_uncle = false;
		while (node->get_parent()->clr == color::red) {
			rb_tree_node_base *u;
			if (node->parent == node->parent->parent->lchild) {
				u = node->get_parent()->get_parent()->get_rchild();
				right_uncle = true;
			} else
				u = node->get_parent()->get_parent()->get_lchild();
			if (u->clr == color::red) {
				node->get_parent()->clr = color::black;
				u->clr = color::black;
				node->get_parent()->get_parent()->clr = color::red;
				node = node->get_parent()->get_parent();
			} else {
				if (right_uncle) {
					if (node == node->get_parent()->rchild) {
						node = node->get_parent();
						left_rotate(node);
					}
					node->get_parent()->clr = color::black;
					node->get_parent()->get_parent()->clr = color::red;
					right_rotate(node->get_parent()->get_parent());
				} else {
					if (node == node->get_parent()->lchild) {
						node = node->get_parent();
						right_rotate(node);
					}
					node->get_parent()->clr = color::black;
					node->get_parent()->get_parent()->clr = color::red;
					left_rotate(node->get_parent()->get_parent());
				}
			}
		}
		root->clr = color::black;
	}
};

#endif /* INCLUDE_R_B_TREE_H_ */
