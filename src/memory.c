#include "riscv.h"
#include "types.h"
#include "memory.h"
#include "printf.h"
#include "kalloc.h"

extern char end[];
ptb_t kptb;

// 物理内存填充size字节
void memset(void *dest, uint64 size, char val)
{
    char *dst = (char *)dest;
    for (uint64 i = 0; i < size; i++)
    {
        dst[i] = val;
    }
}

// 物理内存搬移
void memcopy()
{
}

pte_t *walk(ptb_t pagetable, uint64 va, int alloc)
{
    uint8 level = 2;
    pte_t *p_pte;
    while (level > 0)
    {
        p_pte = &pagetable[PX(level, va)]; // 取出表项

        if (!(*p_pte & PTE_V))
        {
            if (!alloc)
                return 0;
            // 新建页表
            pagetable = (ptb_t)buddy_alloc(1);
            // 回填页表项
            *p_pte = PA2PTE(pagetable) | PTE_V;
        }
        pagetable = (ptb_t)PTE2PA(*p_pte);
        level--;
    }
    // 此时已来到最后一层页表
    return &pagetable[PX(0, va)]; // 取出表项
}

void mapper(ptb_t pagetable, uint64 va, uint64 pa, int size, uint16 flags)
{
    flags = PTE_FLAGS(flags);
    pte_t *p_pte = walk(pagetable, va, 1);
    for (int i = 0; i < size; i++)
    {
        *p_pte = PA2PTE(pa + i * PAGE_SIZE) | flags;
        p_pte++;
        // 到达页表边界
        if (((uint64)p_pte & 0xfff) == 0)
        {
            p_pte = walk(pagetable, va + (i+1)*PAGE_SIZE,1); 
        }
    }
}

void kvminit()
{
    // 建立内核代码区直接映射
    kptb = (ptb_t)buddy_alloc(1);
    mapper(kptb, KBASE, KBASE, KPAGE_NUM, PTE_V | PTE_R | PTE_W | PTE_X);
    // 其他内存区直接映射
    mapper(kptb, (uint64)end, (uint64)end, PMSIZE/PAGE_SIZE - KPAGE_NUM, PTE_V | PTE_R | PTE_W | PTE_X);

    sfence_vma();
    //写satp寄存器，让cpu知道内核页表的首地址在哪
    w_satp(MAKE_SATP(kptb));
    sfence_vma();
}