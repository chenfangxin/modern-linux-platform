/*
 * skiplist是Wiliam Pugh发明的一种随机化的数据结构，基于并联的链表，其插入/删除/查找的复杂度都为O(lgN)，这个特性跟红黑树差不多，但是实现起来简单得多。
 *
 *
 * 代码参考Redis
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist.h"

/*
 * 创建skiplist中的节点, level为该节点的层数
 * */
static inline skiplistNode *slCreateNode(int level, void *obj)
{
	int size;
	skiplistNode *node=NULL;

	size = sizeof(skiplistNode)+level*sizeof(skiplistNode *);
	node=malloc(size);
	if(NULL==node){
		return NULL;
	}
	memset(node, 0, size);
	node->obj= obj;

	return node;
}

/*
 * 创建skiplist
 * */
skiplist *slCreate(cmp_func_t cmp)
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
	sl->cmp = cmp;
	for(j=0;j<SKIPLIST_MAXLEVEL;j++){
		sl->header->forward[j] = NULL;
	}
	return sl;
}

static int slRandomLevel(void)
{
	int level=1;
	while((rand()&0xFFFF)<(SKIPLIST_P*0xFFFF)){
		level += 1;
	}
	return (level<SKIPLIST_MAXLEVEL)?level:SKIPLIST_MAXLEVEL;
}


skiplistNode *slInsert(skiplist *sl, void *obj)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL]={NULL};
	skiplistNode *node;
	int i, level;

	node = sl->header;
	/* 从最高层次开始搜索，查找插入的位置。update数组用于记录各层次的插入点 */
	for(i=sl->level-1; i>=0; i--){
		while(node->forward[i] &&
				sl->cmp(node->forward[i]->obj, obj)){
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
	node = slCreateNode(level, obj);
	/* 分别在每一层执行链表插入动作 */
	for(i=0; i<level; i++){
		node->forward[i] = update[i]->forward[i];
		update[i]->forward[i] = node;
	}

	sl->length++;
	return node;
}

int slDelete(skiplist *sl, void *obj)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL]={NULL};
	skiplistNode *node;
	int i;

	node = sl->header;
	for(i=sl->level-1; i>=0; i--){
		while(node->forward[i] &&
				sl->cmp(node->forward[i]->obj, obj)){
			node = node->forward[i];
		}
		update[i] = node;
	}

	node = node->forward[0];
	if(node && (obj==node->obj)){
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

