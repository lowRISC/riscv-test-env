        .text
        .globl _start
_start: nop
        nop
        jal ra,0f
trap:   csrr ra, mepc
        csrr s0, mcause
        j trap
        csrw mtvec, ra
0:      li sp, 0x40007F00
        jal ra, main
exit:   j exit
