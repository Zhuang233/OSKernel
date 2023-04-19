#ifndef _MEMORY_
#define _MEMORY_

#define PAGE_SIZE 4096 //每页4k
#define KBASE 0x80000000 //物理内存起始地址
#define PMSIZE 0x8000000//物理内存大小，128M
#define PMTOP (KBASE + PMSIZE) //物理内存顶端
#define PG_ALIGN_UP(a)  (((a)+PAGE_SIZE-1) & ~(PAGE_SIZE-1)) //往高地址处对齐4k地址
#define PG_ALIGN_DOWN(a) (((a)) & ~(PAGE_SIZE-1))  //往低地址处对齐4k地址


extern void kmeminit(void);
extern void memset(void* dest, uint64 size, char val);

#endif