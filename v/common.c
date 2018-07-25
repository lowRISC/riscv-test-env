// See LICENSE for license details.

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "hid.h"
#include "eth.h"
#include "lowrisc_memory_map.h"
#include "mini-printf.h"

uip_eth_addr mac_addr;



// LowRISC Ethernet base address
volatile uint64_t *const eth_base = (volatile uint64_t*)eth_base_addr;
volatile uint64_t *const sd_base = (uint64_t *)sd_base_addr;
volatile uint64_t *const uart_base = (uint64_t *)uart_base_addr;
const size_t err = 0x3000, eth = eth_base_addr, ddr = ddr_base_addr, rom = 0x10000, bram = bram_base_addr, intc = 0xc000000, clin = 0; // , hid = 0x41000000;
volatile uint16_t *const hid_vga_ptr = (uint16_t *)vga_base_addr;
volatile uint32_t *const plic = (volatile uint32_t *)plic_base_addr;

volatile uint64_t * get_ddr_base() {
  return (uint64_t *)ddr_base_addr;
}

volatile uint64_t  get_ddr_size() {
  return (uint64_t)0x8000000;
}

void write_led(uint32_t data)
{
  sd_base[15] = data;
}

void _init()
{
  int sw;
  uint32_t macaddr_lo, macaddr_hi;
  extern int main(int, char **);
  extern char _bss_start[], _bss_end[];
  size_t unknown = 0;
  char *unknownstr, *config = (char *)0x10000;
  size_t bsslen = _bss_end - _bss_start;
  memset(_bss_start, 0, bsslen);
  sw = sd_base[31];
  mac_addr.addr[0] = (uint8_t)0xEE;
  mac_addr.addr[1] = (uint8_t)0xE1;
  mac_addr.addr[2] = (uint8_t)0xE2;
  mac_addr.addr[3] = (uint8_t)0xE3;
  mac_addr.addr[4] = (uint8_t)0xE4;
  mac_addr.addr[5] = (uint8_t)(0xE0|(sw >> 12));

  memcpy (&macaddr_lo, mac_addr.addr+2, sizeof(uint32_t));
  memcpy (&macaddr_hi, mac_addr.addr+0, sizeof(uint16_t));
  eth_base[MACLO_OFFSET>>3] = htonl(macaddr_lo);
  eth_base[MACHI_OFFSET>>3] = htons(macaddr_hi);

  macaddr_lo = eth_base[MACLO_OFFSET>>3];
  macaddr_hi = eth_base[MACHI_OFFSET>>3] & MACHI_MACADDR_MASK;
  printf("Calling main with MAC = %x:%x\n", macaddr_hi&MACHI_MACADDR_MASK, macaddr_lo);
 
  int ret = main(1, NULL);
}
