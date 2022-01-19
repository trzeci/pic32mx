#include "hardware.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <regdef.h>

#include <pic32m.h>
#include <pic_hardware.h>

void putChar( char c )
{
  while( ::pic::stdoutUart.txBufFull() ); // wait for TX buffer to be empty
  ::pic::stdoutUart.send( c );
}

extern "C"
{
  int __attribute__((weak)) read( int, char *, int ){  return -1;  }
  int __attribute__((weak)) open( const char*, int ){  return -1;  }
  int __attribute__((weak)) close( int ){  return -1;  }
  int __attribute__((weak)) lseek( int, int, int ){  return -1;  }
  int __attribute__((weak)) fstat( int, void * ){  return -1;  }
  int __attribute__((weak)) isatty( int ){  return 0;  }

  int __attribute__((weak)) getpid(){  return 1;  }
  int __attribute__((weak)) kill( int, int ){  return -1;  }
  void __attribute__((weak))  _exit(int){  puts("_exit()"); for(;;);  }
  void __attribute__((weak))  abort(){  puts("abort()"); for(;;);  }

  int write( int, char *buf, int count )
  {
    for( int i=0; i<count; ++i )
      putChar( *buf++ );
    return count;
  }

  void *sbrk( int incr )
  {
    extern char end;
    static char *heap_end=0;

    if( !heap_end )
      heap_end = &end;

    char *prev_heap_end = heap_end;
    heap_end += incr;

    return (void *)prev_heap_end;
  }

  void _on_bootstrap()
  {
    ::pic::stdoutUart.setHighSpeed( true );
    ::pic::stdoutUart.setBaudRate( 1000000 );
    ::pic::stdoutUart.intDisable();
    ::pic::stdoutUart.intClrFlag();
    ::pic::stdoutUart.start();
    DelayMs(100);
    puts("on_bootstrap");
  }

}
