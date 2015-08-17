/*
 * skiplist是Wiliam Pugh发明的一种随机化的数据结构，基于并联的链表，其插入/删除/查找的复杂度都为O(lgN)，这个特性跟红黑树差不多，但是实现起来简单得多。
 *
 * 代码来自Redis
 * */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "skiplist_timer.h"
#include "rte_thread.h"

skiplistNode *slCreateNode(int level, double score)
{
	int size;
	skiplistNode *node=NULL;
	size = sizeof(struct skiplistNode)+level*sizeof(struct skiplistLevel);
	node=malloc(size);
	if(NULL==node){
		return NULL;
	}
	memset(node, 0, size);
	node->score = score;
	return node;
}

skiplist *slCreate(void)
{
	int j;
	skiplist *sl;

	sl = malloc(sizeof(struct skiplist));
	if(NULL==sl){
		return NULL;
	}
	sl->level = 1;
	sl->length = 0;
	sl->header = slCreateNode(SKIPLIST_MAXLEVEL, 0);
	for(j=0;j<SKIPLIST_MAXLEVEL;j++){
		sl->header->level[j].forward = NULL;
	}
	sl->header->backward = NULL;
	sl->tail = NULL;

	return sl;
}

void slFreeNode(skiplistNode *node)
{
	free(node);
}

void slFree(skiplist *sl)
{
	skiplistNode *node = sl->header->level[0].forward;
	skiplistNode *next;

	free(sl->header);
	while(node){
		next = node->level[0].forward;
		slFreeNode(node);
		node = next;
	}
	free(sl);
}

int slRandomLevel(void)
{
	int level=1;
	while((rand()&0xFFFF)<(SKIPLIST_P*0xFFFF)){
		level += 1;
	}
	return (level<SKIPLIST_MAXLEVEL)?level:SKIPLIST_MAXLEVEL;
}

skiplistNode *slInsert(skiplist *sl, double score)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL];
	skiplistNode *node;
	int i, level;

	node = sl->header;
	for(i=sl->level-1; i>=0; i--){
		while(node->level[i].forward &&
				node->level[i].forward->score<score){
			node = node->level[i].forward;
		}
		update[i]=node;
	}

	level = slRandomLevel();
	if(level>sl->level){
		for(i=sl->level;i<level;i++){
			update[i] = sl->header;
		}
		sl->level = level;
	}
	node = slCreateNode(level, score);
	for(i=0; i<level; i++){
		node->level[i].forward = update[i]->level[i].forward;
		update[i]->level[i].forward = node;
	}

	node->backward = (update[0]==sl->header?NULL:update[0]);
	if(node->level[0].forward){
		node->level[0].forward->backward = node;
	}else{
		sl->tail=node;
	}

	sl->length++;
	return node;
}

void slDeleteNode(skiplist *sl, skiplistNode *node, skiplistNode **update)
{
	int i;
	for(i=0; i<sl->level; i++){
		if(update[i]->level[i].forward==node){
			update[i]->level[i].forward=node->level[i].forward;
		}
	}
	if(node->level[0].forward){
		node->level[0].forward->backward = node->backward;
	}else{
		sl->tail = node->backward;
	}

	while(sl->level>1 && sl->header->level[sl->level-1].forward==NULL){
		sl->level--;
	}
	sl->length--;

	return;
}

int slDelete(skiplist *sl, double score)
{
	skiplistNode *update[SKIPLIST_MAXLEVEL];
	skiplistNode *node;
	int i;

	node = sl->header;
	for(i=sl->level-1; i>=0; i--){
		while(node->level[i].forward && node->level[i].forward->score<score){
			node = node->level[i].forward;
		}
		update[i] = node;
	}

	node = node->level[0].forward;
	if(node && score==node->score){
		slDeleteNode(sl, node, update);
		slFreeNode(node);
		return 1;
	}

	return 0;
}

struct skiplistNode *slSearch(skiplist *sl, double score)
{
	skiplistNode *node;
	int i;

	node = sl->header;
	for(i=sl->level-1; i>=0; i++){
		while(node->level[i].forward && node->level[i].forward->score<score){
			node = node->level[i].forward;
		}
	}
	node = node->level[0].forward;
	if(node && score == node->score){
		printf("Found %d\n", (int)node->score);
		return node;
	}

	printf("Not Found %d\n", (int)score);
	return NULL;
}

