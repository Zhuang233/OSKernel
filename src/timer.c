#include "sbi.h"
#include "riscv.h"

#define CLINT_FREQ 10000000             //clint频率
#define TIMER_INT_FREQ 1             //定时器中断频率
#define PSC CLINT_FREQ/TIMER_INT_FREQ   //分频系数，也是定时器比较寄存器每次的增量

void timer_init(){
    timer_reset();
}

void timer_reset(){
    unsigned long time_now = r_time();
    sbi_set_timer(time_now + PSC);
}