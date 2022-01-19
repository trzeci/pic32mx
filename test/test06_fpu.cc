#include "hardware.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <iomanip>
#include <set>
#include <math.h>

#include <pic_hardware.h>

long double d = 0;

int main()
{
  puts("main()");
  for(size_t n=0;n<30;++n)
  {
    DelayMs(100);
    auto v = ::pow(::sin(d),2)+::pow(::cos(d),2);
    std::cout << ::std::setw(6) << d << " " << v << std::endl;
    d += .1l;
    if( ::fabs(v-1.) > .000001 )
    {
      puts("ERR");
      for(;;);
    }
  }
  puts("OK");
  for(;;);
  return 0;
}
