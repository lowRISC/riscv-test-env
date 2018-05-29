#include "mini-printf.h"

int main(int argc, char **argv)
{
  mini_printf("\nUART test\n");
  for(;;)
    ;
}

void external_interrupt(void)
{
  int i, claim, handled = 0;
  mini_printf("Hello external interrupt! "__TIMESTAMP__"\n");
}
