#include "riscv.h"
#include "types.h"
#include "memory.h"
#include "printf.h"
#include "kalloc.h"

extern char end[];

// 物理内存填充size字节
void memset(void* dest, int size, char val){
    char* dst = (char*)dest;
    for(int i = 0;i < size; i++){
        dst[i] = val;
    }
}

// 物理内存搬移
void memcopy(){

}


void kmeminit(){
    sfence_vma();
    freerange(end+PAGE_SIZE-1, (void*)(PMTOP-1));
    // sfence_vma();
}


