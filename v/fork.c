#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include "mini-printf.h"

static char linbuf[256];

void show_err(const char *cmd, const char *pth, int errno)
{
  printf("%s(%s): err=%d\n", cmd, pth, -errno);
}

int main(int argc, char **argv)
{
  int i, cid;
  puts("Hello, Linux");
  for (i = 0; i < argc; i++)
    puts(argv[i]);
  while (1)
    {
      int cnt, dummy, argc = 0;
      char *envp[2], *argv[99], *ptr1, *ptr = linbuf;
      puts("Type a line");
      cnt = read(0, linbuf, sizeof(linbuf));
      puts(linbuf);
      linbuf[cnt-1] = 0;
      do {
        ptr1 = strchr(ptr, ' ');
        if (ptr1) *ptr1 = 0;
        argv[argc++] = ptr;
        ptr = ptr1+1;
      } while (ptr1);
      argv[argc] = 0;
      for (i = 0; i < argc; i++)
        {
          printf("argv[%d] = #%s#\n", i, argv[i]);
        }
      dummy = access(*argv, X_OK);
      if (!strcmp(*argv, "exit"))
        {
          _exit(0);
        }
      else if (dummy)
        {
          show_err("access", *argv, dummy);
        }
      else
        {
          cid = fork();
          if (!cid)
            {
              envp[0] = 0;
              printf("Child %s", *argv);
              dummy = execve(*argv, argv, envp);
              if (dummy < 0) show_err("execve", *argv, dummy);
              puts("child exit");
              _exit(1);
            }
          else
            {
              int status;
              struct rusage rusage;
              printf("Parent here, child id = %d\n", cid);
              wait4(cid, &status, 0, &rusage);
              printf("wait4 status = %x\n", status);
            }
        }
    }
}
