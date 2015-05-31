/*
 * binary_search_tree.h
 *
 *  Created on: May 27, 2015
 *      Author: tx
 */

#ifndef INCLUDE_BINARY_SEARCH_TREE_H_
#define INCLUDE_BINARY_SEARCH_TREE_H_
struct bst_node_base {
	bst_node_base* lchild;
	bst_node_base* rchild;
	bst_node_base* parent;
};

template<typename T>
struct bst_node: bst_node_base {
	T data;
	bst_node* get_lchild()
	{
		return (bst_node*) lchild;
	}
	bst_node* get_rchild()
	{
		return (bst_node*) rchild;
	}
	bst_node* get_parent()
	{
		return (bst_node*) parent;
	}
};

template<typename T>
class binary_search_tree {
	bst_node<T> nil;
	bst_node<T> *root;
	size_t _size = 0;
public:
	binary_search_tree<T>()
	{
		nil.lchild = nullptr;
		nil.rchild = nullptr;
		nil.parent = nullptr;
		root = &nil;
		root->parent = &nil;
	}
	template<typename U>
	void add(U u)
	{
		bst_node<T> *p = &nil;
		bst_node<T> *r = root;
		while (r != &nil) {
			p = r;
			if (r->data < u)
				r = (bst_node<T>*) r->rchild;
			else
				r = (bst_node<T>*) r->lchild;
		}

		bst_node<U> *node = new bst_node<U>;
		node->data = u;
		node->lchild = &nil;
		node->rchild = &nil;
		node->parent = p;

		if (p == &nil)
			root = node;
		else {
			if (p->data < u)
				p->rchild = node;
			else
				p->lchild = node;
		}
		++_size;
	}
	template<typename U>
	bool remove(U data)
	{
		bst_node<T> *r = find(data);
		if (r == &nil)
			return false;

		if (r->lchild == &nil && r->rchild == &nil) {
			if (r->parent->lchild == r)
				r->parent->lchild = &nil;
			else if (r->parent->rchild == r)
				r->parent->rchild = &nil;
			else
				root = &nil;
			delete r;
		} else if (r->lchild != &nil) {
			bst_node<T> * in_order_pre = r->get_lchild();
			while (in_order_pre->rchild != &nil)
				in_order_pre = in_order_pre->get_rchild();
			r->data = in_order_pre->data;
			if (in_order_pre->lchild == &nil && in_order_pre->rchild == &nil)
				remove_without_children(in_order_pre);
			else
				remove_in_order_pre_without_rchild(in_order_pre);
		} else {
			bst_node<T> * in_order_post = r->get_rchild();
			while (in_order_post->lchild != &nil)
				in_order_post = in_order_post->get_lchild();
			r->data = in_order_post->data;
			if (in_order_post->lchild == &nil && in_order_post->rchild == &nil)
				remove_without_children(in_order_post);
			else
				remove_in_order_post_without_lchild(in_order_post);
		}
		--_size;
		return true;
	}
	template<typename U>
	bst_node<U>* find(U data)
	{
		bst_node<T> *r = root;
		while (r != &nil) {
			if (r->data == data)
				return r;
			if (r->data < data)
				r = r->get_rchild();
			else
				r = r->get_lchild();
		}
		return &nil;
	}
	size_t size()
	{
		return _size;
	}
	void in_order_traverse()
	{
		bst_node<T> *pre = &nil;
		bst_node<T> *cur = root;
		bst_node<T> *next = cur->get_lchild();
		while (cur != &nil) {
			if (cur->parent == pre) {
				next = cur->get_lchild();
				if (next == &nil) {
					std::cout << cur->data << " ";
					next = cur->get_rchild();
					if (next == &nil) {
						next = cur->get_parent();
					}
				}

			} else if (cur->lchild == pre) {
				std::cout << cur->data << " ";
				next = cur->get_rchild();
				if (next == &nil) {
					next = cur->get_parent();
				}
			} else {//cur->rchild == pre
				next = cur->get_parent();
			}
			pre = cur;
			cur = next;
		}
	}
	virtual ~binary_search_tree()
	{
		if (root == &nil)
			return;
		bst_node<T>* r = root;
		std::stack<bst_node<T>*> stack;
		stack.push(r);
		r = (bst_node<T>*) r->lchild;
		while (!stack.empty() || r != &nil) {
			if (r != &nil) {
				stack.push(r);
				r = (bst_node<T>*) r->lchild;
			} else {
				r = stack.top();
				stack.pop();
				auto rc = r->rchild;
				delete r;
				r = (bst_node<T>*) rc;
			}
		}
	}
private:
	void remove_without_children(bst_node<T>* node)
	{
		if (node->parent->lchild == node)
			node->parent->lchild = &nil;
		else
			node->parent->rchild = &nil;
		delete node;
	}
	void remove_in_order_pre_without_rchild(bst_node<T>* node)
	{
		if (node->parent->lchild == node)
			node->parent->lchild = node->lchild;
		else
			node->parent->rchild = node->lchild;
		node->lchild->parent = node->parent;
		delete node;
	}
	void remove_in_order_post_without_lchild(bst_node<T>* node)
	{
		if (node->parent->lchild == node)
			node->parent->lchild = node->rchild;
		else
			node->parent->rchild = node->rchild;
		node->rchild->parent = node->parent;
		delete node;
	}
};

#endif /* INCLUDE_BINARY_SEARCH_TREE_H_ */
