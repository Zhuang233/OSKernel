#include "sbi.h"
#include "printf.h"

void main()
{
    char a;
    int num = 52010420; 
        
        a = GETCHAR;
        putchar(a);
        putchar(a);
        putchar('\n');
        printf("%d\t %x\n",num,num);
        panic("panic test");

}

