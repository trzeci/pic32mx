#include "hardware.h"

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <pic_hardware.h>

void putChar( char c );

void putS( const char *s )
{
  if( s )
  while( char c = *(s++) )
    putChar( c );
}

void putHexChar( uint8_t v )
{
  if( v < 10 )
    putChar( '0'+v );
  else
    putChar( 'a'+v-10 );
}

void putHex( uint8_t v )
{
  putHexChar( (uint8_t)(v>>4) );
  putHexChar( (uint8_t)(v&0xFU) );
}

void putHex( uint16_t v )
{
  putHex( (uint8_t)(v>>8) );
  putHex( (uint8_t)v );
}

void putHex( uint32_t v )
{
  putHex( (uint16_t)(v>>16) );
  putHex( (uint16_t)v );
}


extern "C" void __attribute__((interrupt,nomicromips,nomips16,noreturn,aligned(16))) _general_exception_handler(void)
{
  uint32_t _c0r12;
  uint32_t _excep_cause;
  uint32_t _excep_addr;

  asm volatile("mfc0 %0,$12" : "=r" (_c0r12));
  asm volatile("mfc0 %0,$13" : "=r" (_excep_cause));
  asm volatile("mfc0 %0,$14" : "=r" (_excep_addr));

  putChar('\n');
  putS( "GEH CODE " ); putHex( (uint8_t)(((_excep_cause>> 2) & 0x1FU)) ); putChar('\n');
  putS( "GEH RIPL " ); putHex( (uint8_t)(((_excep_cause>>10) & 0xFFU)) ); putChar('\n');
  putS( "      @" ); putHex( _excep_addr ); putChar('\n');
  putS( "STATUS " ); putHex( _c0r12 );      putChar('\n');
  while(1);
}


int main()
{
  puts( "main()" );
  void (*f)() = ( void (*)() )0xDEADBEEF;
  f();
  for(;;);
  return 0;
}
