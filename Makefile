BUILDDIR = build
SRCDIR = src
CC = riscv64-unknown-elf-gcc
CFLAGS = -nostdlib -fno-builtin #不使用标准库 不识别内建函数
LD = riscv64-unknown-elf-ld
LDFLAGS = -T link.ld
SBI = SBI/rustsbi-qemu
QEMU = qemu-system-riscv64
QEMUFLAGS = -machine virt -m 128M -nographic -kernel $(BUILDDIR)/kernel
SRCS_S := $(shell find ./src -name "*.s")
SRCS_C := $(shell find ./src -name "*.c")
SRCS = $(SRCS_C) $(SRCS_S)
OBJS = $(addprefix $(BUILDDIR)/, $(notdir $(addsuffix .o, $(basename $(SRCS))))) # basename去前缀，addsuffix加后缀，addprefix加前缀，得到目标.o文件及其路径

.PHONY: all
all: $(BUILDDIR) $(BUILDDIR)/kernel

$(BUILDDIR):
	@mkdir $(BUILDDIR) 

$(BUILDDIR)/kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@  $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all
run: all
	$(QEMU) $(QEMUFLAGS) -bios $(SBI)

.PHONY: clean
clean:
	@rm -rf $(BUILDDIR)

.PHONY: echo
echo:
	@echo $(SRCS)
	@echo $(OBJS)