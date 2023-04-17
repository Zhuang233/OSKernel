#include "riscv.h"
#include "types.h"
#include "memory.h"
#include "printf.h"

extern char end[];

// 物理内存填充size字节
void menset(void* dest, int size, char val){
    char* dst = (char*)dest;
    for(int i = 0;i < size; i++){
        dst[i] = val;
    }
}

// 物理内存搬移
void mencopy(){

}


void free_page(uint64 addr, int size){
    uint64 pg_start = PG_ALIGN_DOWN(addr);
    uint64 end = pg_start + PAGE_SIZE*(size-1); //最后一页首地址
    // 超过物理内存限制
    if(end > PMTOP - PAGE_SIZE){
        panic("free page");
    }

    for(;pg_start < end; pg_start += PAGE_SIZE){
        //todo: 对内存数据结构进行操作

        menset((char*)pg_start, PAGE_SIZE, 0);
    }
}


void kmeminit(){
    sfence_vma();
    free_page((uint64)(end+PAGE_SIZE-1), (PMTOP - (uint64)end)/PAGE_SIZE);
    // sfence_vma();
}


