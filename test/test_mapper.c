#include "../src/printf.h"
#include "../src/types.h"
#include "../src/memory.h"
#include "../src/kalloc.h"
extern ptb_t kptb;
void main()
{
        buddy_init();
        memset(0x87000000,PAGE_SIZE,3);
        memset(0x87001000,PAGE_SIZE,5);
        kvminit();
        printf("kvminit success!!");
        mapper(kptb,0x87000000,0x87001000,1,PTE_V | PTE_R | PTE_W | PTE_X);
        mapper(kptb,0x87001000,0x87000000,1,PTE_V | PTE_R | PTE_W | PTE_X);

        while(1);  
}