#include "sbi.h"
#include "printf.h"
#include "riscv.h"

extern void trap_init(void);
extern void timer_init(void);

void main()
{
    char a;
    int num = 52010420; 
        trap_init();
        timer_init();
        while (1);
}
