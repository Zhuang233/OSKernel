#include "../src/printf.h"
#include "../src/types.h"
#include "../src/memory.h"
#include "../src/kalloc.h"

void main()
{
        buddy_init();
        kvminit();
        printf("kvminit success!!");

        while(1);  
}