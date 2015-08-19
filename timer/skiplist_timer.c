/*
 * skiplist是Wiliam Pugh发明的一种随机化的数据结构，基于并联的链表，其插入/删除/查找的复杂度都为O(lgN)，这个特性跟红黑树差不多，但是实现起来简单得多。
 *
 *
 * 代码参考Redis
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist_timer.h"
#include "rte_thread.h"

/*
 * 创建skiplist中的节点, level为该节点的层数
 * */
static inline skiplistNode *slCreateNode(int level, struct rte_timer *tim)
{
	int size;
	skiplistNode *node=NULL;

	size = sizeof(skiplistNode)+level*sizeof(skiplistNode *);
	node=malloc(size);
	if(NULL==node){
		return NULL;
	}
	memset(node, 0, size);
	node->tim = tim;
	return node;
}

/*
 * 创建skiplist
 * */
static skiplist *slCreate(void)
{
	int j;
	skiplist *sl;

	sl = malloc(sizeof(skiplist));
	if(NULL==sl){
		return NULL;
	}
	sl->level = 1;
	sl->length = 0;
	sl->header = slCreateNode(SKIPLIST_MAXLEVEL, 0);
	for(j=0;j<SKIPLIST_MAXLEVEL;j++){
		sl->header->forward[j] = NULL;
	}
	return sl;
}

#if 0
static void slFree(skiplist *sl)
{
	skiplistNode *node = sl->header->forward[0];
	skiplistNode *next;

	free(sl->header);
	while(node){
		next = node->forward[0];
		free(node);
		node = next;
	}
	free(sl);
}
#endif

static int slRandomLevel(void)
{
	int level=1;
	while((rand()&0xFFFF)<(SKIPLIST_P*0xFFFF)){
		level += 1;
	}
	return (level<SKIPLIST_MAXLEVEL)?level:SKIPLIST_MAXLEVEL;
}

static inline int cmp_node(struct rte_timer *orig, struct rte_timer *new)
{
	return orig->expire<new->expire;
}

static skiplistNode *slInsert(skiplist *sl, struct rte_timer *tim)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL]={NULL};
	skiplistNode *node;
	int i, level;

	node = sl->header;
	/* 从最高层次开始搜索，查找插入的位置。update数组用于记录各层次的插入点 */
	for(i=sl->level-1; i>=0; i--){
		while(node->forward[i] &&
				cmp_node(node->forward[i]->tim, tim)){
			node = node->forward[i];
		}
		update[i]=node;
	}

	/* 完全随机决定本次插入的元素所占的层数 */
	level = slRandomLevel();
	if(level>sl->level){
		for(i=sl->level;i<level;i++){
			update[i] = sl->header;
		}
		sl->level = level;
	}
	node = slCreateNode(level, tim);
	/* 分别在每一层执行链表插入动作 */
	for(i=0; i<level; i++){
		node->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = node;
	}

	sl->length++;
	return node;
}

static int slDelete(skiplist *sl, struct rte_timer *tim)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL]={NULL};
	skiplistNode *node;
	int i;

	node = sl->header;
	for(i=sl->level-1; i>=0; i--){
		while(node->forward[i] &&
				cmp_node(node->forward[i]->tim, tim)){
			node = node->forward[i];
		}
		update[i] = node;
	}

	node = node->forward[0];
	if(node && (tim==node->tim)){
		tim->flags &= ~RTE_TIMER_ADDED;
		/* 在各层次执行链表删除动作 */
		for(i=0; i<sl->level; i++){
			if(update[i]->forward[i]==node){
				update[i]->forward[i]=node->forward[i];
			}
		}
		while(sl->level>1 && sl->header->forward[sl->level-1]==NULL){
			sl->level--;
		}
		sl->length--;
		free(node);
	}

	return 0;
}

static skiplist *global_skiplist[RTE_THREAD_NUM];
int skiplist_timer_system_init(void)
{
	int i=0;
	skiplist *sl=NULL;

	for(i=0;i<RTE_THREAD_NUM;i++){
		sl = slCreate();
		if(NULL==sl){
			return -1;
		}
		global_skiplist[i] = sl;
	}

	return 0;
}

int add_skiplist_timer(struct rte_timer *tim, uint32_t expire)
{
	uint32_t thread_idx = rte_get_thread_id();
	skiplist *sl = global_skiplist[thread_idx];

	if(!(tim->flags & RTE_TIMER_INITED)){
		return -1;
	}

	if(tim->flags & RTE_TIMER_ADDED){
		return -1;
	}

	tim->expire = rte_get_cur_time() + expire;
	// tim->expire = expire;
	slInsert(sl, tim);
	tim->flags |= RTE_TIMER_ADDED;

	return 0;
}

int del_skiplist_timer(struct rte_timer *tim)
{
	uint32_t thread_idx = rte_get_thread_id();
	skiplist *sl = global_skiplist[thread_idx];

	if(!(tim->flags & RTE_TIMER_ADDED)){
		return -1;
	}
	slDelete(sl, tim);
	tim->flags &= ~RTE_TIMER_ADDED;

	return 0;
}

int modify_skiplist_timer(struct rte_timer *tim, uint32_t expire)
{
	// TODO
	return 0;
}

void skiplist_timer_manage(void)
{
	uint32_t thread_idx = rte_get_thread_id();
	skiplist *sl = global_skiplist[thread_idx];
	skiplistNode *node=NULL;
	struct rte_timer *tim=NULL;
	uint64_t cur_usec = rte_get_cur_time();

	while((node=sl->header->forward[0])!=NULL){
		tim = node->tim;
		if(cur_usec<tim->expire){
			break;
		}
		slDelete(sl, tim);
		tim->func(tim);
	}
	return;
}


