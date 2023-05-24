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
void memcopy(void *dest, void *src, uint64 size)
{
    char *d = (char *)dest;
    char *s = (char *)src;
    for (uint64 i = 0; i < size; i++)
    {
        d[i] = s[i];
    }
}

// 模拟mmu,获取页表项
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

// 查找映射后的物理地址
uint64 walkaddr(ptb_t pagetable, uint64 va)
{
  pte_t *pte;
  uint64 pa;

  if(va >= MAXVA)
    return 0;

  pte = walk(pagetable, va, 0);
  if(pte == 0)
    return 0;
  if((*pte & PTE_V) == 0)
    return 0;
  if((*pte & PTE_U) == 0)
    return 0;
  pa = PTE2PA(*pte);
  return pa;
}

// 创建va,pa映射
int mapper(ptb_t pagetable, uint64 va, uint64 pa, int size, uint16 flags)
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
    return 0;
}

// 撤销va,pa映射
void unmap(ptb_t pagetable, uint64 va, uint64 size, int free)
{
    uint64 a;
    pte_t *pte;

    if((va % PAGE_SIZE) != 0)
        panic("unmap: not aligned");

    for(a = va; a < va + size*PAGE_SIZE; a += PAGE_SIZE){
        if((pte = walk(pagetable, a, 0)) == 0)
            panic("unmap: walk");
        if((*pte & PTE_V) == 0)
            panic("unmap: not mapped");
        if(PTE_FLAGS(*pte) == PTE_V)
            panic("unmap: not a leaf");
        if(free){
            uint64 pa = PTE2PA(*pte);
            buddy_free(pa);
        }
    *pte = 0;
    }
}

// 创建用户页表
ptb_t creat_pagetable(){
    // 创建一个空页表
    ptb_t pagetable = (ptb_t)buddy_alloc(1);
    if(pagetable == -1){
        return -1;
    }
    return pagetable;
}

// Given a parent process's page table, copy
// its memory into a child's page table.
// Copies both the page table and the
// physical memory.
// returns 0 on success, -1 on failure.
// frees any allocated pages on failure.
int
uvmcopy(ptb_t old, ptb_t new, uint64 sz)
{
  pte_t *pte;
  uint64 pa, i;
  uint16 flags;
  char *mem;

  for(i = 0; i < sz; i += PAGE_SIZE){
    if((pte = walk(old, i, 0)) == 0)
      panic("uvmcopy: pte should exist");
    if((*pte & PTE_V) == 0)
      panic("uvmcopy: page not present");
    pa = PTE2PA(*pte);
    flags = PTE_FLAGS(*pte);
    if((mem = buddy_alloc(1)) == -1)
      goto err;
    memcopy(mem, (char*)pa, PAGE_SIZE);
    if(mapper(new, i, (uint64)mem, PAGE_SIZE, flags) != 0){
      buddy_free(mem);
      goto err;
    }
  }
  return 0;

 err:
  unmap(new, 0, i / PAGE_SIZE, 1);
  return -1;
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