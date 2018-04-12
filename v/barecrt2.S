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
        la a1, 3f
        li t1, 1
1:      ld t0, 0(a1)
        sd t0, 0(a0)
        ld t0, 8(a1)
        sd t0, 8(a0)
        ld t0, 16(a1)
        sd t0, 16(a0)
        ld t0, 24(a1)
        sd t0, 24(a0)
        ld t0, 32(a1)
        sd t0, 32(a0)
        addi a0,a0,32
        addi t1,t1,-1
        bnez t1,1b
        jal ra, main
exit:   j exit
        .data
        .align 4
3:      .asciz "Hello, Ariane!                                "
