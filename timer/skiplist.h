#ifndef __SKIPLIST_H__
#define __SKIPLIST_H__

#define SKIPLIST_MAXLEVEL 8 /* 最大层级 */
#define SKIPLIST_P 0.5 /* 下级元素出现在上一级的概率 */

typedef struct skiplistNode{
	void *obj;
	struct skiplistNode *forward[0]; // Level
}skiplistNode;

typedef int (*cmp_func_t)(void *, void *);
typedef struct skiplist{
	struct skiplistNode *header;
	unsigned long length;
	int level;
	cmp_func_t cmp;
}skiplist;

skiplist *slCreate(cmp_func_t cmp);
skiplistNode *slInsert(skiplist *sl, void *obj);
int slDelete(skiplist *sl, void *obj);
#endif
