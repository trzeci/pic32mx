// author: Copyright (C) 2022 Pawel Susicki
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "../include/core_timer.h"

CoreTimer coreTimer;


CoreTimer::CoreTimer()
: rtc(0)
{
  set32(0);
  setIntPri(7);
  setIntSubPri(1);
  intEnable();
  _CP0_SET_COMPARE(0xFFFFFFFFUL);
}


void CoreTimer::reset()
{
  auto intStatus = ::pic::disableInterrupts();
  rtc = 0;
  set32(0);
  _CP0_SET_COMPARE(0xFFFFFFFFUL);
  coreTimer.intClrFlag();
  ::pic::restoreInterrupts( intStatus );
}


#if __XC32_VERSION__
extern "C" void __ISR( _CORE_TIMER_VECTOR, iplCoreTimer ) __rtc_isr()
#else
extern "C" __ISRshadow(_CORE_TIMER_VECTOR)
#endif
{
  _CP0_SET_COMPARE( 0xFFFFFFFFUL );
  coreTimer.intClrFlag();
  ++ coreTimer.rtc;
}


uint64_t CoreTimer::read64()
{
  uint32_t rtc0 = rtc;
  uint32_t v = read32();
  uint32_t rtc1 = rtc;
  uint32_t rtc2 = ( rtc1 != rtc0 && (int32_t)v < 0 ) ? rtc0 : rtc1;
  uint64_t rv = (((uint64_t)rtc2)<<32) | (uint64_t)v;
  return rv;
}


#if !__PIC32MZ__

uint32_t __attribute__((nomips16)) CoreTimer::read32()
{
  uint32_t v;
  asm volatile( "mfc0 %0, $9" : "=r"(v) );
  return v;
}

void     __attribute__((nomips16)) CoreTimer::set32( uint32_t v )
{
  asm volatile( "mtc0 %0, $9" :: "r"(v) );
}

uint32_t __attribute__((nomips16)) CoreTimer::readCompare()
{
  uint32_t v;
  asm volatile( "mfc0 %0, $11" : "=r"(v) );
  return v;
}

#endif
