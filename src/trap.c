#include "riscv.h"
#include "ptregs.h"
#include "printf.h"

#define SCAUSE_EC (0xf) /* 支持16个异常*/
#define SOFT_INT 1
#define TIMER_INT 5
#define EX_INT 9

extern void do_exception_vector(void);
extern void timer_reset(void);

struct fault_info {
	int (*fn)(struct pt_regs *regs, const char *name);
	const char *name;
};

void show_regs(struct pt_regs *regs)
{
	printf("sepc: %p ra : %p sp : %p\n",
		regs->sepc, regs->ra, regs->sp);
	printf(" gp : %p tp : %p t0 : %p\n",
		regs->gp, regs->tp, regs->t0);
	printf(" t1 : %p t2 : %p t3 : %p\n",
		regs->t1, regs->t2, regs->s0);
	printf(" s1 : %p a0 : %p a1 : %p\n",
		regs->s1, regs->a0, regs->a1);
	printf(" a2 : %p a3 : %p a4 : %p\n",
		regs->a2, regs->a3, regs->a4);
	printf(" a5 : %p a6 : %p a7 : %p\n",
		regs->a5, regs->a6, regs->a7);
	printf(" s2 : %p s3 : %p s4 : %p\n",
		regs->s2, regs->s3, regs->s4);
	printf(" s5 : %p s6 : %p s7 : %p\n",
		regs->s5, regs->s6, regs->s7);
	printf(" s8 : %p s9 : %p s10: %p\n",
		regs->s8, regs->s9, regs->s10);
	printf(" s11: %p t3 : %p t4: %p\n",
		regs->s11, regs->t3, regs->t4);
	printf(" t5 : %p t6 : %p\n",
		regs->t5, regs->t6);
}

static void do_trap_error(struct pt_regs *regs, const char *str)
{
	printf("Oops - %s\n", str);
	show_regs(regs);
	printf("sstatus:%p  sbadaddr:%p  scause:%p\n",
			regs->sstatus, regs->sbadaddr, regs->scause);
	panic("trap error");
}

#define DO_ERROR_INFO(name)				\
int name(struct pt_regs *regs, const char *str)				\
{									\
	do_trap_error(regs, str);	\
	return 0;            \
}

DO_ERROR_INFO(do_trap_unknown);
DO_ERROR_INFO(do_trap_insn_misaligned);
DO_ERROR_INFO(do_trap_insn_fault);
DO_ERROR_INFO(do_trap_insn_illegal);
DO_ERROR_INFO(do_trap_load_misaligned);
DO_ERROR_INFO(do_trap_load_fault);
DO_ERROR_INFO(do_trap_store_misaligned);
DO_ERROR_INFO(do_trap_store_fault);
DO_ERROR_INFO(do_trap_ecall_u);
DO_ERROR_INFO(do_trap_ecall_s);
DO_ERROR_INFO(do_trap_break);
DO_ERROR_INFO(do_page_fault);

static const struct fault_info fault_info[] = {
	{do_trap_insn_misaligned, "Instruction address misaligned"},//指令地址没对齐
	{do_trap_insn_fault, "Instruction access fault"},//指令访问异常
	{do_trap_insn_illegal, "Illegal instruction"},//无效指令
	{do_trap_break, "Breakpoint"},//断点
	{do_trap_load_misaligned, "Load address misaligned"},//加载地址没对齐*
	{do_trap_load_fault, "Load access fault"},//加载访问异常
	{do_trap_store_misaligned, "Store/AMO address misaligned"},//存储/AMO地址没对齐
	{do_trap_store_fault, "Store/AMO access fault"},//存储/AMO访问异常
	{do_trap_ecall_u, "Environment call from U-mode"},//用户模式的系统调用异常
	{do_trap_ecall_s, "Environment call from S-mode"},//特权模式下的系统调用异常*
	{do_trap_unknown, "unknown 10"},//
	{do_trap_unknown, "unknown 11"},//
	{do_page_fault, "Instruction page fault"},//指令缺页异常
	{do_page_fault, "Load page fault"},//加载缺页异常
	{do_trap_unknown, "unknown 14"},//
	{do_page_fault, "Store/AMO page fault"},//存储/AMO缺页异常
};

static inline const struct fault_info *ec_to_fault_info(unsigned int scause)
{
	return fault_info + (scause & SCAUSE_EC);
}

// 定时器中断处理函数
void timer_irq_handle(){
	printf("timer_int\n");
	//todo:安排进程调度等工作

	timer_reset();
}

// 软件中断处理函数
void soft_irq_handle(){
	//todo:
	panic("soft_irq todo");
}

// 外部中断处理函数
void ex_irq_handle(){
	//todo:
	panic("ex_irq todo");
}

void do_exception(struct pt_regs *regs, unsigned long scause)
{
	const struct fault_info *inf;

	// printf("%s, scause:%p\n", __func__, scause);

	if (scause & SCAUSE_INT) {
		// 中断异常处理
		switch (scause & ~SCAUSE_INT)
		{
		case SOFT_INT:
			soft_irq_handle();break;
		case TIMER_INT:
			timer_irq_handle();break;
		case EX_INT:
			ex_irq_handle();break;
		default:
			panic("unknown interrupt type");
			break;
		}
	} else {
		// 其他异常报错
		inf = ec_to_fault_info(scause);
		if (!inf->fn(regs, inf->name))
			return;
	}
}

void trap_init(void)
{
	w_sscratch(0);
	/* 设置异常向量表地址 */
	w_stvec((uint64)do_exception_vector);
	printf("stvec=%p, %p\n", r_stvec(), do_exception_vector);
	/* 使能所有中断 */
	w_sie(-1);
}
