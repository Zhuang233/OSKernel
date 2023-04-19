// Physical memory allocator, for user processes,
// kernel stacks, page-table pages,
// and pipe buffers. Allocates whole 4096-byte pages.

#include "types.h"
#include "riscv.h"
#include "printf.h"
#include "memory.h"
#include "kalloc.h"

#define LEFT_LEAF(index) ((index) * 2 + 1)
#define RIGHT_LEAF(index) ((index) * 2 + 2)
#define PARENT(index) ( ((index) + 1) / 2 - 1)

#define IS_POWER_OF_2(x) (!((x)&((x)-1)))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

struct buddy {
  bdNode size;
  bdNode longest[PMSIZE/PAGE_SIZE*2-1]; // 树节点
}bd;

extern char end[]; // first address after kernel.
                   // defined by kernel.ld.

// 申请内存大小修正为2的次方
static uint64 fixsize(uint64 size) {
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  return size+1;
}

uint64 buddy_alloc(uint64 size) {
  uint64 index = 0;
  uint64 node_size;
  uint64 offset = 0;

  // 最少要申请1个，size将自动调整为装得下的2的次方大小
  if (size <= 0)
    size = 1;
  else if (!IS_POWER_OF_2(size))
    size = fixsize(size);

  // 明显不够,直接不给
  if (bd.longest[index] < size)
    return -1;

  // 往下一直找到合适大小的块 
  for(node_size = bd.size; node_size != size; node_size /= 2 ) {
    // 左孩子有就就往左，否则往右
    if (bd.longest[LEFT_LEAF(index)] >= size)
      index = LEFT_LEAF(index);
    else
      index = RIGHT_LEAF(index);
  }

  // 剩余最长块设为0
  bd.longest[index] = 0;

  // 计算该块的偏移，应为该子树最左叶节点相对于整棵树最左叶节点的偏移
  offset = (index + 1) * node_size - bd.size;

  if(offset != 0){
    // 空间抹0
    memset((void*)KBASE + offset * PAGE_SIZE, size*PAGE_SIZE, 5);
  }


  // 向上逐层更新父节点的剩余最长块大小
  while (index) {
    index = PARENT(index);
    bd.longest[index] = 
      MAX(bd.longest[LEFT_LEAF(index)], bd.longest[RIGHT_LEAF(index)]);
  }

  return KBASE+offset*PAGE_SIZE;
}


void buddy_free(uint64 addr) {
  uint64 offset = (addr-KBASE)/PAGE_SIZE;
  uint64 node_size, index = 0;
  uint64 left_longest, right_longest;
 
  if(offset >= PMTOP/PAGE_SIZE){
      panic("buddy free");
  }
  if(offset == 0){
    panic("can't not free kernel");
  }

  node_size = 1;
  index = offset + bd.size - 1;

  // 向上找到当初申请该块的祖先结点
  for (; bd.longest[index] ; index = PARENT(index)) {
    node_size *= 2;
    if (index == 0)
      return;
  }

  // 剩余最长块设为节点大小
  bd.longest[index] = node_size;

  // 向上逐层更新父节点的剩余最长块大小，能合并的就合并成大块
  while (index) {
    index = PARENT(index);
    node_size *= 2;

    left_longest = bd.longest[LEFT_LEAF(index)];
    right_longest = bd.longest[RIGHT_LEAF(index)];
    
    // 左右孩子都整块还在就合并
    if (left_longest + right_longest == node_size) 
      bd.longest[index] = node_size;
    else
    // 另一块不是整块，只更新节点的剩余最长块大小
      bd.longest[index] = MAX(left_longest, right_longest);
  }
}

// 查询块大小
uint64 buddy_size(uint64 addr) {
  uint64 offset = (addr-KBASE)/PAGE_SIZE;
  uint64 node_size, index = 0;

  if(offset >= PMTOP/PAGE_SIZE){
      panic("buddy size");
  }

  node_size = 1;
  for (index = offset + bd.size - 1; bd.longest[index] ; index = PARENT(index))
    node_size *= 2;

  return node_size*PAGE_SIZE;
}

void buddy_init() {
  uint64 node_size=0;
  uint64 i=0;
  bd.size = PMSIZE/PAGE_SIZE;
  
  node_size = bd.size * 2;

  for (i = 0; i < 2 * bd.size - 1; i++) {
    // 设置每个节点的内存管理大小
    if (IS_POWER_OF_2(i+1))
      node_size /= 2;
    bd.longest[i] = node_size;
  }
  int kernel_size = ((uint64)end - KBASE)/PAGE_SIZE;
  buddy_alloc(kernel_size);
  printf("buddy init finish\n");
}
