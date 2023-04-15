#include "sbi.h"
#include "printf.h"

extern void trap_init(void);

void main()
{
    char a;
    int num = 52010420; 
        trap_init();
        a = GETCHAR;
        test();
        putchar(a);
        putchar(a);
        putchar('\n');
        printf("%d\t %x\n",num,num);
        panic("panic test");
        
}

