#define icache 0x700
#define dcache 0x701
#define dummy 0x777
#define sinten 0x130
#define minten 0x330
        
#include "encoding.h"

        .text
#ifdef __riscv64
# define LREG ld
# define SREG sd
# define REGBYTES 8
#else
# define LREG lw
# define SREG sw
# define REGBYTES 4
#endif
        .globl _start
_start: nop
        nop
        jal ra,0f
trap_entry:
  addi sp, sp, -272

  SREG x1, 1*REGBYTES(sp)
  SREG x2, 2*REGBYTES(sp)
  SREG x3, 3*REGBYTES(sp)
  SREG x4, 4*REGBYTES(sp)
  SREG x5, 5*REGBYTES(sp)
  SREG x6, 6*REGBYTES(sp)
  SREG x7, 7*REGBYTES(sp)
  SREG x8, 8*REGBYTES(sp)
  SREG x9, 9*REGBYTES(sp)
  SREG x10, 10*REGBYTES(sp)
  SREG x11, 11*REGBYTES(sp)
  SREG x12, 12*REGBYTES(sp)
  SREG x13, 13*REGBYTES(sp)
  SREG x14, 14*REGBYTES(sp)
  SREG x15, 15*REGBYTES(sp)
  SREG x16, 16*REGBYTES(sp)
  SREG x17, 17*REGBYTES(sp)
  SREG x18, 18*REGBYTES(sp)
  SREG x19, 19*REGBYTES(sp)
  SREG x20, 20*REGBYTES(sp)
  SREG x21, 21*REGBYTES(sp)
  SREG x22, 22*REGBYTES(sp)
  SREG x23, 23*REGBYTES(sp)
  SREG x24, 24*REGBYTES(sp)
  SREG x25, 25*REGBYTES(sp)
  SREG x26, 26*REGBYTES(sp)
  SREG x27, 27*REGBYTES(sp)
  SREG x28, 28*REGBYTES(sp)
  SREG x29, 29*REGBYTES(sp)
  SREG x30, 30*REGBYTES(sp)
  SREG x31, 31*REGBYTES(sp)

  csrr a0, mcause
  csrr a1, mepc
  mv a2, sp
  bgez a0, 1f
  jal handle_interrupt
  j 2f      
1:
  jal handle_trap
  csrw mepc, a0
2:

  # Remain in M-mode after eret
  li t0, MSTATUS_MPP
  csrs mstatus, t0

  LREG x1, 1*REGBYTES(sp)
  LREG x2, 2*REGBYTES(sp)
  LREG x3, 3*REGBYTES(sp)
  LREG x4, 4*REGBYTES(sp)
  LREG x5, 5*REGBYTES(sp)
  LREG x6, 6*REGBYTES(sp)
  LREG x7, 7*REGBYTES(sp)
  LREG x8, 8*REGBYTES(sp)
  LREG x9, 9*REGBYTES(sp)
  LREG x10, 10*REGBYTES(sp)
  LREG x11, 11*REGBYTES(sp)
  LREG x12, 12*REGBYTES(sp)
  LREG x13, 13*REGBYTES(sp)
  LREG x14, 14*REGBYTES(sp)
  LREG x15, 15*REGBYTES(sp)
  LREG x16, 16*REGBYTES(sp)
  LREG x17, 17*REGBYTES(sp)
  LREG x18, 18*REGBYTES(sp)
  LREG x19, 19*REGBYTES(sp)
  LREG x20, 20*REGBYTES(sp)
  LREG x21, 21*REGBYTES(sp)
  LREG x22, 22*REGBYTES(sp)
  LREG x23, 23*REGBYTES(sp)
  LREG x24, 24*REGBYTES(sp)
  LREG x25, 25*REGBYTES(sp)
  LREG x26, 26*REGBYTES(sp)
  LREG x27, 27*REGBYTES(sp)
  LREG x28, 28*REGBYTES(sp)
  LREG x29, 29*REGBYTES(sp)
  LREG x30, 30*REGBYTES(sp)
  LREG x31, 31*REGBYTES(sp)

  addi sp, sp, 272
  mret
0:      la t0, 2f
        csrw mtvec, t0
/*
        csrw icache, zero
        csrw dcache, zero
*/
        csrw minten, 15
        csrw sinten, 15
        csrw dummy, zero
2:      csrw mtvec, ra
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
        .align 14
        .incbin "../../dummy-riscv64/spike.dtb"
        .align 4
3:      .asciz "Hello, Ariane!                                "
        .data
