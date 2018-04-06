        .text
        .globl _start
_start: jal ra,0f
trap:   csrr ra, mepc
        csrr s0, mcause
        j trap
0:      csrw mtvec, ra
        li sp, 0x4000FF00
        jal ra, main
exit:   j exit
