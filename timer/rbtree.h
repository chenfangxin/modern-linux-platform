#ifndef __RBTREE_H__
#define __RBTREE_H__

#include "rte_types.h"

#define RB_RED 		0
#define RB_BLACK 	1

struct rb_node {
	uint64_t rb_parent_color; /* 既存储parent地址，也在最低2Bit存储结点颜色 */
	struct rb_node *rb_right;
	struct rb_node *rb_left;
} __attribute__((aligned(sizeof(uint64_t)))); /* 8字节对齐，所以其地址低2Bit为0 */

struct rb_root {
	struct rb_node *rb_node;
};

#define RB_ROOT (struct rb_root){NULL, }
#define rb_entry(ptr, type, member) container_of(ptr, type, member)

static inline void rb_link_node(struct rb_node *node, struct rb_node *parent, struct rb_node **rb_link)
{
	node->rb_parent_color = (uint64_t)parent;
	node->rb_left = node->rb_right = NULL;
	*rb_link = node;
}

struct rb_node *rb_first(const struct rb_root *root);
struct rb_node *rb_last(const struct rb_root *root);
struct rb_node *rb_next(const struct rb_node *node);
struct rb_node *rb_prev(const struct rb_node *node);
void rb_insert_color(struct rb_node *, struct rb_root*);
void rb_erase(struct rb_node *, struct rb_root *);

#endif
