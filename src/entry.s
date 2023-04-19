
#  定义代码段 .text.entry
.section .text.entry 

# 定义全局标号_start
.globl _start 

_start:
    la    sp, stack_top     # 初始化栈指针
    call main
halt:   
    j halt          # 无限循环



.section .bss.stack    # 定义堆栈段 .bss.stack
.align 12              # 2的12次方字节对齐，即4096字节对齐
.global stack_top      # 定义全局标号stack_top

# 堆栈段
.space 4096 * 4
stack_top: