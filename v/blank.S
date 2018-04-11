        .text
        .globl _start
_start: nop
        nop
        jal ra,0f
trap:   csrr ra, mepc
        csrr s0, mcause
        j trap
0:      csrw mtvec, ra
        li sp, 0x4000FF00
        li a0, 0x41008000
        li t1, 128
        li t0, 0x2020202020202020
1:      sd t0, 0(a0)
        sd t0, 8(a0)
        sd t0, 16(a0)
        sd t0, 24(a0)
        addi a0,a0,32
        addi t1, t1, -1
        bnez t1, 1b
2:      sbreak
        j 1b
