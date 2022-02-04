#define UART_NUMBER 2
#define LED_PORT E
#define LED_PIN  3
#define TE_PORT  B
#define TE_PIN  13

#define VERSION 1

#include <ctype.h>
#include <pic32m.h>
#include <pic_hardware.h>
#include "nvmem.h"
#include "crc16.h"

#if 0 // A18

  #define LED_PORT_AND_PIN pic::PortD,13
  #define Uart Uart1

#elif 1 // MAIN

  #define LED_PORT_AND_PIN pic::PortE, 3
  #define TE_PORT_AND_PIN  pic::PortB,13
  #define Uart Uart2

#endif


#if __PIC32MZ__
  #define APP_FLASH_BASE_ADDRESS  __KSEG0_PROGRAM_MEM_BASE
  #define APP_FLASH_LENGTH        __KSEG0_PROGRAM_MEM_LENGTH
  #define USER_APP_RESET_ADDRESS  APP_FLASH_BASE_ADDRESS
#elif __PIC32MX__
  #define APP_FLASH_BASE_ADDRESS  __KSEG0_PROGRAM_MEM_BASE
  #define APP_FLASH_LENGTH        __KSEG0_PROGRAM_MEM_LENGTH
  #define USER_APP_RESET_ADDRESS  APP_FLASH_BASE_ADDRESS
#endif


#ifdef LED_PORT_AND_PIN
  static inline void LED( ::pic::Port& p, uint8_t pin )
  {
    p.setOut( pin );
    p.setPin( pin );
  }

  pic::PinOut PinLED( LED_PORT_AND_PIN );
  static inline void LED( bool v ) {  if( v ) PinLED.set();  else PinLED.clr();  }
  static inline void LED() {  PinLED.inv();  }
#else
  static inline void LED( bool ) {}
  static inline void LED() {}
#endif

#ifdef TE_PORT_AND_PIN
  static inline void TE( ::pic::Port& p, uint8_t pin )
  {
    p.setOut( pin );
    p.setPin( pin );
  }

  static inline void TE()
  {
    TE( TE_PORT_AND_PIN );
  }
#else
  static inline TE(){}
#endif


void uartSpeed( uint32_t v )
{
  ::pic::Uart.setBaudRate( v > 500000, v ); // hi spd over 500kbit
}

bool getC( uint8_t *c )
{
  if( ::pic::Uart.anyError() )
    ::pic::Uart.clearErrors();

  if( ::pic::Uart.receivedDataIsAvailable() )
  {
    *c = ::pic::Uart.getDataByte();
    return true;
  }
  else
  {
    return false;
  }
}

char getC()
{
  if( ::pic::Uart.anyError() )
    ::pic::Uart.clearErrors();
  while( !::pic::Uart.receivedDataIsAvailable() );
  return (char)::pic::Uart.getDataByte();
}

void putC( char c )
{
  while( ::pic::Uart.txBufFull() );
  ::pic::Uart.send( c );
}

void putS( const char *s )
{
  while( char c = *(s++) )
    putC( c );
  putC( '\n' );
}

extern "C" void _on_bootstrap()
{
  LED( LED_PORT_AND_PIN );
  uartSpeed( 115200 );
  ::pic::Uart.set8N2();
  ::pic::Uart.start();
}


void __attribute__((nomips16,nimicromips)) jumpToApp()
{
  LED(false);

  // reset IPL
  {
    asm volatile("di");
    asm volatile("ehb");
    uint32_t val;
    asm volatile("mfc0   %0,$12" : "=r"(val));
    val &= ~( 0x3FU << 10 );
    asm volatile("mtc0   %0,$12" : "+r"(val));
    asm volatile("ehb");
  }

  _bsc0( 12, 0, 0x3FU << 10 );

  void (*fptr)(void);
  fptr = (void (*)(void))USER_APP_RESET_ADDRESS;
  fptr();
}


uint32_t read32()
{
  union
  {
    char b[4];
    uint32_t w;
  } v;
  for( uint8_t n=0; n<4; ++n )
    v.b[n] = getC();
  return v.w;
}


char page_buffer[::pic::Flash::pageSize];

int main()
{
  TE();
  LED(true);
  uint8_t c=0;
//  for(;;)
//  {
//    c = getC();
//    putC(c);
//  }
  for( size_t n=0; n<5; ++n )
  {
    putC('$');
    DelayMs(100);
    if( getC(&c) )
    {
      if( islower(c) )
        c = toupper(c);
      if( c == 'L' ) // jump to loader
        break;
      else
      if( c == 'J' ) // jump to app
        jumpToApp();
    }
  }
  // timeout -> jump to app
  if( c != 'L' )
  {
    putC('J');
    jumpToApp();
  }
  putC('L');
  for(;;)
  {
    putC('$');
    c = getC();
    LED();
    if( islower(c) )
      c = toupper(c);
    switch( c )
    {
      case 'V': {
                  putC( (char)VERSION );
                  putC( char(uint8_t(__builtin_ffs(::pic::Flash::pageSize)-1)) );
                }
                break; // info: version, log2(page size)
      case 'S': {
                   // change speed, 115200, 500000, 1000000 or any
                  uint32_t v = read32();
                  uartSpeed( v );
                  DelayMs(100);
                }
                break;
      case 'P': {
                  // program page
                  uint32_t a = read32();
                  NVMemStartErasePage((void *)a); // start erasing the page
                  if( NVMemWaitOperation() )  // wait page erased successfully
                  {
                    putC('!');
                    break;
                  }
                  putC('A');
                  for( size_t n=0; n<::pic::Flash::pageSize; ++n )
                    page_buffer[n] = getC();
                  {
                    auto ProgAddress = (size_t)PA_TO_KVA0((size_t)a);
                    if( APP_FLASH_BASE_ADDRESS <= ProgAddress && ProgAddress+::pic::Flash::pageSize <= APP_FLASH_BASE_ADDRESS+APP_FLASH_LENGTH )
                    {
                      char *src_addr = page_buffer;
                      for( uint16_t i=0; i< ::pic::Flash::pageSize/::pic::Flash::rowSize; ++i )
                      {
                        // Make sure we are not writing boot area and device configuration bits.
                        if( APP_FLASH_BASE_ADDRESS <= ProgAddress && ProgAddress < APP_FLASH_BASE_ADDRESS+APP_FLASH_LENGTH )
                          NVMemWriteRow( (void *)ProgAddress, src_addr );
                        ProgAddress += ::pic::Flash::rowSize;
                        src_addr += ::pic::Flash::rowSize;
                      }
                      uint16_t crc = crc16( (uint8_t *)PA_TO_KVA0(a), ::pic::Flash::pageSize );
                      putC('C');
                      putC( uint8_t( crc & 0xFFU ) );
                      putC( uint8_t( crc >> 8 ) );
                    }
                    else
                    {
                      putC('!');
                    }
                  }
                }
                break;
      default : putC('?');
    }
  }
  return 0;
}
