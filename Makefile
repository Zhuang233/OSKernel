BUILDDIR = build
SBI = SBI/rustsbi-qemu
QEMU = qemu-system-riscv64
QEMUFLAGS = -machine virt -m 128M -nographic  
# -kernel $(BUILDDIR)/kernel

all:
	
run:
	$(QEMU) $(QEMUFLAGS) -bios $(SBI)