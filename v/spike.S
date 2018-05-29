#if __riscv_xlen == 64
# define LOAD     ld
#else
# define LOAD     lw
#endif

  .section ".text.init","ax",@progbits
  .globl _start
_start: auipc  t0,0x0
        li     a1, 0x40008000
        csrr   a0, mhartid
        LOAD   t0,24(t0)
        jr     t0
