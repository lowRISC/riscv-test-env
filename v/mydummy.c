#include <limits.h>
#include <stddef.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include "memory.h"
#include "mini-printf.h"

int main()
{
  void volatile *start;
  printf("\ndummy test\n");
}

void external_interrupt(void)
{
  int i, claim, handled = 0;
#ifdef VERBOSE
  printf("Hello external interrupt! "__TIMESTAMP__"\n");
#endif  
}

ssize_t sys_write(int fd, const void *buf, size_t count)
{
  return count;
}
