//#define SIM
// A hello int program
#include "encoding.h"
#include "bits.h"
#include "elf.h"
#include "hid.h"
#include "mini-printf.h"
#include "memory.h"
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "encoding.h"
#define snprintf mini_snprintf

enum {SD_CARD_RESP_END=1,SD_CARD_RW_END=2, SD_CARD_CARD_REMOVED_0=4, SD_CARD_CARD_INSERTED_0=8};

enum {align_reg,clk_din_reg,arg_reg,cmd_reg,
      setting_reg,start_reg,reset_reg,blkcnt_reg,
      blksiz_reg,timeout_reg,clk_pll_reg,irq_en_reg,
      unused1,unused2,unused3,led_reg};

enum {resp0,resp1,resp2,resp3,
      wait_resp,status_resp,packet_resp0,packet_resp1,
      data_wait_resp,trans_cnt_resp,obsolete1,obsolet2,
      detect_resp,xfr_addr_resp,irq_stat_resp,pll_resp,
      align_resp,clk_din_resp,arg_resp,cmd_i_resp,
      setting_resp,start_resp,reset_resp,blkcnt_resp,
      blksize_resp,timeout_resp,clk_pll_resp,irq_en_resp};

void sd_irq_en(int mask)
{
  sd_base[irq_en_reg] = mask;
}

void external_interrupt(void)
{
  int handled = 0;
  printf("Hello external interrupt! "__TIMESTAMP__"\n");
  sd_irq_en(0);
  if (!handled)
    {
      printf("unhandled interrupt!\n");
    }
}

volatile uint64_t * get_ddr_base() {
  return (uint64_t *)(0x80000000);
}

void *mysbrk(size_t len)
{
  static unsigned long rused = 0;
  char *rd = rused + (char *)get_ddr_base();
  rused += ((len-1)|7)+1;
  return rd;
}

ssize_t sys_write(int fd, const void *buf, size_t count)
{
  char *ptr = (char *)buf;
  while (count--)
    hid_console_putchar(*ptr++);
}

int main(void) {
  int old_mstatus, old_mie;
  hid_init();
  printf("Hello LowRISC! "__TIMESTAMP__"\n");
  printf("Enabling interrupts\n");
  old_mstatus = read_csr(mstatus);
  old_mie = read_csr(mie);
  set_csr(mstatus, MSTATUS_MIE|MSTATUS_HIE);
  set_csr(mie, ~(1 << IRQ_M_TIMER));
  printf("Interrupts are now enabled\n");
  sd_irq_en(SD_CARD_CARD_REMOVED_0 | SD_CARD_CARD_INSERTED_0);
  printf("Interrupt triggered!\n");
}
