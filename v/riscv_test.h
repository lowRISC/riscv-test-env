// See LICENSE for license details.

#ifndef _ENV_VIRTUAL_SINGLE_CORE_H
#define _ENV_VIRTUAL_SINGLE_CORE_H

#include "../p/riscv_test.h"

//-----------------------------------------------------------------------
// Begin Macro
//-----------------------------------------------------------------------

#undef RVTEST_FP_ENABLE
#define RVTEST_FP_ENABLE fssr x0

#undef RVTEST_CODE_BEGIN
#define RVTEST_CODE_BEGIN                                               \
        .text;                                                          \
        .global userstart;                                              \
userstart:                                                              \
        init

//-----------------------------------------------------------------------
// Pass/Fail Macro
//-----------------------------------------------------------------------

#undef RVTEST_PASS
#define RVTEST_PASS li a0, 1; scall

#undef RVTEST_FAIL
#define RVTEST_FAIL sll a0, TESTNUM, 1; 1:beqz a0, 1b; or a0, a0, 1; scall;

//-----------------------------------------------------------------------
// Data Section Macro
//-----------------------------------------------------------------------

#undef RVTEST_DATA_END
#define RVTEST_DATA_END

//-----------------------------------------------------------------------
// Supervisor mode definitions and macros
//-----------------------------------------------------------------------

#define MAX_TEST_PAGES 63 // this must be the period of the LFSR below
#define LFSR_NEXT(x) (((((x)^((x)>>1)) & 1) << 5) | ((x) >> 1))

#define PGSHIFT 12
#define PGSIZE (1UL << PGSHIFT)

#define SIZEOF_TRAPFRAME_T ((__riscv_xlen / 8) * 36)

#ifndef __ASSEMBLER__

typedef unsigned long pte_t;
#define LEVELS (sizeof(pte_t) == sizeof(uint64_t) ? 3 : 2)
#define PTIDXBITS (PGSHIFT - (sizeof(pte_t) == 8 ? 3 : 2))
#define VPN_BITS (PTIDXBITS * LEVELS)
#define VA_BITS (VPN_BITS + PGSHIFT)
#define PTES_PER_PT (1UL << RISCV_PGLEVEL_BITS)
#define MEGAPAGE_SIZE (PTES_PER_PT * PGSIZE)

enum {__zero, __ra, __sp, __gp,
      __tp, __t0, __t1, __t2,
      __fp, __s1, __a0, __a1,
      __a2, __a3, __a4, __a5,
      __a6, __a7, __s2, __s3,
      __s4, __s5, __s6, __s7,
      __s8, __s9, __s10, __s11,
      __t3, __t4, __t5, __t6};

typedef struct
{
  long gpr[32];
  long sr;
  long epc;
  long badvaddr;
  long cause;
} trapframe_t;
#endif

#endif
