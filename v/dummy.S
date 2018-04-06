        .globl _start
_start: li sp, 0x4000FF00
        li a0, 0x41008000
        li t0, 0x2020202020202020
        li t2, 0x0101010101010101
1:      li t1, 128
        add t0, t0, t2
2:      sd t0,(a0)
        sd t0,8(a0)
        sd t0,16(a0)
        sd t0,24(a0)
        addi a0,a0,32
        addi t1, t1, -1
        bnez t1, 2b
        j 1b
