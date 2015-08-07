/* 
 * 堆排序
 *
 * 代码来自Libevent库
 */
#include "min_heap_timer.h"

static void min_heap_ctor(min_heap_t *s)
{
	s->p = NULL;
	s->n = 0;
	s->a = 0;

	return;
}

static void min_heap_dtor(min_heap_t *s)
{
	if(NULL!=s->p){
		free(s->p);
		min_heap_ctor(s);
	}
	return;
}
