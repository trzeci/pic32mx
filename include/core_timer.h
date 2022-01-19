// author: Copyright (C) 2022 Pawel Susicki
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#ifndef __CORE_TIMER_H__
#define __CORE_TIMER_H__

#include "pic_hardware.h"

#if __cplusplus


class CoreTimer   // counts Fcy/2
{
  public:

    static ::pic::InterruptMask Ien[];
    static ::pic::InterruptVector IntVec[];

    bool intFlag   () {  return Ien[0].flag();  }
    void intClrFlag() {  Ien[0].clrFlag();  }
    void intEnable () {  Ien[0].enable();  }
    void intDisable() {  Ien[0].disable();  }

    void setIntPri   ( uint8_t v ) {  IntVec[0].setPri(v);  }
    void setIntSubPri( uint8_t v ) {  IntVec[0].setSubPri(v);  }

    volatile uint32_t rtc;

    CoreTimer();
    void reset();

#if __PIC32MZ__
    uint32_t read32()
    {
      uint32_t v;
      asm volatile( "mfc0 %0, $9" : "=r"(v) );
      return v;
    }

    void set32( uint32_t v )
    {
      asm volatile( "mtc0 %0, $9" :: "r"(v) );
    }

    uint32_t readCompare()
    {
      uint32_t v;
      asm volatile( "mfc0 %0, $11" : "=r"(v) );
      return v;
    }
#else
    uint32_t __attribute__((nomips16)) read32();
    void     __attribute__((nomips16)) set32( uint32_t v );
    uint32_t __attribute__((nomips16)) readCompare();
#endif

    uint64_t read64();

    uint64_t read()
    {
      return read64();
    }

    uint64_t operator() ()
    {
      return read64();
    }

    void delayNs( uint32_t ns ) // delay ns
    {
      volatile int32_t t = (int32_t)read32() + NsToCoreTimerTicks32( ns );
      while( (int32_t)read32() - t < 0 )
        ;
    }

    void delayUs( uint32_t us ) // delay us
    {
      while( us-- )
        delayNs(1000);
    }

    void delayMs( uint32_t ms ) // delay ms
    {
      while( ms-- )
        delayUs(1000);
    }

    // return seconds
    double readDouble()
    {
      // Fct - core timer freq
      uint64_t t = read64();
      return (double)t / Fct;
    }

    // return seconds
    float readFloat()
    {
      // Fct - core timer freq
      uint64_t t = read64();
      return (float)t / Fct;
    }
};


extern CoreTimer coreTimer;

#endif // __cplusplus
#endif
