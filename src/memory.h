#ifndef _MEMORY_
#define _MEMORY_

#define PAGE_SIZE 4096 //每页4k
#define KBASE 0x80000000 //物理内存起始地址
#define KPAGE_NUM ((PG_ALIGN_UP((uint64)end)- KBASE)/PAGE_SIZE)
#define PMSIZE 0x8000000//物理内存大小，128M
#define PMTOP (KBASE + PMSIZE) //物理内存顶端
#define PG_ALIGN_UP(a)  (((a)+PAGE_SIZE-1) & ~(PAGE_SIZE-1)) //往高地址处对齐4k地址
#define PG_ALIGN_DOWN(a) (((a)) & ~(PAGE_SIZE-1))  //往低地址处对齐4k地址

#define PTE_V (1L << 0) // 页表项有效位
#define PTE_R (1L << 1) // 页表项读权限控制位
#define PTE_W (1L << 2) // 页表项写权限控制位
#define PTE_X (1L << 3) // 页表项执行权限控制位
#define PTE_U (1L << 4) // 页表项用户权限控制位

// 物理地址转换为页表项值（低10位存放页表项各标志位）
#define PA2PTE(pa) ((((uint64)pa) >> 12) << 10)

// 页表项值转换为物理地址（页首地址）
#define PTE2PA(pte) (((pte) >> 10) << 12)

// 提取页表项各权限位
#define PTE_FLAGS(pte) ((pte) & 0x3FF)

// 页表分成三层的树（27位 每层9位）
#define PXMASK          0x1FF // 每层9位
#define PGSHIFT 12
#define PXSHIFT(level)  (PGSHIFT+(9*(level)))
// 由虚拟地址获取某层的页表项下标
#define PX(level, va) ((((uint64) (va)) >> PXSHIFT(level)) & PXMASK)

// 最大虚拟地址
// 一个超出最高虚拟地址的地址
// MAXVA实际上比Sv39允许的最大值小一位，以避免必须对具有高位设置的虚拟地址进行符号扩展。
#define MAXVA (1L << (9 + 9 + 9 + 12 - 1))

extern int mapper(ptb_t pagetable, uint64 va, uint64 pa, int size, uint16 flags);
extern void kvminit(void);
extern void memset(void* dest, uint64 size, char val);

#endif