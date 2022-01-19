#include "hardware.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <pic32m.h>
#include <pic_hardware.h>


pic::PinOut PinLED( LED_PORT_AND_PIN );


void putChar( char c );

volatile unsigned timer1_counter=0;
extern "C"
{
  __ISRshadow(_TIMER_1_VECTOR)
  {
    ::pic::Timer1.intClrFlag();
    PinLED.inv();
    ++timer1_counter;
  }
}


void putS( const char *s )
{
  if( s )
  while( char c = *(s++) )
    putChar( c );
}


int main()
{
  ::pic::configureSystemMultVector();
  ::pic::enableInterrupts();

  DelayMs(50);
  puts( "main()" );
  {
    ::pic::Timer1.setPrescale256(); // 80Mhz/256 = 312500 Hz
    ::pic::Timer1 = 0;
    ::pic::Timer1.setPeriod( 62500/2 ); // .2s interval interrupt
    ::pic::Timer1.start();

    ::pic::Timer1.setIntPri(7); // SRS!
    ::pic::Timer1.setIntSubPri(0);
    ::pic::Timer1.intEnable();
  }
  DelayMs(50);

  for(size_t n=0;n<20;++n)
  {
    printf("tick %10zu %10u cnt=%u\n", n, (unsigned)::pic::Timer1(), timer1_counter );
    DelayMs(50);
  }
  if( timer1_counter < 5 )
    puts("ERR");
  else
    puts("OK");
  for(;;);
  return 0;
}
