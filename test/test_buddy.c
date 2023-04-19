#include "../src/printf.h"
#include "../src/types.h"
#include "../src/memory.h"
#include "../src/kalloc.h"

extern void trap_init(void);
extern void timer_init(void);

void main()
{
        buddy_init();
        printf("kernel buddy size:%d\n",buddy_size(KBASE));
        uint64 m1 = buddy_alloc(4);
        uint64 m2 = buddy_alloc(9);
        uint64 m3 = buddy_alloc(3);
        uint64 m4 = buddy_alloc(7);

        printf("alloc 4 :%p\tsize: 0x%x\n", m1, buddy_size(m1));
        printf("alloc 9 :%p\tsize: 0x%x\n", m2, buddy_size(m2));
        printf("alloc 3 :%p\tsize: 0x%x\n", m3, buddy_size(m3));
        printf("alloc 7 :%p\tsize: 0x%x\n", m4, buddy_size(m4));

        buddy_free(m1);
        buddy_free(m2);
        buddy_free(m3);
        buddy_free(m4);

        uint64 m5 = buddy_alloc(32);
        printf("alloc 32 :%p\tsize: 0x%x\n", m5, buddy_size(m5));
        buddy_free(m5);

        uint64 m6 = buddy_alloc(0);
        printf("alloc 0 :%p\tsize: 0x%x\n", m6, buddy_size(m6));
        buddy_free(m6);

        while(1);  
}

