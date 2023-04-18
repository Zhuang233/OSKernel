#ifndef _KALLOC
#define _KALLOC

extern void freerange(void *pa_start, void *pa_end);
extern void kfree(void *pa);
extern void *kalloc(void);
#endif