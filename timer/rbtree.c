#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define rb_parent(r) 	((struct rb_node *)((r)->rb_parent_color & ~3))
#define rb_color(r) 	((r)->rb_parent_color & 1)
#define rb_is_red(r) 	(!rb_color(r))
#define rb_is_black(r) 	(rb_color(r))
#define rb_set_red(r)	do{(r)->rb_parent_color &= ~1;}while(0)
#define rb_set_black(r) do{(r)->rb_parent_color |= 1;}while(0)

/* 设置节点的parent节点， 但不改变节点颜色 */
static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
{
	rb->rb_parent_color = (rb->rb_parent_color & 3) | (uint64_t)p;
}

/* 设置节点颜色 */
static inline void rb_set_color(struct rb_node *rb, int color)
{
	rb->rb_parent_color = (rb->rb_parent_color & ~1)|color;
}

/*
 * 左旋
 *   以node及其右子节点Y之间的连接为轴，逆时针旋转。使得node的右子节点Y成为子树新的根节点，而node成为Y的左子节点，Y原来的左子节点成为node的右子节点。
 * */
static void rb_rotate_left(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *right = node->rb_right;
	struct rb_node *parent = rb_parent(node);
	
	if(NULL!=(node->rb_right=right->rb_left)){
		rb_set_parent(right->rb_left, node);
	}
	right->rb_left = node;

	rb_set_parent(right, parent);
	if(parent){
		if(node==parent->rb_left){
			parent->rb_left = right;
		}else{
			parent->rb_right = right;
		}
	}else{
		root->rb_node = right;
	}
	rb_set_parent(node, right);
	return;
}

/*
 * 右旋
 *   以node及其左子节点Y之间的连接为轴，顺时针旋转。使得Y成为子树的新根节点，node成为Y的右子节点，Y原来的右子节点成为node的左子节点。
 * */
static void rb_rotate_right(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *left = node->rb_left;
	struct rb_node *parent = rb_parent(node);

	if(NULL!=(node->rb_left=left->rb_right)){
		rb_set_parent(left->rb_right, node);
	}
	left->rb_right = node;
	rb_set_parent(left, parent);
	if(parent){
		if(node==parent->rb_right){
			parent->rb_right = left;
		}else{
			parent->rb_left = left;
		}
	}else{
		root->rb_node = left;
	}
	rb_set_parent(node, left);

	return;
}

#define rb_swap_node(na, nb) 	\
		do{						\
			register void *tmp;	\
			tmp = na;			\
			na = nb;			\
			nb = tmp;			\
		}while(0);

/*
 * 插入一个节点
 * 三种情况下需要对红黑树做调整：
 * 1）当前节点的父节点是红色，且叔节点是红色时
 * 2）当前节点的父节点是红色，叔节点是黑色，当前节点为右子节点时
 * 3）当前节点的父节点是红色，叔节点是黑色，当前节点为左子节点时
 * */
void rb_insert_color(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;
	/* 当前节点的父节点是红色 */
	while((parent=rb_parent(node)) && rb_is_red(parent)){
		gparent = rb_parent(parent);
		if(parent==gparent->rb_left){
			{
				/* 叔节点(祖父节点的另一个子节点)是红色 */
				register struct rb_node *uncle = gparent->rb_right;
				if(uncle && rb_is_red(uncle)){
					rb_set_black(uncle); // 叔节点涂黑
					rb_set_black(parent); // 父节点涂黑
					rb_set_red(gparent); // 祖父节点涂红
					node = gparent; // 当前节点指向祖父节点，并从新的位置重新执行算法
					continue;
				}
			}

			/* 叔节点是黑色，且当前节点为父节点的右子节点 */
			if(parent->rb_right==node){
				rb_rotate_left(parent, root); //
				rb_swap_node(parent, node)
			}
			rb_set_black(parent);
			rb_set_red(gparent);
			rb_rotate_right(gparent, root);
		}else{
			{
				/* 叔节点(祖父节点的另一个子节点)是红色 */
				register struct rb_node *uncle = gparent->rb_left;
				if(uncle && rb_is_red(uncle)){
					rb_set_black(uncle);
					rb_set_black(parent);
					rb_set_red(gparent);
					node = gparent;
					continue;
				}
			}

			/* 叔节点是黑色，且当前节点为父节点的左子节点 */
			if(parent->rb_left==node){
				rb_rotate_right(parent, root);
				rb_swap_node(parent, node);
			}
			rb_set_black(parent);
			rb_set_red(gparent);
			rb_rotate_left(gparent, root);
		}
	}
	rb_set_black(root->rb_node);
}

/*
 * 删除一个节点时，如何选择顶替的节点？
 * 1） 没有子节点
 * 2） 有一个子节点
 * 3） 有两个子节点 -- 通常选取左子树中的最大元素
 *
 * 如果删除节点是红色，则原来红黑树的性质不会改变，不需要调整；
 * 如果删除节点是黑色，则需要顶替被删除节点的那个节点开始调整，并认为其有另一重黑色。
 * 删除时，出现如下几种情况：
 * 1） 当前节点是红+黑 -- 直接把当前节点涂黑
 * 2） 当前节点是黑+黑，且为根节点 -- 不需要做调整
 * 3） 当前节点是黑+黑，兄弟节点是红色
 * 4） 当前节点是黑+黑，兄弟节点是黑色且兄弟节点的两个子节点也为黑色
 * 5） 当前节点是黑+黑，兄弟节点是黑色，兄弟节点的右子节点为黑色，左子节点为红色
 * 6） 当前节点是黑+黑，兄弟节点是黑色，兄弟节点的右子节点为红色，左子节点任意
 * */
static void rb_erase_color(struct rb_node *node, struct rb_node *parent, struct rb_root *root)
{
	struct rb_node *other;
	while((!node||rb_is_black(node)) && node!=root->rb_node){
		if(parent->rb_left==node){
			other = parent->rb_right;
			if(rb_is_red(other)){
				rb_set_black(other);
				rb_set_red(parent);
				rb_rotate_left(parent, root);
				other = parent->rb_right;
			}
			if((!other->rb_left || rb_is_black(other->rb_left)) &&
				(!other->rb_right||rb_is_black(other->rb_right))){
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}else{
				if(!other->rb_right || rb_is_black(other->rb_right)){
					rb_set_black(other->rb_left);
					rb_set_red(other);
					rb_rotate_right(other, root);
					other = parent->rb_right;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_right);
				rb_rotate_left(parent, root);
				node = root->rb_node;
				break;
			}
		}else{
			other = parent->rb_left;
			if(rb_is_red(other)){
				rb_set_black(other);
				rb_set_red(parent);
				rb_rotate_right(parent, root);
				other = parent->rb_left;
			}
			if((!other->rb_left || rb_is_black(other->rb_left)) &&
				(!other->rb_right || rb_is_black(other->rb_right))){
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			}else{
				if(!other->rb_left || rb_is_black(other->rb_left)){
					rb_set_black(other->rb_right);
					rb_set_red(other);
					rb_rotate_left(other, root);
					other = parent->rb_left;
				}
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->rb_left);
				rb_rotate_right(parent, root);
				node = root->rb_node;
				break;
			}
		}
	}
	if(node){
		rb_set_black(node);
	}

	return;
}

/*
 * 删除一个节点
 * */
void rb_erase(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;	
	int color;
	if(!node->rb_left){
		child = node->rb_right;
	}else if(!node->rb_right){
		child = node->rb_left;
	}else{
		struct rb_node *old = node, *left;
		node = node->rb_right;
		while((left=node->rb_left)!=NULL){
			node = left;
		}
		if(rb_parent(old)){
			if(rb_parent(old)->rb_left==old){
				rb_parent(old)->rb_left = node;
			}else{
				rb_parent(old)->rb_right = node;
			}
		}else{
			root->rb_node = node;
		}
		child = node->rb_right;
		parent = rb_parent(node);
		color = rb_color(node);

		if(parent==old){
			parent = node;
		}else{
			if(child){
				rb_set_parent(child, parent);
			}
			parent->rb_left = child;
			node->rb_right = old->rb_right;
			rb_set_parent(old->rb_right, node);
		}
		node->rb_parent_color = old->rb_parent_color;
		node->rb_left = old->rb_left;
		rb_set_parent(old->rb_left, node);
		goto color;
	}
	parent = rb_parent(node);
	color = rb_color(node);
	if(child){
		rb_set_parent(child, parent);
	}
	if(parent){
		if(parent->rb_left==node){
			parent->rb_left = child;
		}else{
			parent->rb_right = child;
		}
	}else{
		root->rb_node = child;
	}
color:
	if(color==RB_BLACK){
		rb_erase_color(child, parent, root);
	}

	return;
}

struct rb_node *rb_first(const struct rb_root *root)
{
	struct rb_node *node=root->rb_node;
	if(NULL==node){
		return NULL;
	}
	while(node->rb_left){
		node = node->rb_left;
	}
	return node;
}

struct rb_node *rb_last(const struct rb_root *root)
{
	struct rb_node *node=root->rb_node;
	if(NULL==node){
		return NULL;
	}
	while(node->rb_right){
		node = node->rb_right;
	}
	return node;
}

struct rb_node *rb_next(const struct rb_node *node)
{
	struct rb_node *parent;
	if(rb_parent(node)==node){
		return NULL;
	}

	if(node->rb_right){
		node = node->rb_right;
		while(node->rb_right){
			node = node->rb_right;
		}
		return (struct rb_node *)node;
	}
	while((parent=rb_parent(node)) && node==parent->rb_right){
		node = parent;
	}
	return parent;
}

struct rb_node *rb_prev(const struct rb_node *node)
{
	struct rb_node *parent;
	if(rb_parent(node)==node){
		return NULL;
	}

	if(node->rb_left){
		node = node->rb_left;
		while(node->rb_right){
			node = node->rb_right;
		}
		return (struct rb_node *)node;
	}
	while((parent=rb_parent(node)) && node==parent->rb_left){
		node = parent;
	}
	return parent;
}
