#ifndef _SBI_H
#define _SBI_H
// 参考sbi二进制接口规范编写
// Legacy Extensions
// a7 是 SBI extension ID ,a6不用
// a0 返回值，a1不用

// Other Extension
// a7 是 SBI extension ID（组编号）
// a6 是 SBI function ID（具体编号）
// a0 a1 需要在调用前保存， sbi返回a0(错误码，含义见文档)和a1(值)
// 寄存器的值类型需要为unsigned long 

#define SBI_ECALL(__num, __a0, __a1, __a2)                           \
({                                                                  \
    register unsigned long a0 asm("a0") = (unsigned long)(__a0);    \
    register unsigned long a1 asm("a1") = (unsigned long)(__a1);    \
    register unsigned long a2 asm("a2") = (unsigned long)(__a2);    \
    register unsigned long a7 asm("a7") = (unsigned long)(__num);   \
    asm volatile("ecall"                                            \
                 : "+r"(a0)                                         \
                 : "r"(a1), "r"(a2), "r"(a7)                        \
                 : "memory");                                       \
    a0;                                                             \
})

#define SBI_ECALL_0(__num) SBI_ECALL(__num, 0, 0, 0)
#define SBI_ECALL_1(__num, __a0) SBI_ECALL(__num, __a0, 0, 0)
#define SBI_ECALL_2(__num, __a0, __a1) SBI_ECALL(__num, __a0, __a1, 0)


#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2
#define SBI_CLEAR_IPI 3
#define SBI_SEND_IPI 4
#define SBI_REMOTE_FENCE_I 5
#define SBI_REMOTE_SFENCE_VMA 6
#define SBI_REMOTE_SFENCE_VMA_ASID 7
#define SBI_SHUTDOWN 8


#define SET_TIMER(__a0) SBI_ECALL_1(SBI_SET_TIMER, __a0)
#define PUTCHAR(__a0) SBI_ECALL_1(SBI_CONSOLE_PUTCHAR, __a0)
#define GETCHAR SBI_ECALL_0(SBI_CONSOLE_GETCHAR)
#define SHUTDOWN SBI_ECALL_0(SBI_SHUTDOWN)


#endif