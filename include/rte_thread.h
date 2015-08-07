#ifndef __RTE_THREAD_H__
#define __RTE_THREAD_H__

static inline uint32_t rte_get_thread_id(void)
{
	return 0;
}

static inline uint32_t rte_create_thread(void (*func)(uint64_t), uint64_t data)
{
	return 0;
}
#endif
