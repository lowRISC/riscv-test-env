#include "riscv_test.h"

        .align 12
        RVTEST_RV64U
        RVTEST_CODE_BEGIN
        .globl userstart
userstart:      
        .incbin "dram.bin"
        .globl userstop
userstop:      
