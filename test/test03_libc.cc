#include "hardware.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <pic_hardware.h>

extern uint32_t __stack_end;

int main()
{
  puts("main()");
  printf( "stackend = %p\n", __stack_end );
  for(unsigned n=0;n<10;++n)
  {
    DelayMs(500);
    printf("%d\r\n",n);
  }
  puts("OK");
  for(;;);
  return 0;
}
