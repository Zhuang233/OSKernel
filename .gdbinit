set confirm off
set architecture riscv:rv64
target remote 127.0.0.1:25000
symbol-file build/kernel_test
set disassemble-next-line auto
b main
c
