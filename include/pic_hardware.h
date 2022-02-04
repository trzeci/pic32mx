// author: Copyright (C) 2022 Pawel Susicki
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef __PIC_HARDWARE_H__
#define __PIC_HARDWARE_H__

#if !__ASSEMBLER__
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#endif

#if __cplusplus
#include <cstdlib>
#endif

#if defined(__XC32_VERSION__) // if library is used with xc32 toolchain
  #include <xc.h>
  #include <cp0defs.h>
  #include <sys/attribs.h>
#else
  #include <pic32m.h> // gcc toolchain otherwise
#endif

#ifndef Fcy
#error "define Fcy system clock"
#endif
#ifndef Fpb
#error "define Fpb hardware clock"
#endif

#define Fct ((Fcy)/2)


// core timer
#define ticks_per_us ((Fct)/1000000UL)
#define ticks_per_ms ((Fct)/1000UL)

#define ns_per_tick ((1000000000UL+Fct/2)/Fct) //  for _mfc0 (_CP0_COUNT, _CP0_COUNT_SELECT) register from cp0defs.h, 25ns for 80MHz cpu clk, eventually rounded up


#if __cplusplus

inline uint32_t NsToCoreTimerTicks32( uint32_t ns )
{
  return ( ns + (ns_per_tick+1)/2 )/ ns_per_tick;
}

inline uint32_t NsToCoreTimerTicks( uint32_t ns )
{
  return NsToCoreTimerTicks32( ns );
}

inline uint64_t NsToCoreTimerTicks64( uint64_t ns )
{
  return ( ns + (ns_per_tick+1)/2 )/ ns_per_tick;
}


#if __PIC32MZ__
inline uint32_t __attribute__((nomips16)) _ReadCoreTimer()
{
  uint32_t v;
  asm volatile( "mfc0 %0, $9" : "=r"(v) );
  return v;
}
#else
uint32_t __attribute__((nomips16)) _ReadCoreTimer();
#endif

inline void DelayNs( uint32_t ns )
{
  int32_t ticks = NsToCoreTimerTicks( ns );
  volatile int32_t when = (int32_t)_ReadCoreTimer() + ticks;
  while( (int32_t)_ReadCoreTimer() - when < 0 );
}

inline void DelayUs( uint32_t us )
{
  int32_t ticks = us * ticks_per_us;
  volatile int32_t when = (int32_t)_ReadCoreTimer() + ticks;
  while( (int32_t)_ReadCoreTimer() - when < 0 );
}

inline void DelayMs( uint32_t ms )
{
  int32_t ticks = ms * ticks_per_ms;
  volatile int32_t when = (int32_t)_ReadCoreTimer() + ticks;
  while( (int32_t)_ReadCoreTimer() - when < 0 );
}


namespace pic
{

  void __attribute__((nomips16)) configureSystemMultVector();
  void __attribute__((nomips16)) cacheFlushData( const void *_addr, size_t n );
  void __attribute__((nomips16)) cacheInvalidateData( const void *_addr, size_t n );

  inline bool __attribute__((nomips16)) enableInterrupts()
  {
    uint32_t status = 0;
    asm volatile("ei    %0" : "=r"(status));
    return !!(status&1);
  }

  inline bool __attribute__((nomips16)) disableInterrupts()
  {
    uint32_t status = 0;
    asm volatile("di %0" : "=r"(status));
    asm volatile( "ehb" );
    return !!(status&1);
  }

  inline void __attribute__((nomips16))  restoreInterrupts( bool status )
  {
    if( status )
    {
      asm volatile("ei");
    }
    else
    {
      asm volatile("di");
      asm volatile( "ehb" );
    }
  }


  struct InterruptMaskI2C
  {
    volatile uint32_t *ifs, *iec;
    uint32_t ie_mask[3];

    typedef enum { evMaster=0,evSlave=0,evCollision=2 } Event;

    bool enabled( Event ev ) {  return !!(iec[0]&ie_mask[ev]);  }
    bool flag   ( Event ev ) {  return !!(ifs[0]&ie_mask[ev]);  }
    void clrFlag( Event ev ) {  ifs[1] = ie_mask[ev];  }

    bool enable( Event ev )
    {
      bool rv = enabled(ev);
      iec[2] = ie_mask[ev];  // SET
      return rv;
    }

    bool disable( Event ev )
    {
      bool rv = enabled(ev);
      iec[1] = ie_mask[ev];  // CLR
      return rv;
    }

    bool enable( Event ev, bool on )
    {
      bool rv = enabled(ev);
      if( on )
        enable(ev);
      else
        disable(ev);
      return rv;
    }
  };

  struct InterruptMaskComm
  {
    volatile uint32_t *ifs, *iec;
    uint32_t ie_mask[3];

    typedef enum { Rx=0,Tx=1,Er=2 } Event;

    bool enabled( Event ev ) {  return !!(iec[0]&ie_mask[ev]);  }
    bool flag   ( Event ev ) {  return !!(ifs[0]&ie_mask[ev]);  }
    void clrFlag( Event ev ) {  ifs[1] = ie_mask[ev];  }

    bool enable( Event ev )
    {
      bool rv = enabled(ev);
      iec[2] = ie_mask[ev];  // SET
      return rv;
    }

    bool disable( Event ev )
    {
      bool rv = enabled(ev);
      iec[1] = ie_mask[ev];  // CLR
      return rv;
    }

    bool enable( Event ev, bool on )
    {
      bool rv = enabled(ev);
      if( on )
        enable(ev);
      else
        disable(ev);
      return rv;
    }
  };

  struct InterruptMask
  {
    volatile uint32_t *ifs, *iec;
    uint32_t ie_mask;

    bool enabled() {  return !!(iec[0]&ie_mask);  }
    bool flag()    {  return !!(ifs[0]&ie_mask);  }
    void clrFlag() {  ifs[1] = ie_mask;  }
    void setFlag() {  ifs[2] = ie_mask;  }

    bool enable()
    {
      bool rv = enabled();
      iec[2] = ie_mask;  // SET
      return rv;
    }

    bool disable()
    {
      bool rv = enabled();
      iec[1] = ie_mask;  // CLR
      return rv;
    }

    bool enable( bool on )
    {
      bool rv = enabled();
      if( on )
        enable();
      else
        disable();
      return rv;
    }
  };

  struct InterruptVectorComm
  {
    volatile uint32_t *ipc[3]; uint8_t sub_shift[3], pri_shift[3];

    typedef enum { Rx=0,Tx=1,Er=2 } Event;

    uint8_t getPri( Event ev )    {  return (ipc[ev][0]>>pri_shift[ev]) & 7;  }
    uint8_t getSubPri( Event ev ) {  return (ipc[ev][0]>>sub_shift[ev]) & 3;  }

    void setPri( Event ev, uint8_t v )
    {
      ipc[ev][1] = ( 7 << pri_shift[ev] );
      ipc[ev][2] = ((uint32_t)v) << pri_shift[ev];
    }

    void setSubPri( Event ev, uint8_t v )
    {
      ipc[ev][1] = ( 3 << sub_shift[ev] );
      ipc[ev][2] = ((uint32_t)v) << sub_shift[ev];
    }
  };


  struct InterruptVector
  {
    volatile uint32_t *ipc; uint8_t sub_shift, pri_shift;

    uint8_t getPri()    {  return (ipc[0]>>pri_shift) & 7;  }
    uint8_t getSubPri() {  return (ipc[0]>>sub_shift) & 3;  }

    void setPri( uint8_t v )
    {
      ipc[1] = ( 7 << pri_shift );
      ipc[2] = ((uint32_t)v) << pri_shift;
    }

    void setSubPri( uint8_t v )
    {
      ipc[1] = ( 3 << sub_shift );
      ipc[2] = ((uint32_t)v) << sub_shift;
    }
  };


  struct SoftwareInterrupt
  {
    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    uint8_t N;
    uint8_t no() const {  return N;  }

    SoftwareInterrupt( uint8_t n ) : N(n) {}

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intSetFlag() {  Ien[no()].setFlag();  } // method of invoking of interrupt
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }
  };
  extern SoftwareInterrupt softwareInterrupt0,softwareInterrupt1;


  struct Port;

  #ifdef _PORTA
  extern Port PortA;
  #endif
  #ifdef _PORTB
  extern Port PortB;
  #endif
  #ifdef _PORTC
  extern Port PortC;
  #endif
  #ifdef _PORTD
  extern Port PortD;
  #endif
  #ifdef _PORTE
  extern Port PortE;
  #endif
  #ifdef _PORTF
  extern Port PortF;
  #endif
  #ifdef _PORTG
  extern Port PortG;
  #endif

  struct Register
  {
    volatile uint32_t _val;
    volatile uint32_t _clr;
    volatile uint32_t _set;
    volatile uint32_t _inv;

    uint32_t operator = ( uint32_t v ) {  _val = v;  return v;  }
    uint32_t operator () () {  return _val;  }
    void set( uint32_t v ) {  _set = v;  }
    void clr( uint32_t v ) {  _clr = v;  }
    void inv( uint32_t v ) {  _inv = v;  }

    void setPin( uint8_t n ) {  set( 1U<<n );  }
    void clrPin( uint8_t n ) {  clr( 1U<<n );  }
    void invPin( uint8_t n ) {  inv( 1U<<n );  }
  };


  struct Port
  {
#if __PIC32MZ__
    uint8_t no() const // 0..
    {
#if __PIC32MX__
      return ((size_t)this - (size_t)&PortA) / 0x40;
#elif __PIC32MZ__
      return ((size_t)this - (size_t)&PortA) / 0x100;
#endif
    }
#endif

    Port() = delete;
    Port( const Port& ) = delete;
    Port( void * ){} // special for fake ports

#if __PIC32MZ__
    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }
#endif

#if __PIC32MZ__
    Register ANSEL;  // 0=digital, 1=analog, 1 after reset, 0 after btldr
#endif
    Register TRIS;   // 0=output, 1=input, default=input
    Register PORT;
    Register LAT;
    Register ODC;
#if __PIC32MZ__
    Register PullUp;
    Register PullDown;

    volatile union CNCON
    {
      struct
      {
        uint32_t :11;
        uint32_t EDGEDETECT:1;
        uint32_t :3;
        uint32_t ON:1;
      };
      uint32_t w;
    } cn, cnCLR, cnSET, cnINV;
    Register cnEnable,cnStat,cnNeg,cnF;

    void CNstart()   {  cn.ON=1;  }
    void CNsetEdgeDetect() {  cn.EDGEDETECT=1;  }

    void CNenable ( uint32_t v ) {  cnEnable.set( v );  } // raising if edgedetect
    void CNdisable( uint32_t v ) {  cnEnable.clr( v );  }

    void CNenableRaising( uint32_t v ) {  cnEnable.set( v );  }
    void CNenableFalling( uint32_t v ) {  cnNeg   .set( v );  }

#endif

    uint32_t operator = ( uint32_t v ) {  LAT = v;  return v;  }
    uint32_t operator () () {  return PORT();  }

    void set( uint32_t v ) {  LAT.set( v );  }
    void clr( uint32_t v ) {  LAT.clr( v );  }
    void inv( uint32_t v ) {  LAT.inv( v );  }

    void setPin( uint8_t n ) {  LAT.set( 1U<<n );  }
    void clrPin( uint8_t n ) {  LAT.clr( 1U<<n );  }
    void invPin( uint8_t n ) {  LAT.inv( 1U<<n );  }

    void setInOut( uint32_t v ) {  TRIS = v;  }
    void setIn   ( uint32_t v ) {  TRIS.set( v );  }
    void setOut  ( uint32_t v ) {  TRIS.clr( v );  }

    void setPinIn   ( uint8_t n ) {  TRIS.set( 1U<<n );  }
    void setPinOut  ( uint8_t n ) {  TRIS.clr( 1U<<n );  }

#if __PIC32MZ__
    void setDigital( uint32_t v ) {  ANSEL.clr(v);  }
    void setAnalog ( uint32_t v ) {  ANSEL.set(v);  setIn(v);  }
#elif __PIC32MX__
    inline void setDigital( uint32_t v );
    inline void setAnalog ( uint32_t v );
#endif
  };

  struct Pin
  {
    Port &port;
    uint32_t mask;
    uint8_t bit;
    Pin( Port &_p, uint8_t _b ) : port(_p), mask(1U<<_b), bit(_b) {  setDigital();  }

    void setIn () {  port.setIn (mask);  }
    void setOut() {  port.setOut(mask);  }

    bool operator = ( bool v ) {  if(v) set(); else clr();  return v;  }
    bool operator () () {  return !!(port()&mask);  }

    void set() {  port.set( mask );  }
    void clr() {  port.clr( mask );  }
    void inv() {  port.inv( mask );  }

    void setDigital() {  port.setDigital(mask);  }
    void setAnalog () {  port.setAnalog (mask);  }
#if __PIC32MZ__
    void CNenable () {  port.CNenable (mask);  } // raising if edgedetect
    void CNdisable() {  port.CNdisable(mask);  }

    void pullUp  () {  port.PullUp  .set(mask);  }
    void pullDown() {  port.PullDown.set(mask);  }

    void CNenableRaising() {  port.CNenableRaising(mask);  }
    void CNenableFalling() {  port.CNenableFalling(mask);  }
#endif
  };

  struct PinIn : public Pin
  {
    PinIn ( Port &_p, uint8_t _b ) : Pin(_p,_b) {  setIn();   }
  };

  struct PinInv : public PinIn
  {
    PinInv( Port &_p, uint8_t _b ) : PinIn(_p,_b) {}
    bool operator () () {  return !PinIn::operator()();  }
  };

  struct PinOut : public Pin
  {
    PinOut( Port &_p, uint8_t _b ) : Pin(_p,_b) {  clr();  setOut();  }
    PinOut( Port &_p, uint8_t _b, bool b ) : Pin(_p,_b) {  if(b) set(); else clr();  setOut();  }
  };

  struct PinOutInv : public Pin   // active low, initilized to high
  {
    PinOutInv( Port &_p, uint8_t _b ) : Pin(_p,_b) {  clr();  setOut();  }

    bool operator = ( bool v ) {  if(v) set(); else clr();  return v;  }
    bool operator () () {  return !(port()&mask);  }

    void set() {  port.clr( mask );  }
    void clr() {  port.set( mask );  }
  };


  struct Spi;

  #ifdef _SPI1
  extern Spi Spi1;
  #endif
  #ifdef _SPI2
  extern Spi Spi2;
  #endif
  #ifdef _SPI3
  extern Spi Spi3;
  #endif
  #ifdef _SPI4
  extern Spi Spi4;
  #endif
  #ifdef _SPI5
  extern Spi Spi5;
  #endif
  #ifdef _SPI6
  extern Spi Spi6;
  #endif

  struct Spi
  {
    uint8_t no() const // 0..
    {
#if __PIC32MX__
      return ((size_t)this - (size_t)&Spi3) / 0x200;
#elif __PIC32MZ__
      return ((size_t)this - (size_t)&Spi1) / 0x200;
#endif
    }

    static InterruptMaskComm Ien[];
#if __PIC32MX__
    static InterruptVector IntVec[];
#elif __PIC32MZ__
    static InterruptVectorComm IntVec[];
#endif

    typedef InterruptMaskComm::Event Event;
#if __PIC32MZ__
    typedef InterruptVectorComm::Event EventV;
#endif

    bool intFlag( Event ev ) {  return Ien[no()].flag(ev);  }
    void intClrFlag( Event ev ) {  Ien[no()].clrFlag(ev);  }

    void intEnable ( Event ev ) {  Ien[no()].enable(ev);   }
    void intDisable( Event ev ) {  Ien[no()].disable(ev);  }

    void intClrFlag() {  Ien[no()].clrFlag(InterruptMaskComm::Er);  Ien[no()].clrFlag(InterruptMaskComm::Tx);  Ien[no()].clrFlag(InterruptMaskComm::Rx);  }
    void intEnable () {  Ien[no()].enable (InterruptMaskComm::Er);  Ien[no()].enable (InterruptMaskComm::Tx);  Ien[no()].enable (InterruptMaskComm::Rx);  }
    void intDisable() {  Ien[no()].disable(InterruptMaskComm::Er);  Ien[no()].disable(InterruptMaskComm::Tx);  Ien[no()].disable(InterruptMaskComm::Rx);  }


#if __PIC32MX__
    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }
#elif __PIC32MZ__
    void setIntPri   ( EventV ev, uint8_t v ) {  IntVec[no()].setPri(ev, v);  }
    void setIntSubPri( EventV ev, uint8_t v ) {  IntVec[no()].setSubPri(ev, v);  }
#endif


    volatile union CON
    {
      struct {
        uint32_t SRXISEL:2;
        uint32_t STXISEL:2;
        uint32_t DISSDI:1;
        uint32_t MSTEN:1;
        uint32_t CKP:1;
        uint32_t SSEN:1;
        uint32_t CKE:1;
        uint32_t SMP:1;
        uint32_t MODE:2;
        uint32_t DISSDO:1;
        uint32_t :2;
        uint32_t ON:1;
        uint32_t ENHBUF:1;
      };
      uint32_t w;
    } con, conCLR, conSET, conINV;

    volatile union STAT
    {
      struct {
        uint32_t SPIRBF:1; // rx buf full
        uint32_t SPITBF:1; // tx buf full
        uint32_t :1;
        uint32_t SPITBE:1; // tx buf empty
        uint32_t :1;
        uint32_t SPIRBE:1; // rx buf empty only if ENHBUF=1
        uint32_t SPIROV:1; // rx overflow
        uint32_t SRMT:1;   // shift tx empty
        uint32_t :3;
        uint32_t SPIBUSY:1; // !idle
        uint32_t :4;
        uint32_t TXBUFELM:5; // only if ENHBUF=1
        uint32_t :3;
        uint32_t RXBUFELM:5; // only if ENHBUF=1
      };
      uint32_t w;
    } stat, statCLR, statSET, statINV;

    volatile uint32_t buf;
    volatile uint32_t dummy4[3];
    volatile uint32_t brg;        // 13 bits
    volatile uint32_t dummy5[3];

    void set8bit()  {  con.MODE=0b00;  }
    void set16bit() {  con.MODE=0b01;  }
    void set32bit() {  con.MODE=0b10;  }

    void disableSDI() {  con.DISSDI=1;  }
    void disableSDO() {  con.DISSDO=1;  }

    void setClockPolarityPositive() {  con.CKP=0;  }  // default
    void setClockPolarityNegative() {  con.CKP=1;  }

    void setSDOonEdgeActive()  {  con.CKE=0;  }  // default
    void setSDOonEdgeIdle()    {  con.CKE=1;  }

    void setSDIinMidle()  {  con.SMP=0;  }  // default
    void setSDIatEnd()    {  con.SMP=1;  }

    void setMaster()  {  con.MSTEN=1;  }
    void setSlave()   {  con.MSTEN=0;  }  // default

    void setFifo()    {  con.ENHBUF=1;  }
    void setNoFifo()  {  con.ENHBUF=0;  } // default

    void setTxIntFifoNotFull  (){  con.STXISEL=3;  }
    void setTxIntFifoHalf     (){  con.STXISEL=2;  }
    void setTxIntFifoEmpty    (){  con.STXISEL=1;  }
    void setTxIntShiftRegEmpty(){  con.STXISEL=0;  } // default !!!

    bool overflow() // also clears the flag
    {
      return stat.SPIROV;
    }

    void clearOverflowFlag()
    {
      union STAT s;
      s.w=0;
      s.SPIROV=1;
      statCLR.w = s.w;
    }

    void setBaudRate( uint32_t baudrate )
    {
      brg = Fpb/(baudrate*2)-1;
    }

    void reset()
    {
      con.w = 0;
    }

    void start()
    {
      con.ON = 1;
    }

    void stop()
    {
      con.ON = 0;
      __asm volatile ( "nop" );
    }

    bool txShiftRegEmpty() {  return stat.SRMT;  }
    bool txFull() {  return con.ENHBUF ? stat.SPITBF : !stat.SPITBE;  }
    bool rxEmpty() {  return con.ENHBUF ? stat.SPIRBE : !stat.SPIRBF;  }
    bool busy() {  return stat.SPIBUSY;  }

    void waitBusy() {  while(busy());  }
    void waitData() {  while(rxEmpty());  }

    struct Config
    {
      uint32_t con=0,brg=0;
    };

    Config getConfig()
    {
      Config cfg;
      cfg.con = con.w;
      cfg.brg = brg;
      return cfg;
    }

    Config setConfig( Config ncfg ) // returns old config
    {
      Config ocfg;
      ocfg.con = con.w;
      ocfg.brg = brg;
      con.w = ncfg.con;
      brg = ncfg.brg;
      return ocfg;
    }

    uint32_t operator()() volatile {  return buf;  }
    void operator = ( uint32_t c ) volatile {  buf = c;  }

    volatile uint32_t *bufferAddress(){  return &buf;  }

    uint32_t read()
    {
      while( rxEmpty() );
      return operator()();
    }

    void write( uint32_t v )
    {
      while( txFull() );
      operator = (v);
    }
  };


  struct Uart;
  template < Uart& > inline uint8_t uartNo();

  #ifdef _UART1
  extern Uart Uart1;
  #endif
  #ifdef _UART2
  extern Uart Uart2;
  #endif
  #ifdef _UART3
  extern Uart Uart3;
  #endif
  #ifdef _UART4
  extern Uart Uart4;
  #endif
  #ifdef _UART5
  extern Uart Uart5;
  #endif
  #ifdef _UART6
  extern Uart Uart6;
  #endif

  struct Uart
  {
    uint8_t no() const // 0..
    {
      return ((size_t)this - (size_t)&Uart1) / 0x200;
    }

    static InterruptMaskComm Ien[];
#if __PIC32MX__
    static InterruptVector IntVec[];
#elif __PIC32MZ__
    static InterruptVectorComm IntVec[];
#endif

    typedef InterruptMaskComm::Event Event;
    typedef InterruptVectorComm::Event EventV;

    bool intFlag( Event ev ) {  return Ien[no()].flag(ev);  }
    void intClrFlag( Event ev ) {  Ien[no()].clrFlag(ev);  }

    void intEnable ( Event ev ) {  Ien[no()].enable(ev);   }
    void intDisable( Event ev ) {  Ien[no()].disable(ev);  }

    void intClrFlag() {  Ien[no()].clrFlag(InterruptMaskComm::Er);  Ien[no()].clrFlag(InterruptMaskComm::Tx);  Ien[no()].clrFlag(InterruptMaskComm::Rx);  }
    void intEnable () {  Ien[no()].enable (InterruptMaskComm::Er);  Ien[no()].enable (InterruptMaskComm::Tx);  Ien[no()].enable (InterruptMaskComm::Rx);  }
    void intDisable() {  Ien[no()].disable(InterruptMaskComm::Er);  Ien[no()].disable(InterruptMaskComm::Tx);  Ien[no()].disable(InterruptMaskComm::Rx);  }

#if __PIC32MX__
    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }
#elif __PIC32MZ__
    void setIntPri   ( EventV ev, uint8_t v ) {  IntVec[no()].setPri(ev, v);  }
    void setIntSubPri( EventV ev, uint8_t v ) {  IntVec[no()].setSubPri(ev, v);  }
#endif

    volatile union MODE
    {
      struct
      {
        unsigned STSEL:1;  // 0=1stop, 1=2stop
        unsigned PDSEL:2;  // 0=8N1, 1=8E1, 2=8O1, 3=9N1
        unsigned BRGH:1;   // 0=1/16, 1=1/4 Fpb
        unsigned RXINV:1;  // inv Rx
        unsigned ABAUD:1;  // auto baud on next 0x55
        unsigned :1;
        unsigned :1;
        unsigned :2;
        unsigned :1;
        unsigned :1;
        unsigned :1;
        unsigned :1;
        unsigned :1;
        unsigned ON:1;
      };
      uint32_t w;
    } mode, modeCLR, modeSET, modeINV;

    volatile union STA
    {
      struct
      {
        unsigned URXDA:1;    // RxBuf not empty
        unsigned OERR:1;     // must be cleared to 0
        unsigned FERR:1;
        unsigned PERR:1;
        unsigned :1;
        unsigned :1;
        unsigned URXISEL:2;  // interrupt: 0,1-character received, 2-RxBuf 3/4 full (3 chars), 3-RxBuf full (4 chars)
        unsigned TRMT:1;     // Transmit shit reg empty and TxBuf empty
        unsigned UTXBF:1;    // TxBuf full
        unsigned UTXEN:1;    // transmit enable
        unsigned UTXBRK:1;   // transmit break
        unsigned URXEN:1;    // receive enable
        unsigned UTXINV:1;   // inv Tx
        unsigned UTXISEL:2;  // interrupt: 0-TxBuf not full, 1-everything transmitted, 2-TxBuf empty
        unsigned :8;
        unsigned :1;
      };
      uint32_t w;
    }sta, staCLR, staSET, staINV;

    volatile uint32_t txreg;
    volatile uint32_t dummy3[3];

    volatile uint32_t rxreg;
    volatile uint32_t dummy4[3];

    volatile uint32_t ubrg;
    volatile uint32_t dummy5[3];

    void setRxInterruptRx1Char  () {  sta.URXISEL = 0;  }
    void setRxInterruptRx3Chars () {  sta.URXISEL = 2;  }
    void setRxInterruptRxBufFull() {  sta.URXISEL = 3;  }

    void setTxInterruptTxBufNotFull() {  sta.UTXISEL = 0;  }
    void setTxInterruptTxIdle      () {  sta.UTXISEL = 1;  }
    void setTxInterruptTxBufEmpty  () {  sta.UTXISEL = 2;  }

    void set1StopBit()  {  mode.STSEL = 0;  }
    void set2StopBits() {  mode.STSEL = 1;  }

    void set8Nx() {  mode.PDSEL = 0;  }
    void set8Ex() {  mode.PDSEL = 1;  }
    void set8Ox() {  mode.PDSEL = 2;  }
    void set9Nx() {  mode.PDSEL = 3;  }

    void set8N1()
    {
      set8Nx();
      set1StopBit();
    }

    void set8N2()
    {
      set8Nx();
      set2StopBits();
    }

    void setHighSpeed( bool high )
    {
      mode.BRGH = !!high;
    }

    void setBaudRate( uint32_t baudrate )
    {
      ubrg = mode.BRGH ? Fpb/(4*baudrate)-1 : Fpb/(16*baudrate)-1;
    }

    void setBaudRate( bool hiSpd, uint32_t baudrate )
    {
      setHighSpeed( hiSpd );
      ubrg = hiSpd ? Fpb/(4*baudrate)-1 : Fpb/(16*baudrate)-1;
    }

    void start()
    {
      sta.UTXEN = 1;
      sta.URXEN = 1;
      mode.ON = 1;
    }

    void startTx()
    {
      sta.UTXEN = 1;
      mode.ON = 1;
    }

    void stop()
    {
      mode.ON = 0;
      __asm volatile ( "nop" );
    }

    bool anyError()
    {
      return sta.FERR || sta.OERR || sta.PERR;
    }

    bool frameError()
    {
      return sta.FERR;
    }

    bool overrunError()
    {
      return sta.OERR;
    }

    bool parityError()
    {
      return sta.PERR;
    }

    void clearErrors()
    {
      getDataByte();  // dummy read to clear ERR
      sta.OERR = 0;
    }

    bool receivedDataIsAvailable()
    {
      return !!sta.URXDA;
    }

    char getDataByte() volatile
    {
      return rxreg;
    }

    bool txBufFull()
    {
      return !!sta.UTXBF;
    }

    bool txBusy()
    {
      return !sta.TRMT;
    }

    void sendDataByte( char c )
    {
      txreg = (uint8_t)c;
    }

    void send( char c )
    {
      sendDataByte( c );
    }

    void clearOverrunError()
    {
      if( sta.OERR )
      {
        //sta.OERR=0;
        //staCLR=2;
        union STA u;
        u.w = 0;
        u.OERR = 1;
        staCLR.w = u.w;
      }
    }
  };

  template<> inline uint8_t uartNo<Uart1>() {  return 0;  }
  template<> inline uint8_t uartNo<Uart2>() {  return 1;  }
  template<> inline uint8_t uartNo<Uart3>() {  return 2;  }
  template<> inline uint8_t uartNo<Uart4>() {  return 3;  }
  template<> inline uint8_t uartNo<Uart5>() {  return 4;  }
  template<> inline uint8_t uartNo<Uart6>() {  return 5;  }


  struct TimerA;
  struct TimerB;
  struct Timer32;

  #ifdef _TMR1
  extern TimerA  Timer1;
  #endif
  #ifdef _TMR2
  extern TimerB  Timer2;
  #endif
  #ifdef _TMR3
  extern TimerB  Timer3;
  #endif
  #ifdef _TMR4
  extern TimerB  Timer4;
  #endif
  #ifdef _TMR5
  extern TimerB  Timer5;
  #endif
  #ifdef _TMR6
  extern TimerB  Timer6;
  #endif
  #ifdef _TMR7
  extern TimerB  Timer7;
  #endif
  #ifdef _TMR8
  extern TimerB  Timer8;
  #endif
  #ifdef _TMR9
  extern TimerB  Timer9;
  #endif
  #ifdef _TMR23
  extern Timer32 Timer23;
  #endif
  #ifdef _TMR45
  extern Timer32 Timer45;
  #endif
  #ifdef _TMR67
  extern Timer32 Timer67;
  #endif
  #ifdef _TMR89
  extern Timer32 Timer89;
  #endif

  struct Timer
  {
    uint8_t no() const // 0..
    {
      return ((size_t)this - (size_t)&Timer1) / 0x200;
    }

    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }

    volatile union CON
    {
      struct {
        uint32_t :1;
        uint32_t TCS:1;   // clock 0=internal
        uint32_t :1;      // TSYNC
        uint32_t T32:1;   // tylko T2,T4,...
        uint32_t TCKPS:3; // /1, /8, /64, /256, albo /1, /2, /4, ... /256
        uint32_t TGATE:1;
        uint32_t :3;
        uint32_t :1; // TWIP
        uint32_t :1; // TWDIS
        uint32_t :1; // SIDL, stop in idle
        uint32_t :1;
        uint32_t ON:1;
      };
      uint32_t w;
    } con, conCLR, conSET, conINV;
    volatile uint32_t tmr, tmrCLR, tmrSET, tmrINV;
    volatile uint32_t pr, prCLR, prSET, prINV;

    void reset() {  con.w=0; tmr=0; pr=0;  }
    void setGated() {  con.TGATE=1;   }
    void setClockExternal() {  con.TCS=1;   }

    void setPeriod( uint16_t v ) {  pr = v;  }

    void operator = ( uint16_t v ) volatile {  tmr = v;  }
    uint16_t operator () () {  return tmr;  }

    void start() {  con.ON=1;  }
    void stop()  {  con.ON=0; __asm volatile ( "nop" );  }
  };


  struct TimerA : public Timer
  {
    void setPrescale1  () {  con.TCKPS = 0;  }
    void setPrescale8  () {  con.TCKPS = 1;  }
    void setPrescale64 () {  con.TCKPS = 2;  }
    void setPrescale256() {  con.TCKPS = 3;  }

    bool setPrescale( uint32_t prescale )
    {
      switch(prescale)
      {
        case   1: setPrescale1  (); break;
        case   8: setPrescale8  (); break;
        case  64: setPrescale64 (); break;
        case 256: setPrescale256(); break;
        default : return false;
      }
      return true;
    }

    void operator = ( uint16_t v ) volatile {  Timer::operator = (v);  }
  };

  struct TimerB : public Timer
  {
    void setPrescale1  () {  con.TCKPS = 0;  }
    void setPrescale2  () {  con.TCKPS = 1;  }
    void setPrescale4  () {  con.TCKPS = 2;  }
    void setPrescale8  () {  con.TCKPS = 3;  }
    void setPrescale16 () {  con.TCKPS = 4;  }
    void setPrescale32 () {  con.TCKPS = 5;  }
    void setPrescale64 () {  con.TCKPS = 6;  }
    void setPrescale256() {  con.TCKPS = 7;  }

    bool setPrescale( uint32_t prescale )
    {
      switch(prescale)
      {
        case   1: setPrescale1  (); break;
        case   2: setPrescale2  (); break;
        case   4: setPrescale4  (); break;
        case   8: setPrescale8  (); break;
        case  16: setPrescale16 (); break;
        case  32: setPrescale32 (); break;
        case  64: setPrescale64 (); break;
        case 256: setPrescale256(); break;
        default : return false;
      }
      return true;
    }

    void operator = ( uint16_t v ) {  Timer::operator = (v);  }
  };

  struct Timer32 : public TimerB
  {
    bool intFlag   () {  return Ien[no()+1].flag();  }
    void intClrFlag() {  Ien[no()+1].clrFlag();  }
    void intEnable () {  Ien[no()+1].enable();  }
    void intDisable() {  Ien[no()+1].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()+1].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()+1].setSubPri(v);  }

    void set32() {  con.T32=1;  }
    void setPeriod( uint32_t v ) {  pr = v;  }
    void operator = ( uint32_t v ) {  tmr = v;  }
    uint32_t operator () () volatile {  return tmr;  }
  };



  struct OutputCompare;

  #ifdef _OCMP1
  extern OutputCompare OC1;
  #endif
  #ifdef _OCMP2
  extern OutputCompare OC2;
  #endif
  #ifdef _OCMP3
  extern OutputCompare OC3;
  #endif
  #ifdef _OCMP4
  extern OutputCompare OC4;
  #endif
  #ifdef _OCMP5
  extern OutputCompare OC5;
  #endif
  #ifdef _OCMP6
  extern OutputCompare OC6;
  #endif
  #ifdef _OCMP7
  extern OutputCompare OC7;
  #endif
  #ifdef _OCMP8
  extern OutputCompare OC8;
  #endif
  #ifdef _OCMP9
  extern OutputCompare OC9;
  #endif

  struct OutputCompare
  {
    uint8_t no() const // 0..
    {
      return ((size_t)this - (size_t)&OC1) / 0x200;
    }

    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }

    volatile union CON
    {
      struct {
        uint32_t OCM:3;
        uint32_t OCTSEL:1;
        uint32_t :1; // OCFLT
        uint32_t OC32:1;
        uint32_t :7;
        uint32_t :1; // SIDL
        uint32_t :1;
        uint32_t ON:1;
      };
      struct {
        uint32_t w:32;
      };
    } con,conCLR,conSET,conINV;
    volatile uint32_t r,rCLR,rSET,rINV;
    volatile uint32_t rs,rsCLR,rsSET,rsINV;

    void set32() {  con.OC32=1; }
    void start() {  con.ON=1;   }
    void stop()  {  con.ON=0; __asm volatile ( "nop" );  }
    void setTimer2() {  con.OCTSEL=0;  }
    void setTimer3() {  con.OCTSEL=1;  }

    void modeDisable() {  con.OCM=0;  }
    void modeLowHigh() {  con.OCM=1;  }
    void modeHighLow() {  con.OCM=2;  }
    void modeToggle () {  con.OCM=3;  }
    void modePulse  () {  con.OCM=4;  }
    void modePulses () {  con.OCM=5;  }
    void modePWM    () {  con.OCM=6;  }

    void setR ( uint32_t v ) {  r=v;   }
    void setRS( uint32_t v ) {  rs=v;  }
    void setRS( uint32_t _r, uint32_t _rs ) {  r=_r; rs=_rs;  }
    void setPwmDutyCycle( uint32_t v ) {  setRS(v);  }
  };



  struct InputCapture;

  #ifdef _ICAP1
  extern InputCapture IC1;
  #endif
  #ifdef _ICAP2
  extern InputCapture IC2;
  #endif
  #ifdef _ICAP3
  extern InputCapture IC3;
  #endif
  #ifdef _ICAP4
  extern InputCapture IC4;
  #endif
  #ifdef _ICAP5
  extern InputCapture IC5;
  #endif
  #ifdef _ICAP6
  extern InputCapture IC6;
  #endif
  #ifdef _ICAP7
  extern InputCapture IC7;
  #endif
  #ifdef _ICAP8
  extern InputCapture IC8;
  #endif
  #ifdef _ICAP9
  extern InputCapture IC9;
  #endif

  struct InputCapture
  {
    uint8_t no() const // 0..
    {
      return ((size_t)this - (size_t)&IC1) / 0x200;
    }

    static InterruptMask Ien[];
    static InterruptMask IEen[];
    static InterruptVector IntVec[];

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    bool intEFlag   () {  return IEen[no()].flag();  }
    void intEClrFlag() {  IEen[no()].clrFlag();  }
    void intEEnable () {  IEen[no()].enable();  }
    void intEDisable() {  IEen[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }

    volatile union CON
    {
      struct {
        uint32_t ICM:3;
        uint32_t ICBNE:1;
        uint32_t ICOV:1;
        uint32_t :2;      // ICI=0 int on every capture event
        uint32_t ICTMR:1; // 0-Timer3, 1-Timer2
        uint32_t C32:1;
        uint32_t FEDGE:1;
        uint32_t :3;
        uint32_t :1; // SIDL
        uint32_t :1;
        uint32_t ON:1;
      };
      struct {
        uint32_t w:32;
      };
    } con,conCLR,conSET,conINV;
    volatile uint32_t buf;

    void set32bit() {  con.C32=1; }
    void start() {  con.ON=1;   }
    void stop()  {  con.ON=0; __asm volatile ( "nop" );  }

    void setTimer2() {  con.ICTMR=1;  }
    void setTimer3() {  con.ICTMR=0;  }

    bool overflow() {  return !!con.ICOV;  }
    bool captured() {  return !!con.ICBNE; }

    void modeDisable    () {  con.ICM=0;  }
    void modeEveryEdge  () {  con.ICM=1;  }
    void modeFallingEdge() {  con.ICM=2;  }
    void modeRisingEdge () {  con.ICM=3;  }
    void mode4thRisingEdge() {  con.ICM=4;  }
    void mode16thRisingEdge() {  con.ICM=5;  }
    void modeFallingEdgeFirst() {  con.ICM=6; con.FEDGE=0;  }
    void modeRisingEdgeFirst () {  con.ICM=6; con.FEDGE=1;  }

    uint32_t operator () () volatile {  return buf;  }
  };


  struct InterruptsConfig
  {
    volatile union CON
    {
      struct
      {
        uint32_t INT0EP:1;
        uint32_t INT1EP:1;
        uint32_t INT2EP:1;
        uint32_t INT3EP:1;
        uint32_t INT4EP:1;
        uint32_t :3;
        uint32_t TPC:3;
        uint32_t :1;
        uint32_t MVEC:1;
        uint32_t :11;
        uint32_t NMIKEY:8;
      };
      struct {
        uint32_t w:32;
      };
    } con,conCLR,conSET,conINV;

    void setExternalInterruptEdgeFalling( uint8_t _no ) {  conCLR.w = 1U<<_no;  }
    void setExternalInterruptEdgeRising ( uint8_t _no ) {  conSET.w = 1U<<_no;  }

    void configureSystemMultVector()
    {
      CON c;
      c.w = 0;
      c.MVEC = 1;
      conSET.w = c.w;
    }

    void setTemporalProximityControl( uint8_t level ) // 0..7
    {
      CON c;
      c.w = 0;
      c.TPC = 7U;
      conCLR.w = c.w;
      if( level )
      {
        c.TPC = (uint32_t)level;
        conSET.w = c.w;
      }
    }
  };

  extern InterruptsConfig Interrupts;


  struct ExternalInt;
  extern ExternalInt ExternalInt0;
  extern ExternalInt ExternalInt1;
  extern ExternalInt ExternalInt2;
  extern ExternalInt ExternalInt3;
  extern ExternalInt ExternalInt4;

  struct ExternalInt
  {
    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    uint8_t _no;
    ExternalInt( uint8_t n ) : _no(n) {}
    uint8_t no() {  return _no;  }

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }

    static void setEdgeFalling( uint8_t _no ) {  Interrupts.setExternalInterruptEdgeFalling(_no);  }  // default
    static void setEdgeRising ( uint8_t _no ) {  Interrupts.setExternalInterruptEdgeRising (_no);  }

    void setEdgeFalling() {  setEdgeFalling(_no);  }  // default
    void setEdgeRising()  {  setEdgeRising (_no);  }
  };


  struct Dma;
  struct DmaChannel;

  #ifdef _DMAC
  extern Dma        Dma;
  extern DmaChannel DmaChannel0;
  extern DmaChannel DmaChannel1;
  extern DmaChannel DmaChannel2;
  extern DmaChannel DmaChannel3;
  extern DmaChannel DmaChannel4;
  extern DmaChannel DmaChannel5;
  extern DmaChannel DmaChannel6;
  extern DmaChannel DmaChannel7;
  #endif

  struct Dma
  {
    volatile union CON
    {
      struct {
        uint32_t :15;
        uint32_t ON:1;
      };
      struct {
        uint32_t w:32;
      };
    } con,conCLR,conSET,conINV;

    void start() {  con.ON=1;   }
    void stop()  {  con.ON=0; __asm volatile ( "nop" );  }
  };

  struct DmaChannel
  {
    uint8_t no() const // 0..
    {
      return ((size_t)this - (size_t)&DmaChannel0) / 0xC0;
    }

    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    bool intFlag   () {  return Ien[no()].flag();  }
    void intClrFlag() {  Ien[no()].clrFlag();  }
    void intEnable () {  Ien[no()].enable();  }
    void intDisable() {  Ien[no()].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }

    volatile union CON
    {
      struct
      {
        uint32_t CHPRI:2;
        uint32_t CHEDET:1;
        uint32_t :1;
        uint32_t CHAEN:1;
        uint32_t CHCHN:1;
        uint32_t CHAED:1;
        uint32_t CHEN:1;
        uint32_t CHCHNS:1;
        uint32_t :2;
        uint32_t CHPATLEN:1;
        uint32_t :1;
        uint32_t CHPIGNEN:1;
        uint32_t :1;
        uint32_t CHBUSY:1;
        uint32_t :8;
        uint32_t CHPIGN:8;
      };
      struct {
        uint32_t w:32;
      };
    } con,conCLR,conSET,conINV;

    volatile union ECON
    {
      struct
      {
        uint32_t :3;
        uint32_t AIRQEN:1;
        uint32_t SIRQEN:1;
        uint32_t PATEN:1;
        uint32_t CABORT:1;
        uint32_t CFORCE:1;
        uint32_t CHSIRQ:8;
        uint32_t CHAIRQ:8;
      };
      struct {
        uint32_t w:32;
      };
    } econ,econCLR,econSET,econINV;

    volatile union INT
    {
      struct
      {
        uint32_t CHERIF:1;
        uint32_t CHTAIF:1;
        uint32_t CHCCIF:1;
        uint32_t CHBCIF:1;
        uint32_t CHDHIF:1;
        uint32_t CHDDIF:1;
        uint32_t CHSHIF:1;
        uint32_t CHSDIF:1;
        uint32_t :8;
        uint32_t CHERIE:1; // err
        uint32_t CHTAIE:1; // transfer abort
        uint32_t CHCCIE:1; // cell transfer complete
        uint32_t CHBCIE:1; // block transfer complete
        uint32_t CHDHIE:1; // dest half
        uint32_t CHDDIE:1; // dest full
        uint32_t CHSHIE:1; // src half
        uint32_t CHSDIE:1; // src full
      };
      struct {
        uint32_t w:32;
      };
    } int_,int_CLR,int_SET,int_INV;

    volatile uint32_t ssa,ssaCLR,ssaSET,ssaINV;
    volatile uint32_t dsa,dsaCLR,dsaSET,dsaINV;
    volatile uint32_t ssiz,ssizCLR,ssizSET,ssizINV;
    volatile uint32_t dsiz,dsizCLR,dsizSET,dsizINV;
    volatile uint32_t sptr,sptrCLR,sptrSET,sptrINV;
    volatile uint32_t dptr,dptrCLR,dptrSET,dptrINV;
    volatile uint32_t csiz,csizCLR,csizSET,csizINV;
    volatile uint32_t cptr,cptrCLR,cptrSET,cptrINV;
    volatile uint32_t dat,datCLR,datSET,datINV;

    void clearFlags()
    {
      int_CLR.w = 0xFFU;
    }

    static uint32_t VirtToPhys( const void *_p )
    {
      uint32_t p = (uint32_t)_p;
      if( p & 0x80000000UL )
        return p & 0x1FFFFFFFUL;
      else
        return p + 0x40000000UL;
      //return (int32_t)p<0 ? ((int32_t)p&0x1fffffffL):(uint32_t)((unsigned char*)p+0x40000000L);
    }

    void setTxfer( const void *src, void *dst, int srcSize, int dstSize, int cellSize )
    {
      ssa = VirtToPhys( src );
      dsa = VirtToPhys( dst );

      ssiz = srcSize;
      dsiz = dstSize;
      csiz = cellSize;

      clearFlags();  // clear any previous set flags
    }

    void startTxfer()
    {
      clearFlags();
      con.CHEN = 1;    // enable
      econ.CFORCE = 1; // start transfer
    }

    void abort()
    {
      econ.CABORT = 1;
      Dma.start();  // make sure it's on
      while( econ.CABORT );  // wait abort
    }

    void open()
    {
      intDisable();
      abort();
      econ.AIRQEN=0;
      econ.SIRQEN=0;
      econ.PATEN=0;
      int_CLR.w = 0xFFFFFFFFUL;
      intClrFlag();
    }

    void setPriority( uint8_t p ) {  con.CHPRI = p;  }   // 0..3

    void enableStartIrq( uint8_t irqno ) {  econ.SIRQEN=1;  econ.CHSIRQ=irqno;  }
    void enableAbortIrq( uint8_t irqno ) {  econ.AIRQEN=1;  econ.CHAIRQ=irqno;  }

    void enableEventBlockTransferComplete() {  int_.CHBCIE=1;  }
    bool getEventBlockTransferComplete()    {  return !!int_.CHBCIF;  }
    void clrEventBlockTransferComplete()    {  int_.CHBCIF=0;  }

    void start() {  con.CHEN=1;   }
    void stop()  {  con.CHEN=0; __asm volatile ( "nop" );  }
  };


  struct Adc;
#if __PIC32MX__ && defined(_ADC10)
  extern Adc Adc1;
#endif

  struct Adc
  {
    static InterruptMask Ien[];
    static InterruptVector IntVec[];

    bool intFlag   () {  return Ien[0].flag();  }
    void intClrFlag() {  Ien[0].clrFlag();  }
    void intEnable () {  Ien[0].enable();  }
    void intDisable() {  Ien[0].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[0].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[0].setSubPri(v);  }

    volatile union CON1
    {
      struct
      {
        uint32_t DONE:1;
        uint32_t SAMP:1;
        uint32_t ASAM:1;
        uint32_t :1;
        uint32_t CLRASAM:1;
        uint32_t SSRC:3;
        uint32_t FORM:3;
        uint32_t :2;
        uint32_t SIDL:1;
        uint32_t :1;
        uint32_t ON:1;
      };
      struct {  uint32_t w:32;  };
    } con1,con1CLR,con1SET,con1INV;

    volatile union CON2
    {
      struct
      {
        uint32_t ALTS:1;
        uint32_t BUFM:1;
        uint32_t SMPI:4;
        uint32_t :1;
        uint32_t BUFS:1;
        uint32_t :2;
        uint32_t CSCNA:1;
        uint32_t :1;
        uint32_t OFFCAL:1;
        uint32_t VCFG:3;
      };
      struct {  uint32_t w:32;  };
    } con2,con2CLR,con2SET,con2INV;

    volatile union CON3
    {
      struct
      {
        uint32_t ADCS:8;
        uint32_t SAMC:5;
        uint32_t :2;
        uint32_t ADRC:1;
      };
      struct {  uint32_t w:32;  };
    } con3,con3CLR,con3SET,con3INV;

    volatile uint32_t dummy[4];

    volatile union CHS
    {
      struct
      {
        uint32_t :16;
        uint32_t CH0SA:4;
        uint32_t :3;
        uint32_t CH0NA:1;
        uint32_t CH0SB:4;
        uint32_t :3;
        uint32_t CH0NB:1;
      };
      struct {  uint32_t w:32;  };
    } chs,chsCLR,chs3SET,chsINV;

    volatile union
    {
      struct {  uint32_t CSSL:16;  };
      struct {  uint32_t w;        };
    } cssl,csslCLR,csslSET,csslINV;

    volatile union
    {
      struct {  uint32_t PCFG:16;  };
      struct {  uint32_t w;        };
    } pcfg,pcfgCLR,pcfgSET,pcfgINV;

    volatile struct BUF
    {
      uint32_t BUF;
      uint32_t dummy[3];
    } buf[16];

    void start() {  con1.ON=1;   }
    void stop()  {  con1.ON=0; __asm volatile ( "nop" );  }
    void reset() {  con1.w=0; con2.w=0; con3.w=0; chs.w=0; cssl.w=0; __asm volatile ( "nop" );  }

    void setConvertManual () {  con1.SSRC=0;  }  // default
    void setConvertInt0   () {  con1.SSRC=1;  }
    void setConvertTimer3 () {  con1.SSRC=2;  }
    void setConvertClkAuto() {  con1.SSRC=7;  }

    void setSamplingManual() {  con1.ASAM=0;  } // default
    void setSamplingAuto  () {  con1.ASAM=1;  }

    void stopSampling () {  con1.SAMP=0;  } // default
    void startSampling() {  con1.SAMP=1;  }

    bool done() {  return con1.DONE;  }

    void setAnalogIn       ( uint8_t an_pin ) {  pcfgCLR.w = 1UL<<(uint32_t)an_pin;  }
    void selectForInputScan( uint8_t an_pin ) {  csslSET.w = 1UL<<(uint32_t)an_pin;  }

    void setFpbConversionClock()      {  con3.ADRC=0;  } // default
    void setInternalConversionClock() {  con3.ADRC=1;  }

    void setAutoSampleTime( uint8_t Tad ) {  con3.SAMC = Tad;  } // default 0 and not allowed, 1..31
    void setTadTime( uint16_t Tad ) {  con3.ADCS = (Tad>>1)-1;  } // 2..512 with step of 2

    void setVrefVssVdd() {  con2.VCFG = 0;  } // default
    void setVrefVssExt() {  con2.VCFG = 1;  }
    void setVrefExtVdd() {  con2.VCFG = 2;  }
    void setVrefExtExt() {  con2.VCFG = 3;  }

    void clrScanInputs() {  con2.CSCNA = 0; } // default
    void setScanInputs() {  con2.CSCNA = 1; }

    void setIntPerSamples( uint8_t n ) {  con2.SMPI = n-1;  }  // 1..16

    uint16_t read( uint8_t an_pin ) // 0..15
    {
      return buf[an_pin].BUF;
    }
  };

#if __PIC32MX__
  extern Register Ad1Pcfg;
  inline void Port::setDigital( uint32_t v ) {  if(this==&PortB) Ad1Pcfg.set(v);  }
  inline void Port::setAnalog ( uint32_t v ) {  if(this==&PortB) Ad1Pcfg.clr(v);  setIn(v);  }
#endif

  struct I2C;
  template < I2C& > inline uint8_t i2cNo();

  #ifdef _I2C1
  extern I2C I2C1;
  #endif
  #ifdef _I2C2
  extern I2C I2C2;
  #endif
  #ifdef _I2C3
  extern I2C I2C3;
  #endif
  #ifdef _I2C4
  extern I2C I2C4;
  #endif
  #ifdef _I2C5
  extern I2C I2C5;
  #endif

  struct I2C
  {

    uint8_t no() const // 0..
    {
#if __PIC32MX__
      return ((size_t)this - (size_t)&I2C3) / 0x100;
#elif __PIC32MZ__
      return ((size_t)this - (size_t)&I2C1) / 0x200;
#endif
    }

    static InterruptMaskI2C Ien[];
#if __PIC32MX__
    static InterruptVector IntVec[];
#elif __PIC32MZ__
    // static InterruptVector?? IntVec[];
#endif

    typedef InterruptMaskI2C::Event Event;
#if __PIC32MZ__
    //typedef InterruptVectorComm::Event EventV;
#endif

    bool intFlag( Event ev ) {  return Ien[no()].flag(ev);  }
    void intClrFlag( Event ev ) {  Ien[no()].clrFlag(ev);  }

    void intEnable ( Event ev ) {  Ien[no()].enable(ev);   }
    void intDisable( Event ev ) {  Ien[no()].disable(ev);  }

    void intClrFlag() {  Ien[no()].clrFlag(InterruptMaskI2C::evCollision);  Ien[no()].clrFlag(InterruptMaskI2C::evMaster);  Ien[no()].clrFlag(InterruptMaskI2C::evSlave);  }
    void intEnable () {  Ien[no()].enable (InterruptMaskI2C::evCollision);  Ien[no()].enable (InterruptMaskI2C::evMaster);  Ien[no()].enable (InterruptMaskI2C::evSlave);  }
    void intDisable() {  Ien[no()].disable(InterruptMaskI2C::evCollision);  Ien[no()].disable(InterruptMaskI2C::evMaster);  Ien[no()].disable(InterruptMaskI2C::evSlave);  }

#if __PIC32MX__
    void setIntPri   ( uint8_t v ) {  IntVec[no()].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[no()].setSubPri(v);  }
#elif __PIC32MZ__
//     void setIntPri   ( EventV ev, uint8_t v ) {  IntVec[no()].setPri(ev, v);  }
//     void setIntSubPri( EventV ev, uint8_t v ) {  IntVec[no()].setSubPri(ev, v);  }
#endif


    volatile union CON
    {
      struct
      {
        uint32_t SEN:1;
        uint32_t RSEN:1;
        uint32_t PEN:1;
        uint32_t RCEN:1;
        uint32_t ACKEN:1;
        uint32_t ACKDT:1;
        uint32_t STREN:1;
        uint32_t GCEN:1;
        uint32_t SMEN:1;
        uint32_t DISSLW:1;
        uint32_t A10M:1;
        uint32_t STRICT:1;
        uint32_t SCLREL:1;
        uint32_t SIDL:1;
        uint32_t :1;
        uint32_t ON:1;
//     uint32_t DHEN:1;
//     uint32_t AHEN:1;
//     uint32_t SBCDE:1;
//     uint32_t SDAHT:1;
//     uint32_t BOEN:1;
//     uint32_t SCIE:1;
//     uint32_t PCIE:1;
      };
      struct {  uint32_t w:32;  };
    } con,conCLR,conSET,conINV;

    volatile union STAT
    {
      struct
      {
        uint32_t TBF:1;
        uint32_t RBF:1;
        uint32_t R_W:1;
        uint32_t S:1;
        uint32_t P:1;
        uint32_t D_A:1;
        uint32_t I2COV:1;
        uint32_t IWCOL:1;
        uint32_t ADD10:1;
        uint32_t GCSTAT:1;
        uint32_t BCL:1;
        uint32_t :3;        // :2;    uint32_t ACKTIM:1;
        uint32_t TRSTAT:1;
        uint32_t ACKSTAT:1;
      };
      struct {  uint32_t w:32;  };

      bool transmitterFull()  const volatile {  return TBF;  }
      bool dataAvailable()    const volatile {  return RBF;  }
      bool slaveRead()        const volatile {  return R_W;  } // not valid if master
      bool startDetected()    const volatile {  return S;    }
      bool stopDetected()     const volatile {  return P;    }
      bool slaveData()        const volatile {  return D_A;  } // false if last sent/receives byte was an address, true otherwise
      bool rxOverflow()       const volatile {  return I2COV; }
      bool txOverflow()       const volatile {  return IWCOL; }
      bool slave10bitAdressMatch() const volatile {  return ADD10; }
      bool generalCallAddressReceived() const volatile {  return GCSTAT; }
      bool arbitrationLoss()  const volatile {  return BCL;    }
      bool txBusy()           const volatile {  return TRSTAT; }
      bool byteAcknowledged() const volatile {  return !ACKSTAT; }

    } stat,statCLR,statSET,statINV;

    volatile uint32_t add,addCLR,addSET,addINV;  // 10bit
    volatile uint32_t msk,mskCLR,mskSET,mskINV;  // 10bit
    volatile uint32_t brg,brgCLR,brgSET,brgINV;  // 16bit
    volatile uint32_t trn,trnCLR,trnSET,trnINV;  //  8bit
    volatile uint32_t rcv;                       //  8bit

    uint32_t setFrequencyHigh()
    {
      return setFrequency( 400000 );
    }

    uint32_t setFrequency()
    {
      return setFrequency( 100000 );
    }

    uint32_t setFrequency( uint32_t i2cClock )
    {
      return setFrequency( Fpb, i2cClock );
    }

    uint32_t setFrequency( uint32_t sourceClock, uint32_t i2cClock )
    {
      brg = (sourceClock/i2cClock)/2 - 2;
      return ( sourceClock/(brg + 2) )/2;
    }

    void reset()
    {
      con.w = 0;
      stat.w = 0;
//       add = msk = brg = 0;
    }

    void enable()
    {
      con.ON = 1;
    }

    void disable()
    {
      con.ON = 0;
    }

    bool busIdle()
    {
      // Check the status of the Start & Stop bits to determine if the bus is idle.
      // tu jest jakaś Francja, z PLIBa
      return ( stat.S == 0 && stat.P == 0 ) || ( stat.S == 0 && stat.P == 1 );
    }

//     void waitIdle()
//     {
//       while( con.SEN || con.PEN || con.RSEN || con.RCEN || con.ACKEN || stat.TRSTAT );
//     }

    bool slaveDataReadRequested()
    {
      return stat.R_W;
    }

    void slaveClockHold()
    {
      con.SCLREL = 0;
    }

    void slaveClockRelease()
    {
      con.SCLREL = 1;
    }

    bool receivedByteIsAnAddress()
    {
      return !stat.D_A;
    }

    void setSlaveAddress7bit()
    {
      con.A10M = 0;
    }

    void setSlaveAddress10bit()
    {
      con.A10M = 1;
    }

    void setSlaveNoGeneralCallAddress()
    {
      con.GCEN = 0;
    }

    void setSlaveGeneralCallAddress()  // Enable general call address identification.
    {
      con.GCEN = 1; // Setting this bit configures the module to identify the general call address (0) in slave mode.
    }

    void setSlaveNoReservedAddress()
    {
      con.STRICT = 0;
    }

    void setSlaveUseReservedAddress()
    {
      con.STRICT = 1;  // Disable reserved address protection, allowing respones to reserved addresses (violates I2C specification)
    }

    void setSlaveAddress( uint16_t address, uint16_t mask )
    {
      add = address;
      msk = mask;
    }


    bool transmitterReady()
    {
      return !stat.TBF;
    }

    bool transmissionCompleted()
    {
      return !stat.TRSTAT;
    }

    bool receivedDataAvailable()
    {
      return stat.RBF;
    }

    bool acknowledged() const volatile
    {
      return stat.byteAcknowledged();
    }


    bool start()  // true if success
    {
      con.SEN = 1;
      return !stat.BCL;  // check collisions
    }

    bool repeatStart()
    {
      con.RSEN = 1;
      // Check for collisions
      return !( stat.BCL || stat.IWCOL );
    }

    void stop()
    {
      con.PEN = 1;
    }

    void clrStop()
    {
      con.PEN = 0;
    }

    bool receiverEnable( bool enable = true )
    {
      con.RCEN = enable;
      return !stat.I2COV; // Check for an overflow condition
    }

//     int masterWrite( unsigned char data_out )
//     {
//       trn = data_out;
//       if( stat.IWCOL )  // collision
//         return -1;
//       else
//       {
//         while( stat.TBF );   // wait until write cycle is complete
//         idle();
//         if( stat.ACKSTAT )   // test for ACK condition received
//           return -2;
//         else
//           return 0;
//       }
//     }

    bool sendByte( unsigned char data )
    {
      trn = data;
      return !( stat.BCL || stat.IWCOL );  // Check for collisions
    }

//     unsigned char masterRead()
//     {
//       receiverEnable();
//       while( con.RCEN );
//       stat.I2COV = 0;
//       return getByte();
//     }

    unsigned char getByte()
    {
      return rcv;
    }

    void ack()
    {
      con.ACKDT = 0;
      con.ACKEN = 1;
    }

    void nak()
    {
      con.ACKDT = 1;
      con.ACKEN = 1;
    }

    bool acknowledgeCompleted()
    {
      return !con.ACKEN;
    }


    void clearStatus()
    {
      stat.w = 0;
    }

    union STAT getStatus()
    {
      // Read the status and control registers
      union STAT status;
      union CON  control;

      status.w = stat.w;
      control.w = con.w;

      // Condition the "Start" flag so it does not get set until
      // the "Start" (or "Repeated Start") signal is complete.
      if( control.SEN || control.RSEN )
        status.S = 0;

      // Condition the "Stop" flag so it does not get set until
      // the "Stop" signal is complete.
      if( control.PEN )
        status.P = 0;

      return status;
    }
  };


  struct Flash
  {
    volatile union CON
    {
      struct
      {
        uint32_t NVMOP:4;
#if __PIC32MX__
        uint32_t :7;
        uint32_t LVDSTAT:1;
#elif __PIC32MZ__
        uint32_t :2;
        uint32_t BFSWAP:1;
        uint32_t PFSWAP:1;
        uint32_t :4;
#else
        uint32_t :8;
#endif
        uint32_t LVDERR:1;
        uint32_t WRERR:1;
        uint32_t WREN:1;
        uint32_t WR:1;
      };
      struct
      {
        uint32_t w:32;
      };
    } con,conCLR,conSET,conINV;

    volatile uint32_t KEY;
    volatile uint32_t dummy1[3];

    volatile uint32_t ADDR;
    volatile uint32_t dummy2[3];

#if __PIC32MX__
    volatile uint32_t DATA;
    volatile uint32_t dummy3[3];
#elif __PIC32MZ__
    struct
    {
      volatile uint32_t DATA;
      volatile uint32_t dummy[3];
    } DATA[4];
#endif

    volatile uint32_t SOURCE_ADDR;
    volatile uint32_t dummy4[3];

    bool isError() {  return con.LVDERR || con.WRERR;  }
    bool isWriting() {  return con.WR;  }

#if __PIC32MZ__
    static constexpr size_t pageSize = 16384;
    static constexpr size_t rowSize  =  2048;
#elif __PIC32MX__
    static constexpr size_t pageSize =  4096;
    static constexpr size_t rowSize  =   512;
#endif

    void __attribute__((nomips16)) startOperation( unsigned nvmop )
    {
      union CON con0;
      con0.w = 0;
      con0.WREN = 1;
      con0.NVMOP = nvmop;
      con.w = con0.w;
      DelayUs(7);
      KEY = 0xAA996655;
      KEY = 0x556699AA;
      con0.w = 0;
      con0.WR = 1;
      conSET.w = con0.w;
    }

    void stop()
    {
      union CON con0;
      con0.w = 0;
      con0.WREN = 1;
      conCLR.w = con0.w;
    }

  };
  extern Flash NVM;


#if __PIC32MX__
  template<> inline uint8_t i2cNo<I2C3>() {  return 0;  }
  template<> inline uint8_t i2cNo<I2C4>() {  return 1;  }
  template<> inline uint8_t i2cNo<I2C5>() {  return 2;  }
  template<> inline uint8_t i2cNo<I2C1>() {  return 3;  }
  #ifdef _I2C2
  template<> inline uint8_t i2cNo<I2C2>() {  return 4;  }
  #endif
#elif __PIC32MZ__
  template<> inline uint8_t i2cNo<I2C1>() {  return 0;  }
  template<> inline uint8_t i2cNo<I2C2>() {  return 1;  }
  template<> inline uint8_t i2cNo<I2C3>() {  return 2;  }
  template<> inline uint8_t i2cNo<I2C4>() {  return 3;  }
  template<> inline uint8_t i2cNo<I2C5>() {  return 4;  }
#endif

#if __PIC32MX__

#ifdef _PCACHE

  struct CacheConfig
  {
    volatile union CheCon
    {
      struct {
        uint32_t PFMWS:3;
        uint32_t :1;
        uint32_t PREFEN:2;
        uint32_t :2;
        uint32_t DCSZ:2;
        uint32_t :6;
        uint32_t CHECOH:1;
      };
      struct {
        uint32_t w:32;
      };
    } con,conclr,conset,coninv;

    void invalidateAllOnPFM() {  CheCon c; c.w=0; c.CHECOH=1; conset.w = c.w;  }
    void setDataCacheLines( uint32_t v ) {  con.DCSZ = v;  } // 0->0, 1->1, 2->2, 3->4
    void setPrefetchCache( uint32_t v ) {  con.PREFEN = v;  } // 0->disable, 1->cacheable regions, 2->non cacheable, 3->all memory
    void setFlashWaitStates( uint32_t v ) {  con.PFMWS = v;  } // 0..7

  };
  extern CacheConfig CacheConfig;

#endif
#ifdef _BMX

  struct BusMatrix
  {
    volatile union BmxCon
    {
      struct {
        uint32_t BMXARB:3;
        uint32_t :3;
        uint32_t BMXWSDRM:1;
        uint32_t :9;
        uint32_t BMXERRIS:1;
        uint32_t BMXERRDS:1;
        uint32_t BMXERRDMA:1;
        uint32_t BMXERRICD:1;
        uint32_t BMXERRIXI:1;
        uint32_t :5;
        uint32_t BMXCHEDMA:1;
      };
      struct {
        uint32_t w:32;
      };
    } con,conclr,conset,coninv;

    void set0WaitStatesToRAM()
    {
      con.BMXWSDRM = 0;
    }
  };
  extern BusMatrix BusMatrix;

#endif

#endif

  void setupVectorsOffsets();

}


// plib macros

  #define BIT_0 (1U<<0)
  #define BIT_1 (1U<<1)
  #define BIT_2 (1U<<2)
  #define BIT_3 (1U<<3)
  #define BIT_4 (1U<<4)
  #define BIT_5 (1U<<5)
  #define BIT_6 (1U<<6)
  #define BIT_7 (1U<<7)
  #define BIT_8 (1U<<8)
  #define BIT_9 (1U<<9)
  #define BIT_10 (1U<<10)
  #define BIT_11 (1U<<11)
  #define BIT_12 (1U<<12)
  #define BIT_13 (1U<<13)
  #define BIT_14 (1U<<14)
  #define BIT_15 (1U<<15)

  #define IOPORT_A ::pic::PortA
  #define IOPORT_B ::pic::PortB
  #define IOPORT_C ::pic::PortC
  #define IOPORT_D ::pic::PortD
  #define IOPORT_E ::pic::PortE
  #define IOPORT_F ::pic::PortF
  #define IOPORT_G ::pic::PortG
  #define IOPORT_H ::pic::PortH
  #define IOPORT_I ::pic::PortI

  #define IoPortId ::pic::Port&

  #define PORTSetPinsDigitalOut( p, b ) do{ (p).setDigital(b); (p).setOut(b); }while(0)
  #define PORTSetPinsDigitalIn( p, b )  do{ (p).setDigital(b); (p).setIn(b);  }while(0)
  #define PORTSetBits( p, b )           (p).set(b)
  #define PORTClearBits( p, b )         (p).clr(b)
  #define PORTToggleBits( p, b )        (p).inv(b)
  #define PORTReadBits( p, b )          ( (p)()&(b) )

#endif // __cplusplus
#endif // __PIC_HARDWARE_H__


#if 0

// BOOTLOADER hack

//   #define IOPORT_A A
//   #define IOPORT_B B
//   #define IOPORT_C C
//   #define IOPORT_D D
//   #define IOPORT_E E
//   #define IOPORT_F F
//   #define IOPORT_G G
//   #define IOPORT_H H
//   #define IOPORT_I I

//   #define PORTconcat( a, b, c ) a##b##c
//   #define PORTSetPinsDigitalOut( p, b ) ( PORTconcat(ANSEL,p,CLR) = 1, PORTconcat(TRIS,p,CLR) = (b) )
//   #define PORTSetPinsDigitalIn( p, b )  ( PORTconcat(ANSEL,p,CLR) = 1, PORTconcat(TRIS,p,SET) = (b) )
//   #define PORTSetBits( p, b )           ( PORTconcat(PORT,p,SET) = (b) )
//   #define PORTClearBits( p, b )         ( PORTconcat(PORT,p,CLR) = (b) )
//   #define PORTToggleBits( p, b )        ( PORTconcat(PORT,p,INV) = (b) )
//   #define PORTReadBits( p, b )          ( PORTconcat(PORT,p,)&(b) )

#endif
