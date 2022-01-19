#include "hardware.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <pic_hardware.h>

pic::PinOut PinLED( LED_PORT_AND_PIN );

int main()
{
  for(;;)
  {
    DelayMs(500);
    PinLED.inv();
  }
  return 0;
}
