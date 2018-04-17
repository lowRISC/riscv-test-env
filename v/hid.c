// See LICENSE for license details.

#include "hid.h"
#include "mini-printf.h"

//enum {scroll_start=4096-256};
enum {scroll_start=0};
volatile uint32_t *const sd_base = (uint32_t *)0x41010000;
volatile uint8_t *const hid_vga_ptr = (uint8_t *)0x41008000;
const size_t err = 0x3000, eth = 0x41020000, ddr = 0x80000000, rom = 0x10000, bram = 0x40000000, intc = 0xc000000, clin = 0, hid = 0x41000000;
static int addr_int = scroll_start;

void hid_console_putchar(unsigned char ch)
{
  int lmt;
  switch(ch)
    {
    case 8: case 127: if (addr_int & 127) hid_vga_ptr[--addr_int] = ' '; break;
    case 13: addr_int = addr_int & -128; break;
    case 10:
      {
        int lmt = (addr_int|127)+1; while (addr_int < lmt) hid_vga_ptr[(addr_int++)] = ' ';
        break;
      }
    default: hid_vga_ptr[addr_int++] = ch;
    }
  if (addr_int >= 4096-128)
    {
      // this is where we scroll
      for (addr_int = 0; addr_int < 4096; addr_int++)
        if (addr_int < 4096-128)
          hid_vga_ptr[addr_int] = hid_vga_ptr[addr_int+128];
        else
          hid_vga_ptr[addr_int] = ' ';
      addr_int = 4096-256;
    }
}

int putchar(int ch)
{
  hid_console_putchar(ch);
  return ch;
}

void hid_init(void)
{
  if (addr_int != scroll_start)
    addr_int = scroll_start;
#if 0  
  size_t unknown = 0;
  char *unknownstr, *config = (char *)0x10000;
  for (int i = 128; i < 4096; i++)
    {
      char ch = config[i] & 0x7f; 
      if (ch == '@')
        {
          int j = i+1;
          unsigned addr = 0;
          while ((config[j] >= '0' && config[j] <= '9') || (config[j] >= 'a' && config[j] <= 'f'))
            {
              int hex = config[j] - '0';
              if (hex > 9) hex = config[j] - 'a' + 10;
              addr = (addr << 4) | hex;
              j++;
            }
          j = i - 1;
          while ((config[j] >= '0' && config[j] <= '9') || (config[j] >= 'a' && config[j] <= 'z') || config[j] == '-')
            j--;
          if ((++j < i) && addr)
            {
              uint32_t label = (config[j]<<24) | (config[j+1]<<16) | (config[j+2]<<8) |(config[j+3]);
              switch (label)
                {
                case 'memo':
                  ddr = addr;
                  break;
                case 'bram':
                  bram = addr;
                  break;
                case 'clin':
                  clin = addr;
                  break;
                case 'erro':
                  err = addr;
                  break;
                case 'eth@':
                  eth = addr;
                  break;
                case 'inte':
                  intc = addr;
                  break;
                case 'rom@':
                  rom = addr;
                  break;
                case 'mmio':
                  if (config[j+4] == '@')
                    {
                    bram = addr;
                    }
                  else
                    {
                      hid = addr;
                      eth = addr + 0x20000;
                    }
                  break;
                default:
                  unknown = addr;
                  unknownstr = config+j;
                  break;
                }
            }
        }
    }
  if (hid)
    {
      void *base = (void *)hid;
      hid_base_ptr = (uint32_t *)base;
      sd_base = (uint32_t *)(0x00010000 + (char *)base);
      printf("MMIO2 (32-bit) start 0x%x\n", hid);
      if (eth) printf("Ethernet start 0x%x\n", eth);
      if (err) printf("Error device start 0x%x\n", err);
      if (rom) printf("ROM start 0x%x\n", rom);
      if (bram) printf("Block RAM start 0x%x\n", bram);
      if (ddr) printf("DDR memory start 0x%x\n", ddr);
      if (intc) printf("Interrupt controller start 0x%x\n", intc);
      if (unknown)
        printf("Unknown %s, start = 0x%x\n", unknownstr, unknown);
    }
#endif
}

void hid_send_string(const char *str) {
  while (*str) hid_console_putchar(*str++);
}

int puts(const char *s)
{
  hid_send_string(s);
  hid_console_putchar('\n');
}

void handle_interrupt(long cause)
{
  int mip;
  char code[20];
  cause &= 0x7FFFFFFF;
#ifdef VERBOSE_INTERRUPT
  switch(cause)
    {
    case IRQ_S_SOFT   : strcpy(code, "IRQ_S_SOFT   "); break;
    case IRQ_H_SOFT   : strcpy(code, "IRQ_H_SOFT   "); break;
    case IRQ_M_SOFT   : strcpy(code, "IRQ_M_SOFT   "); break;
    case IRQ_S_TIMER  : strcpy(code, "IRQ_S_TIMER  "); break;
    case IRQ_H_TIMER  : strcpy(code, "IRQ_H_TIMER  "); break;
    case IRQ_M_TIMER  : strcpy(code, "IRQ_M_TIMER  "); break;
    case IRQ_S_DEV    : strcpy(code, "IRQ_S_DEV    "); break;
    case IRQ_H_DEV    : strcpy(code, "IRQ_H_DEV    "); break;
    case IRQ_M_DEV    : strcpy(code, "IRQ_M_DEV    "); break;
    case IRQ_COP      : strcpy(code, "IRQ_COP      "); break;
    case IRQ_HOST     : strcpy(code, "IRQ_HOST     "); break;
    default           : snprintf(code, sizeof(code), "IRQ_%x     ", cause);
    }
 hid_send_string(code);
 mip = read_csr(mip);
 snprintf(code, sizeof(code), "mip=%x\n", mip);
 hid_send_string(code);
#endif
#if 0 
 if (cause==IRQ_M_DEV)
   external_interrupt();
#endif 
}
