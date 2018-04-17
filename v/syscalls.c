#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include "mini-printf.h"

void __attribute__((section(".text.init"))) _start(void)
{
  _exit(main());
}

#ifdef __riscv
#define syscall mini_syscall

long syscall (long syscall_number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7)
{
  long ret;
  long _sys_result;
  register long __a7 asm("a7") = syscall_number;
  register long __a0 asm("a0") = (long) (arg1);
  register long __a1 asm("a1") = (long) (arg2);
  register long __a2 asm("a2") = (long) (arg3);
  register long __a3 asm("a3") = (long) (arg4);
  register long __a4 asm("a4") = (long) (arg5);
  register long __a5 asm("a5") = (long) (arg6);
  register long __a6 asm("a6") = (long) (arg7);
  __asm__ volatile ( "scall\n\t" : "+r" (__a0) : "r" (__a7), "r"(__a1), "r"(__a2), "r"(__a3), "r"(__a4), "r"(__a5), "r"(__a6) : "memory");
  _sys_result = __a0;
  ret = _sys_result;
  return ret;
}

#else
long mini_syscall (long syscall_number, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6, long arg7)
{
  return syscall (syscall_number, arg1, arg2, arg3, arg4, arg5, arg6, arg7);
}

#endif

void *sbrk(intptr_t siz)
{
  static char *base;
  char *retval, *oldbase;
  if (!base)
    base = (void *)syscall(SYS_brk, 0, 0, 0, 0, 0, 0, 0);
  oldbase = base;
  retval = (void *)syscall(SYS_brk, (long)(oldbase+siz), 0, 0, 0, 0, 0, 0);
  if (base < retval)
    base = retval;
  return oldbase;
}

ssize_t write(int fd, const void *buf, size_t count)
{
  return syscall(SYS_write, fd, (long)buf, count, 0, 0, 0, 0);
}

int putchar(int ch)
{
  write(1, &ch, 1);
  return ch;
}

int puts(const char *s)
{
  write(1, s, strlen(s));
  putchar('\n');
}

void _exit(int code)
{
  for (;;) syscall(SYS_exit, code, 0, 0, 0, 0, 0, 0);
}
