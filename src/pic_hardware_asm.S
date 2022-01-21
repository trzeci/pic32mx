// author: Copyright (C) 2022 Pawel Susicki
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "../include/pic_hardware.h"

#define glob( name, addr ) name = addr; .global name

#if __PIC32MX__

  #ifdef _PORTA
    glob( _ZN3pic5PortAE, 0xBF886000 )
  #endif
  #ifdef _PORTB
    glob( _ZN3pic5PortBE, 0xBF886040 )
  #endif
  #ifdef _PORTC
    glob( _ZN3pic5PortCE, 0xBF886080 )
  #endif
  #ifdef _PORTD
    glob( _ZN3pic5PortDE, 0xBF8860C0 )
  #endif
  #ifdef _PORTE
    glob( _ZN3pic5PortEE, 0xBF886100 )
  #endif
  #ifdef _PORTF
    glob( _ZN3pic5PortFE, 0xBF886140 )
  #endif
  #ifdef _PORTG
    glob( _ZN3pic5PortGE, 0xBF886180 )
  #endif

  #ifdef _SPI1
    glob( _ZN3pic4Spi1E, 0xBF805E00 )
  #endif
  #ifdef _SPI2
    glob( _ZN3pic4Spi2E, 0xBF805A00 )
  #endif
  #ifdef _SPI3
    glob( _ZN3pic4Spi3E, 0xBF805800 )
  #endif
  #ifdef _SPI4
    glob( _ZN3pic4Spi4E, 0xBF805C00 )
  #endif

  #ifdef _UART1
    glob( _ZN3pic5Uart1E, 0xBF806000 )
  #endif
  #ifdef _UART2
    glob( _ZN3pic5Uart2E, 0xBF806800 )
  #endif
  #ifdef _UART3
    glob( _ZN3pic5Uart3E, 0xBF806400 )
  #endif
  #ifdef _UART4
    glob( _ZN3pic5Uart4E, 0xBF806200 )
  #endif
  #ifdef _UART5
    glob( _ZN3pic5Uart5E, 0xBF806A00 )
  #endif
  #ifdef _UART6
    glob( _ZN3pic5Uart6E, 0xBF806600 )
  #endif

  #ifdef _TMR1
    glob( _ZN3pic6Timer1E, 0xBF800600 )
  #endif
  #ifdef _TMR2
    glob( _ZN3pic6Timer2E, 0xBF800800 )
  #endif
  #ifdef _TMR3
    glob( _ZN3pic6Timer3E, 0xBF800A00 )
  #endif
  #ifdef _TMR4
    glob( _ZN3pic6Timer4E, 0xBF800C00 )
  #endif
  #ifdef _TMR5
    glob( _ZN3pic6Timer5E, 0xBF800E00 )
  #endif

  #ifdef _TMR23
    glob( _ZN3pic7Timer23E, 0xBF800800 )
  #endif
  #ifdef _TMR45
    glob( _ZN3pic7Timer45E, 0xBF800C00 )
  #endif

  #ifdef _OCMP1
    glob( _ZN3pic3OC1E, 0xBF803000 )
  #endif
  #ifdef _OCMP2
    glob( _ZN3pic3OC2E, 0xBF803200 )
  #endif
  #ifdef _OCMP3
    glob( _ZN3pic3OC3E, 0xBF803400 )
  #endif
  #ifdef _OCMP4
    glob( _ZN3pic3OC4E, 0xBF803600 )
  #endif
  #ifdef _OCMP5
    glob( _ZN3pic3OC5E, 0xBF803800 )
  #endif

  #ifdef _ICAP1
    glob( _ZN3pic3IC1E, 0xBF802000 )
  #endif
  #ifdef _ICAP2
    glob( _ZN3pic3IC2E, 0xBF802200 )
  #endif
  #ifdef _ICAP3
    glob( _ZN3pic3IC3E, 0xBF802400 )
  #endif
  #ifdef _ICAP4
    glob( _ZN3pic3IC4E, 0xBF802600 )
  #endif
  #ifdef _ICAP5
    glob( _ZN3pic3IC5E, 0xBF802800 )
  #endif

  #ifdef _DMAC
    glob( _ZN3pic3DmaE, 0xBF883000 )
    glob( _ZN3pic11DmaChannel0E, 0xBF883060 )
    glob( _ZN3pic11DmaChannel1E, 0xBF883120 )
    glob( _ZN3pic11DmaChannel2E, 0xBF8831E0 )
    glob( _ZN3pic11DmaChannel3E, 0xBF8832A0 )
    glob( _ZN3pic11DmaChannel4E, 0xBF883360 )
    glob( _ZN3pic11DmaChannel5E, 0xBF883420 )
    glob( _ZN3pic11DmaChannel6E, 0xBF8834E0 )
    glob( _ZN3pic11DmaChannel7E, 0xBF8835A0 )
  #endif

  #ifdef _I2C1
    glob( _ZN3pic4I2C1E, 0xBF805300 )
  #endif
  #ifdef _I2C2
    glob( _ZN3pic4I2C2E, 0xBF805400 )
  #endif
  #ifdef _I2C3
    glob( _ZN3pic4I2C3E, 0xBF805000 )
  #endif
  #ifdef _I2C4
    glob( _ZN3pic4I2C4E, 0xBF805100 )
  #endif
  #ifdef _I2C5
    glob( _ZN3pic4I2C5E, 0xBF805200 )
  #endif

  #ifdef _ADC10
    glob( _ZN3pic4Adc1E,    0xBF809000 )
    glob( _ZN3pic7Ad1PcfgE, 0xBF809060 )
  #endif

  glob( _ZN3pic10InterruptsE, 0xBF881000 )

  glob( _ZN3pic4ifsvE, 0xBF881030 )
  glob( _ZN3pic4iecvE, 0xBF881060 )
  glob( _ZN3pic4ipcvE, 0xBF881090 )

  #ifdef _PCACHE
    glob( _ZN3pic11CacheConfigE, 0xBF884000 )
  #endif
  #ifdef _BMX
    glob( _ZN3pic9BusMatrixE, 0xBF882000 )
  #endif

  glob( _ZN3pic3NVME, 0xBF80F400 )

#elif __PIC32MZ__

  #ifdef _PORTA
    glob( _ZN3pic5PortAE, 0xBF860000 )
  #endif
  #ifdef _PORTB
    glob( _ZN3pic5PortBE, 0xBF860100 )
  #endif
  #ifdef _PORTC
    glob( _ZN3pic5PortCE, 0xBF860200 )
  #endif
  #ifdef _PORTD
    glob( _ZN3pic5PortDE, 0xBF860300 )
  #endif
  #ifdef _PORTE
    glob( _ZN3pic5PortEE, 0xBF860400 )
  #endif
  #ifdef _PORTF
    glob( _ZN3pic5PortFE, 0xBF860500 )
  #endif
  #ifdef _PORTG
    glob( _ZN3pic5PortGE, 0xBF860600 )
  #endif

  #ifdef _SPI1
    glob( _ZN3pic4Spi1E,  0xBF821000 )
  #endif
  #ifdef _SPI2
    glob( _ZN3pic4Spi2E,  0xBF821200 )
  #endif
  #ifdef _SPI3
    glob( _ZN3pic4Spi3E,  0xBF821400 )
  #endif
  #ifdef _SPI4
    glob( _ZN3pic4Spi4E,  0xBF821600 )
  #endif
  #ifdef _SPI5
    glob( _ZN3pic4Spi5E,  0xBF821800 )
  #endif
  #ifdef _SPI6
    glob( _ZN3pic4Spi6E,  0xBF821A00 )
  #endif

  #ifdef _UART1
    glob( _ZN3pic5Uart1E, 0xBF822000 )
  #endif
  #ifdef _UART2
    glob( _ZN3pic5Uart2E, 0xBF822200 )
  #endif
  #ifdef _UART3
    glob( _ZN3pic5Uart3E, 0xBF822400 )
  #endif
  #ifdef _UART4
    glob( _ZN3pic5Uart4E, 0xBF822600 )
  #endif
  #ifdef _UART5
    glob( _ZN3pic5Uart5E, 0xBF822800 )
  #endif
  #ifdef _UART6
    glob( _ZN3pic5Uart6E, 0xBF822A00 )
  #endif

  #ifdef _TMR1
    glob( _ZN3pic6Timer1E, 0xBF840000 )
  #endif
  #ifdef _TMR2
    glob( _ZN3pic6Timer2E, 0xBF840200 )
  #endif
  #ifdef _TMR3
    glob( _ZN3pic6Timer3E, 0xBF840400 )
  #endif
  #ifdef _TMR4
    glob( _ZN3pic6Timer4E, 0xBF840600 )
  #endif
  #ifdef _TMR5
    glob( _ZN3pic6Timer5E, 0xBF840800 )
  #endif
  #ifdef _TMR6
    glob( _ZN3pic6Timer6E, 0xBF840A00 )
  #endif
  #ifdef _TMR7
    glob( _ZN3pic6Timer7E, 0xBF840C00 )
  #endif
  #ifdef _TMR8
    glob( _ZN3pic6Timer8E, 0xBF840E00 )
  #endif
  #ifdef _TMR9
    glob( _ZN3pic6Timer9E, 0xBF841000 )
  #endif

  #ifdef _TMR23
    glob( _ZN3pic7Timer23E, 0xBF840200 )
  #endif
  #ifdef _TMR45
    glob( _ZN3pic7Timer45E, 0xBF840600 )
  #endif
  #ifdef _TMR67
    glob( _ZN3pic7Timer67E, 0xBF840A00 )
  #endif
  #ifdef _TMR89
    glob( _ZN3pic7Timer89E, 0xBF840E00 )
  #endif

  #ifdef _OCMP1
    glob( _ZN3pic3OC1E, 0xBF844000 )
  #endif
  #ifdef _OCMP2
    glob( _ZN3pic3OC2E, 0xBF844200 )
  #endif
  #ifdef _OCMP3
    glob( _ZN3pic3OC3E, 0xBF844400 )
  #endif
  #ifdef _OCMP4
    glob( _ZN3pic3OC4E, 0xBF844600 )
  #endif
  #ifdef _OCMP5
    glob( _ZN3pic3OC5E, 0xBF844800 )
  #endif
  #ifdef _OCMP6
    glob( _ZN3pic3OC6E, 0xBF844A00 )
  #endif
  #ifdef _OCMP7
    glob( _ZN3pic3OC7E, 0xBF844C00 )
  #endif
  #ifdef _OCMP8
    glob( _ZN3pic3OC8E, 0xBF844E00 )
  #endif
  #ifdef _OCMP9
    glob( _ZN3pic3OC9E, 0xBF845000 )
  #endif

  #ifdef _ICAP1
    glob( _ZN3pic3IC1E, 0xBF842000 )
  #endif
  #ifdef _ICAP2
    glob( _ZN3pic3IC2E, 0xBF842200 )
  #endif
  #ifdef _ICAP3
    glob( _ZN3pic3IC3E, 0xBF842400 )
  #endif
  #ifdef _ICAP4
    glob( _ZN3pic3IC4E, 0xBF842600 )
  #endif
  #ifdef _ICAP5
    glob( _ZN3pic3IC5E, 0xBF842800 )
  #endif
  #ifdef _ICAP6
    glob( _ZN3pic3IC6E, 0xBF842A00 )
  #endif
  #ifdef _ICAP7
    glob( _ZN3pic3IC7E, 0xBF842C00 )
  #endif
  #ifdef _ICAP8
    glob( _ZN3pic3IC8E, 0xBF842E00 )
  #endif
  #ifdef _ICAP9
    glob( _ZN3pic3IC9E, 0xBF843000 )
  #endif

  #ifdef _DMAC
    glob( _ZN3pic3DmaE, 0xBF811000 )
    glob( _ZN3pic11DmaChannel0E, 0xBF811060 )
    glob( _ZN3pic11DmaChannel1E, 0xBF811120 )
    glob( _ZN3pic11DmaChannel2E, 0xBF8111E0 )
    glob( _ZN3pic11DmaChannel3E, 0xBF8112A0 )
    glob( _ZN3pic11DmaChannel4E, 0xBF811360 )
    glob( _ZN3pic11DmaChannel5E, 0xBF811420 )
    glob( _ZN3pic11DmaChannel6E, 0xBF8114E0 )
    glob( _ZN3pic11DmaChannel7E, 0xBF8115A0 )
  #endif

  #ifdef _I2C1
    glob( _ZN3pic4I2C1E, 0xBF820000 )
  #endif
  #ifdef _I2C2
    glob( _ZN3pic4I2C2E, 0xBF820200 )
  #endif
  #ifdef _I2C3
    glob( _ZN3pic4I2C3E, 0xBF820400 )
  #endif
  #ifdef _I2C4
    glob( _ZN3pic4I2C4E, 0xBF820600 )
  #endif
  #ifdef _I2C5
    glob( _ZN3pic4I2C5E, 0xBF820800 )
  #endif

  #ifdef _ADCHS
  // todo
  #endif

  glob( _ZN3pic3NVME, 0xBF800600 )

  glob( _ZN3pic10InterruptsE, 0xBF810000 )

  glob( _ZN3pic4ifsvE, 0xBF810040 )
  glob( _ZN3pic4iecvE, 0xBF8100C0 )
  glob( _ZN3pic4ipcvE, 0xBF810140 )

  glob( _ZN3pic14vectorsOffsetsE, 0xBF810540 )

#endif
