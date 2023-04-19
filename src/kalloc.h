#ifndef _KALLOC
#define _KALLOC

extern void freerange(void *pa_start, void *pa_end);
extern void kfree(void *pa);
extern void *kalloc(void);
extern void buddy_init(void);
extern uint64 buddy_size(uint64 addr);
extern void buddy_free(uint64 addr);
extern uint64 buddy_alloc(uint64 size);
#endif