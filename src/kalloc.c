// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "riscv.h"
#include "printf.h"
#include "memory.h"
#include "kalloc.h"

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

struct run {
  struct run *next;
};

struct {
  struct run *freelist; //空闲内存链表
} kmem;

//free掉start到end间的物理页
void
freerange(void *pa_start, void *pa_end)
{
  char *p;
  p = (char*)PG_ALIGN_DOWN((uint64)pa_start);
  for(; p <= (char*)pa_end; p += PAGE_SIZE)
    kfree(p);
}

// Free the page of physical memory pointed at by pa,
// which normally should have been returned by a
// call to kalloc().  (The exception is when
// initializing the allocator; see kinit above.)
void
kfree(void *pa)
{
  struct run *r;

  if((uint64)pa < PG_ALIGN_UP((uint64)end) || (uint64)pa >= PMTOP)
    panic("kfree");

  // 填充垃圾数据
  // Fill with junk to catch dangling refs.
  memset(pa, PAGE_SIZE, 1);

  r = (struct run*)PG_ALIGN_DOWN((uint64)pa);
  //头插法将该页加入空闲链表
  r->next = kmem.freelist;
  kmem.freelist = r;
}

// 申请一个内存页（内核使用）
// Allocate one 4096-byte page of physical memory.
// Returns a pointer that the kernel can use.
// Returns 0 if the memory cannot be allocated.
void *kalloc(void)
{
  struct run *r;

  // 从空闲内存链表头部取一块出来使用
  r = kmem.freelist;
  if(r)
    kmem.freelist = r->next;

  if(r)
    memset((char*)r, PAGE_SIZE, 5); // fill with junk 填充垃圾 （安全措施）
  return (void*)r;
}
