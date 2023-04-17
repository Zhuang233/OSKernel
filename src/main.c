#include "sbi.h"
#include "printf.h"
#include "riscv.h"
#include "memory.h"

extern void trap_init(void);
extern void timer_init(void);

void main()
{
    char a;
    int num = 52010420; 
        trap_init();
        // timer_init();
        kmeminit();
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

