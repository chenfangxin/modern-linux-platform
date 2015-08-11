#ifndef __RTE_MUTEX_H__
#define __RTE_MUTEX_H__

typedef struct {
	volatile int sl;
}rte_mutex_t;

static inline void rte_mutex_init(rte_mutex_t *mt)
{
	return;
}

static inline void rte_mutex_lock(rte_mutex_t *mt)
{
	return;
}

static inline void rte_mutex_unlock(rte_mutex_t *mt)
{
	return;
}

#endif
