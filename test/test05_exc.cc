#include "hardware.h"

#include <stdint.h>
#include <iostream>
#include <exception>
#include <assert.h>

#include <pic_hardware.h>

[[noreturn]] void Terminate() noexcept
{
  std::cout << "Terminate()" << std::endl;
  abort();
}

namespace __cxxabiv1
{
  std::terminate_handler __terminate_handler = Terminate;
}

extern "C" void __tzset()         noexcept {}
extern "C" void __tz_lock()       noexcept {}
extern "C" void __tz_unlock()     noexcept {}
extern "C" void _tzset_unlocked() noexcept {}


void raise()
{
  throw 5;
}

struct S{ virtual ~S(){} };
struct T: public S{};
struct U: public S{};

int main()
{
  std::cout << "main()" << std::endl;

  S *s = new T();
  T *t = dynamic_cast<T*>(s);
  std::cout << "t:" << (void *)t << std::endl;
  assert(t);
  assert(!dynamic_cast<U*>(s));
  delete t;

  for(size_t n=0;n<10;++n)
  {
    DelayMs(200);
    try
    {
      std::cout << "raise" << std::endl;
      raise();
    }
    catch( int i )
    {
      std::cout << "caught " << i << std::endl;
    }
    catch(...)
    {
      std::cout << "caught ..." << std::endl;
      puts("ERR");
      for(;;);
    }
    std::cout << "n: " << n++ << std::endl;
  }
  puts("OK");
  for(;;);
  return 0;
}
