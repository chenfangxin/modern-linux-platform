#ifndef __SKIPLIST_TIMER_H__
#define __SKIPLIST_TIMER_H__

#define SKIPLIST_MAXLEVEL 8
#define SKIPLIST_P 0.5

typedef struct skiplistNode{
	double score;
	struct skiplistNode *backward;
	struct skiplistLevel{
		struct skiplistNode *forward;
	}level[];
}skiplistNode;

typedef struct skiplist{
	struct skiplistNode *header, *tail;
	unsigned long length;
	int level;
}skiplist;

#endif
