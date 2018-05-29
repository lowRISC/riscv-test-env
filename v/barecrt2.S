#define icache 0x700
#define dcache 0x701
#define cacheaddr 0x702
#define dummy 0x777
        .text
        .globl _start
_start: nop
        nop
        jal ra,0f
trap:   csrr ra, mepc
        csrr s0, mcause
        j trap
0:      la t0, 2f
        csrw mtvec, t0
/*
        csrw icache, zero
        csrw dcache, zero
        */
        li t0, 0x80000000
        li t0, 0x88000000
        csrw cacheaddr, t0
2:      csrw mtvec, ra
        li sp, 0x40007F00
/*
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
        li a0, 0x41008800
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
        li a0, 0x41006000
        li t0, 216
1:      sd t0, 0(a0)
*/        
        jal ra, main
exit:   j exit
        .data
        .align 4
3:      .asciz "Hello, Ariane!                                "
