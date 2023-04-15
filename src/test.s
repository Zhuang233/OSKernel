.section .text.test

.globl test
test:
    li a0, 0x70000000
    ld a0, (a0)
    ret