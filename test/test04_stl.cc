#include "hardware.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <map>

#include <pic_hardware.h>

struct S  {  size_t n=0;  S( size_t _n ) : n(_n) {}  virtual void m()=0;  };
template <char c>
struct T : S {  T( size_t _n ) : S(_n) {}  virtual void m(){  printf("T%c\n",c);  }  };

std::map<size_t,S*> m;

int main()
{
  puts("main()");
  for(size_t n=0;n<30;++n)
  {
    DelayMs(500);
    if( m.count(n-10) )
    {
      S *s = m[n-10];
      m.erase(n-10);
      s->m();
      delete s;
    }
    if(n<20)
    {
      m.emplace( n, ( n%2 == 0 ) ? (S*)new T<'1'>(n) : (S*)new T<'2'>(n) );
    }
    std::cout << n << " " << m.size() << std::endl;
  }
  if( !m.size() )
    std::cout << "OK" << std::endl;
  else
    puts("ERR");
  for(;;);
  return 0;
}
