// See LICENSE for license details.

#include <string.h>
#include "hid.h"
#include "encoding.h"
#include "mini-printf.h"
#define snprintf mini_snprintf

volatile uint64_t *const uart_base = (uint64_t *)0x41004000;

int myputchar(int ch)
{
  uart_base[0] = ch;
  return ch;
}

void uart_send_string(const char *str) {
  while (*str) myputchar(*str++);
}

int puts(const char *s)
{
  uart_send_string(s);
  uart_console_putchar('\n');
}
