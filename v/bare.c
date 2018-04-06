#include <unistd.h>
#include <sys/syscall.h>
#include "mini-printf.h"
#include "hid.h"

void *sbrk(intptr_t siz)
{
  static char *base = (char *)0x40008000;
  char *retval, *oldbase = base;
  base += ((siz-1) | 7) + 1;
  return oldbase;
}

ssize_t write(int fd, const void *buf, size_t count)
{
  char *ptr = (char *)buf;
  while (count--)
    hid_console_putchar(*ptr++);
}

void _exit(int code)
{
  for (;;)
    ;
}
