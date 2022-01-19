#include <pic32m.h>


void __attribute__((weak,interrupt,nomicromips,nomips16,aligned(16))) _nmi_handler(void)
{
  // clear BEV in CP0 STATUS
  _CP0_BIC_STATUS( _CP0_STATUS_BEV_MASK );
}

void __attribute__((weak,interrupt,nomicromips,nomips16,noreturn,aligned(16))) _general_exception_handler          (void){  while(1);  }

void __attribute__((weak,nomips16, noreturn)) _bootstrap_exception_handler        (void){  while(1);  }
