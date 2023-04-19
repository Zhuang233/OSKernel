BUILDDIR = build
DUMP = riscv64-unknown-elf-objdump
DUMPDIR = dump
SRCDIR = src
CC = riscv64-unknown-elf-gcc
CFLAGS = -g -nostdlib -fno-builtin #不使用标准库 不识别内建函数
LD = riscv64-unknown-elf-ld
LDFLAGS = -T link.ld
SBI = SBI/rustsbi-qemu
QEMU = qemu-system-riscv64
QEMUFLAGS = -machine virt -m 128M -nographic -kernel


SRCS_S := $(shell find ./src -name "*.s")
SRCS_C := $(shell find ./src -name "*.c")
SRCS = $(SRCS_C) $(SRCS_S)
OBJS = $(addprefix $(BUILDDIR)/, $(notdir $(addsuffix .o, $(basename $(SRCS))))) # basename去前缀，addsuffix加后缀，addprefix加前缀，得到目标.o文件及其路径

SRCS_TEST = $(shell find ./test -name "*.c")
OBJS_TEST = $(addprefix $(BUILDDIR)/, $(notdir $(addsuffix .o, $(basename $(SRCS_TEST))))) $(subst build/main.o,,$(OBJS))
# try to generate a unique GDB port
GDBPORT = $(shell expr `id -u` % 5000 + 25000)
# QEMU's gdb stub command line changed in 0.11
QEMUGDB = $(shell if $(QEMU) -help | grep -q '^-gdb'; \
	then echo "-gdb tcp::$(GDBPORT)"; \
	else echo "-s -p $(GDBPORT)"; fi)

.PHONY: all
all: $(BUILDDIR) $(BUILDDIR)/kernel dump


$(BUILDDIR):
	@mkdir $(BUILDDIR) 

$(BUILDDIR)/kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@  $^

$(BUILDDIR)/kernel_test: $(OBJS_TEST)
	$(LD) $(LDFLAGS) -o $@  $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: test/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SRCDIR)/%.s
	$(CC) $(CFLAGS) -c $< -o $@

# 运行
.PHONY: run
run: all
	$(QEMU) $(QEMUFLAGS) $(BUILDDIR)/kernel -bios $(SBI)

# 生成目标文件信息
.PHONY: dump
dump:
	@mkdir -p $(DUMPDIR)
	$(DUMP) -d $(BUILDDIR)/kernel > $(DUMPDIR)/kernel.asm
	$(DUMP) -S $(BUILDDIR)/kernel > $(DUMPDIR)/kernel.s
	$(DUMP) -t $(BUILDDIR)/kernel | grep '^[0-9]' | sort > $(DUMPDIR)/kernel.sym
	readelf -s $(BUILDDIR)/kernel >>$(DUMPDIR)/kernel.sym
	$(DUMP) -h $(BUILDDIR)/kernel > $(DUMPDIR)/kernel.sections
	size -x $(BUILDDIR)/kernel >> $(DUMPDIR)/kernel.sections

# 单元测试
.PHONY: test
test: $(BUILDDIR) $(BUILDDIR)/kernel_test
	$(QEMU) $(QEMUFLAGS) $(BUILDDIR)/kernel_test -bios $(SBI)

.PHONY: test-gdb
test-gdb: $(BUILDDIR) $(BUILDDIR)/kernel_test
	$(QEMU) $(QEMUFLAGS) $(BUILDDIR)/kernel_test -bios $(SBI) -S $(QEMUGDB)

# 远程调试
.PHONY: gdb
gdb:
	$(QEMU) $(QEMUFLAGS) $(BUILDDIR)/kernel -bios $(SBI) -S $(QEMUGDB)

# 项目清理
.PHONY: clean
clean:
	@rm -rf $(BUILDDIR) $(DUMPDIR)

.PHONY: echo
echo:
	# @echo $(SRCS)
	# @echo $(OBJS)
	@$(OBJS_TEST)