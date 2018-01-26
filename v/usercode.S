#include "riscv_test.h"

        .align 12
        RVTEST_RV64U
        RVTEST_CODE_BEGIN
        .globl userstart
        li a0, 64
1:
        li t0, 1024
2:      
        add t0,t0,-1
        bnez t0, 2b
        ecall
        add a0,a0,1
        j 1b
