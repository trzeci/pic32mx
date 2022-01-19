#include <regdef.h>
#include <stdint.h>

union SRSctl
{
  struct
  {
    uint32_t CSS:4;
    uint32_t dm0:2;
    uint32_t PSS:4;
    uint32_t dm1:2;
    uint32_t ESS:4;
    uint32_t dm2:2;
    uint32_t EICSS:4;
    uint32_t dm3:4;
    uint32_t HSS:4;
  };
  uint32_t w;
};

// general pointer should be copied to all shadow register sets, if any
static inline void __attribute__((nomips16,nomicromips,always_inline)) __populateGpToShadows()
{
  union SRSctl currentSRSCtl,pss;
  asm volatile ( "mfc0 %0,$12,2" : "=r"(currentSRSCtl.w) );
  pss = currentSRSCtl;
  /*
    * HSS:
    * 0 -> 1 set
    * 1 -> 2
    * 3 -> 4
    * 7 -> 8
    * */
  for( uint8_t n=1; n<=currentSRSCtl.HSS; ++n )
  {
    pss.PSS = n;
    asm volatile ( "mtc0 %0,$12,2" : : "r"(pss.w) );
    asm volatile ( "ehb" );
    asm volatile ( "wrpgpr $28,$28" ); // gp
  }
  asm volatile ( "mtc0 %0,$12,2" : : "r"(currentSRSCtl.w) );
  asm volatile ( "ehb" );
}

static inline void __attribute__((nomips16,nomicromips,always_inline)) kseg0CacheOn()
{
  register unsigned long tmp;
  asm("mfc0 %0,$16,0" :  "=r"(tmp));
  tmp = (tmp & ~7) | 3;
  asm("mtc0 %0,$16,0" :: "r" (tmp));
}

void __attribute__((weak,nomips16,nomicromips)) __initializeCpuCaches(){}

void __attribute__((nomips16,nomicromips)) __initializeCpu()
{
  __populateGpToShadows();
  __initializeCpuCaches();
  kseg0CacheOn();
}

void __attribute__((nomips16,nomicromips,weak)) _init(){}

typedef void (*init_array_slot)();
extern init_array_slot __attribute__((section(".preinit_array"))) __preinit_array_start, __preinit_array_end;
extern init_array_slot __attribute__((section(".init_array")))       __init_array_start,    __init_array_end;

void __attribute__((nomips16,nomicromips)) __init_array_init()
{
  for( init_array_slot *t = &__preinit_array_end; t > &__preinit_array_start; )
    (*--t)();
  _init();
  for( init_array_slot *t = &__init_array_end; t > &__init_array_start; )
    (*--t)();
}

