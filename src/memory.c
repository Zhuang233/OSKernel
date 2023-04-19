#include "riscv.h"
#include "types.h"
#include "memory.h"
#include "printf.h"
#include "kalloc.h"

extern char end[];

// 物理内存填充size字节
void memset(void* dest, uint64 size, char val){
    char* dst = (char*)dest;
    for(uint64 i = 0;i < size; i++){
        dst[i] = val;
    }
}

// 物理内存搬移
void memcopy(){

}

