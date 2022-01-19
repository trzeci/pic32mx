#include "hardware.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <pic_hardware.h>

pic::PinOut PinLED( LED_PORT_AND_PIN );

void putChar( char c )
{
  while( ::pic::stdoutUart.txBufFull() ); // wait for TX buffer to be empty
  ::pic::stdoutUart.send( c );
}

void puts( const char *s )
{
  while( char c = *(s++) )
    putChar( c );
  putChar( '\r' );
  putChar( '\n' );
}

extern "C" void _on_bootstrap()
{
  ::pic::stdoutUart.setHighSpeed( true );
  ::pic::stdoutUart.setBaudRate( 1000000 );
  ::pic::stdoutUart.start();
  DelayMs(20);
  puts("on_bootstrap");
}

int main()
{
  puts("main()");
  for(unsigned n=0;n<10;++n)
  {
    DelayMs(500);
    PinLED.inv();
    puts("blink");
  }
  puts("OK");
  for(;;);
  return 0;
}
