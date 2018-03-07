#include "riscv_test.h"

#if __riscv_xlen == 64
# define STORE    sd
# define LOAD     ld
# define REGBYTES 8
#else
# define STORE    sw
# define LOAD     lw
# define REGBYTES 4
#endif

  .section ".text.init","ax",@progbits
  .globl _start
_start:
  RVTEST_RV64M
        auipc t0, 0
        li t1, DRAM_BASE
        sub t2, t0, t1
        beqz t2, handle_reset
        # Let's just wait a bit in case we want to debug this bit
        li t2, 10 # 10000000
1:      addi t2, t2, -1
        bnez t2, 1b
        # Relocate to correct place in DRAM
        mv ra, t1
        la t2, _bss_start
        sub t2, t2, t0
2:
        LOAD s0, 0(t0)
        STORE s0, 0(t1)
        addi t0, t0, 8
        addi t1, t1, 8
        addi t2, t2, -8
        bgtz t2, 2b
        fence.i
        ret

  /* NMI vector */
nmi_vector:
  STORE x1, 1*REGBYTES(sp)
  STORE x2, 2*REGBYTES(sp)
  STORE x3, 3*REGBYTES(sp)
  STORE x4, 4*REGBYTES(sp)
  STORE x5, 5*REGBYTES(sp)
  STORE x6, 6*REGBYTES(sp)
  STORE x7, 7*REGBYTES(sp)
  STORE x8, 8*REGBYTES(sp)
  STORE x9, 9*REGBYTES(sp)
  STORE x10, 10*REGBYTES(sp)
  STORE x11, 11*REGBYTES(sp)
  STORE x12, 12*REGBYTES(sp)
  STORE x13, 13*REGBYTES(sp)
  STORE x14, 14*REGBYTES(sp)
  STORE x15, 15*REGBYTES(sp)
  STORE x16, 16*REGBYTES(sp)
  STORE x17, 17*REGBYTES(sp)
  STORE x18, 18*REGBYTES(sp)
  STORE x19, 19*REGBYTES(sp)
  STORE x20, 20*REGBYTES(sp)
  STORE x21, 21*REGBYTES(sp)
  STORE x22, 22*REGBYTES(sp)
  STORE x23, 23*REGBYTES(sp)
  STORE x24, 24*REGBYTES(sp)
  STORE x25, 25*REGBYTES(sp)
  STORE x26, 26*REGBYTES(sp)
  STORE x27, 27*REGBYTES(sp)
  STORE x28, 28*REGBYTES(sp)
  STORE x29, 29*REGBYTES(sp)
  STORE x30, 30*REGBYTES(sp)
  STORE x31, 31*REGBYTES(sp)

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

  LOAD x1, 1*REGBYTES(sp)
  LOAD x2, 2*REGBYTES(sp)
  LOAD x3, 3*REGBYTES(sp)
  LOAD x4, 4*REGBYTES(sp)
  LOAD x5, 5*REGBYTES(sp)
  LOAD x6, 6*REGBYTES(sp)
  LOAD x7, 7*REGBYTES(sp)
  LOAD x8, 8*REGBYTES(sp)
  LOAD x9, 9*REGBYTES(sp)
  LOAD x10, 10*REGBYTES(sp)
  LOAD x11, 11*REGBYTES(sp)
  LOAD x12, 12*REGBYTES(sp)
  LOAD x13, 13*REGBYTES(sp)
  LOAD x14, 14*REGBYTES(sp)
  LOAD x15, 15*REGBYTES(sp)
  LOAD x16, 16*REGBYTES(sp)
  LOAD x17, 17*REGBYTES(sp)
  LOAD x18, 18*REGBYTES(sp)
  LOAD x19, 19*REGBYTES(sp)
  LOAD x20, 20*REGBYTES(sp)
  LOAD x21, 21*REGBYTES(sp)
  LOAD x22, 22*REGBYTES(sp)
  LOAD x23, 23*REGBYTES(sp)
  LOAD x24, 24*REGBYTES(sp)
  LOAD x25, 25*REGBYTES(sp)
  LOAD x26, 26*REGBYTES(sp)
  LOAD x27, 27*REGBYTES(sp)
  LOAD x28, 28*REGBYTES(sp)
  LOAD x29, 29*REGBYTES(sp)
  LOAD x30, 30*REGBYTES(sp)
  LOAD x31, 31*REGBYTES(sp)

  addi sp, sp, 272
  mret

  .globl trap_vector        
trap_vector:
        csrrw sp, mscratch, sp
        STORE a0, 10*REGBYTES(sp)
        STORE a1, 11*REGBYTES(sp)
        csrr a1, mepc
        addi a0, a1, 4
        csrw mepc, a0
        csrr a0, mcause
1:      ebreak
        j 1b
        csrw mscratch, sp
        LOAD a0, 10*REGBYTES(sp)
        LOAD a1, 11*REGBYTES(sp)
        mret

handle_reset:
  la t0, trap_vector
  csrw mtvec, t0
  li sp, (0x40010000 - SIZEOF_TRAPFRAME_T)
  # Clear BSS segment
  la a4, _bss_start
  la a5, _bss_end
  sub a5, a5, a4      
2:      
  STORE zero, 0(a4)
  addi a4, a4, 8
  addi a5, a5, -8
  bgtz a5, 2b
  csrr t0, mhartid
  slli t0, t0, 12
  add sp, sp, t0
  csrw mscratch, sp
  la a0, userstart
  j vm_boot

  .globl  pop_tf
pop_tf:
  LOAD  t0,33*REGBYTES(a0)
  csrw  sepc,t0
  LOAD  x1,1*REGBYTES(a0)
  LOAD  x2,2*REGBYTES(a0)
  LOAD  x3,3*REGBYTES(a0)
  LOAD  x4,4*REGBYTES(a0)
  LOAD  x5,5*REGBYTES(a0)
  LOAD  x6,6*REGBYTES(a0)
  LOAD  x7,7*REGBYTES(a0)
  LOAD  x8,8*REGBYTES(a0)
  LOAD  x9,9*REGBYTES(a0)
  LOAD  x11,11*REGBYTES(a0)
  LOAD  x12,12*REGBYTES(a0)
  LOAD  x13,13*REGBYTES(a0)
  LOAD  x14,14*REGBYTES(a0)
  LOAD  x15,15*REGBYTES(a0)
  LOAD  x16,16*REGBYTES(a0)
  LOAD  x17,17*REGBYTES(a0)
  LOAD  x18,18*REGBYTES(a0)
  LOAD  x19,19*REGBYTES(a0)
  LOAD  x20,20*REGBYTES(a0)
  LOAD  x21,21*REGBYTES(a0)
  LOAD  x22,22*REGBYTES(a0)
  LOAD  x23,23*REGBYTES(a0)
  LOAD  x24,24*REGBYTES(a0)
  LOAD  x25,25*REGBYTES(a0)
  LOAD  x26,26*REGBYTES(a0)
  LOAD  x27,27*REGBYTES(a0)
  LOAD  x28,28*REGBYTES(a0)
  LOAD  x29,29*REGBYTES(a0)
  LOAD  x30,30*REGBYTES(a0)
  LOAD  x31,31*REGBYTES(a0)
  LOAD  a0,10*REGBYTES(a0)
  sret

  .global  trap_entry
  .align 4
trap_entry:
  csrrw sp, sscratch, sp

  # save gprs
  STORE  x1,1*REGBYTES(sp)
  STORE  x3,3*REGBYTES(sp)
  STORE  x4,4*REGBYTES(sp)
  STORE  x5,5*REGBYTES(sp)
  STORE  x6,6*REGBYTES(sp)
  STORE  x7,7*REGBYTES(sp)
  STORE  x8,8*REGBYTES(sp)
  STORE  x9,9*REGBYTES(sp)
  STORE  x10,10*REGBYTES(sp)
  STORE  x11,11*REGBYTES(sp)
  STORE  x12,12*REGBYTES(sp)
  STORE  x13,13*REGBYTES(sp)
  STORE  x14,14*REGBYTES(sp)
  STORE  x15,15*REGBYTES(sp)
  STORE  x16,16*REGBYTES(sp)
  STORE  x17,17*REGBYTES(sp)
  STORE  x18,18*REGBYTES(sp)
  STORE  x19,19*REGBYTES(sp)
  STORE  x20,20*REGBYTES(sp)
  STORE  x21,21*REGBYTES(sp)
  STORE  x22,22*REGBYTES(sp)
  STORE  x23,23*REGBYTES(sp)
  STORE  x24,24*REGBYTES(sp)
  STORE  x25,25*REGBYTES(sp)
  STORE  x26,26*REGBYTES(sp)
  STORE  x27,27*REGBYTES(sp)
  STORE  x28,28*REGBYTES(sp)
  STORE  x29,29*REGBYTES(sp)
  STORE  x30,30*REGBYTES(sp)
  STORE  x31,31*REGBYTES(sp)

  csrrw  t0,sscratch,sp
  STORE  t0,2*REGBYTES(sp)

  # get sr, epc, badvaddr, cause
  csrr   t0,sstatus
  STORE  t0,32*REGBYTES(sp)
  csrr   t0,sepc
  STORE  t0,33*REGBYTES(sp)
  csrr   t0,sbadaddr
  STORE  t0,34*REGBYTES(sp)
  csrr   t0,scause
  STORE  t0,35*REGBYTES(sp)

  move  a0, sp
        j handle_trap
#if 1
        .data
cursor: 
        .word   0
#endif
        
