#include "sbi.h"
#include "printf.h"
#include "riscv.h"
#include "memory.h"
#include "kalloc.h"

extern void trap_init(void);
extern void timer_init(void);

void main()
{
    char a;
    int num = 52010420; 
        trap_init();
        // unsigned long i;
        // for(i=65500;;i++){
        //     printf("%d\n",i);
        // }
        buddy_init();
        printf("kernel buddy size:%d\n",buddy_size(0x80000000));
        printf("kernel buddy size:%d\n",buddy_size(0x80000000));
        buddy_alloc(5);
        
        // timer_init();
        // kmeminit();
        // void*r[10];
        // for(int i=0;i<10;i++){
        //     r[i]=kalloc();
        // }
        // a = r_sstatus();
        // printf("sstatus = %p",a);
        // putchar('a');
        // GETCHAR;
        
        // SHUTDOWN;
        // GETCHAR;
        // 
        // putchar('a');
        while(1);


        
        
}

