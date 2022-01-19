// author: Copyright (C) 2022 Pawel Susicki
//
// This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0. If a copy of the MPL was not
// distributed with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

#include "../include/pic_hardware.h"
#include "../include/core_timer.h"
#include <string>


extern "C"
{
  void __attribute__((nomips16,nomicromips)) __initializeCpuCaches()
  {
    // interrupt should still be disabled
#ifdef _PCACHE
    static constexpr uint32_t FlashMaxSpeedHz = 30000000;
    {
      // set the flash wait states based on 1 wait state for every 30 MHz above 30MHz
      uint32_t wait_states = 0;
      uint32_t sys_clock = Fcy;
      while( sys_clock > FlashMaxSpeedHz )
      {
        wait_states++;
        sys_clock -= FlashMaxSpeedHz;
      }

      ::pic::CacheConfig.setFlashWaitStates( wait_states <= 7 ? wait_states : 7 );
    }
    ::pic::CacheConfig.setPrefetchCache(3); // cache all addresses
#endif
#ifdef _BMX
    ::pic::BusMatrix.set0WaitStatesToRAM();
#endif
  }
}

namespace pic
{
  void __attribute__((nomips16)) configureSystemMultVector()
  {
    uint32_t val;
    // set the CP0 cause IV bit high
    asm volatile("mfc0   %0,$13" : "=r"(val));
    val |= 0x00800000;
    asm volatile("mtc0   %0,$13" : "+r"(val));
    Interrupts.configureSystemMultVector();
  }
}

#if defined(__32MX575F512H_H) || defined(__32MX575F512L_H) || defined(__32MX695F512H_H) || defined(__32MX695F512L_H)

  namespace pic
  {

    extern volatile uint32_t ifsv[ 3][4];
    extern volatile uint32_t iecv[ 3][4];
    extern volatile uint32_t ipcv[13][4];

    InterruptMask SoftwareInterrupt::Ien[] =
    {
      {  &ifsv[0][0],  &iecv[0][0],  _IFS0_CS0IF_MASK  },
      {  &ifsv[0][0],  &iecv[0][0],  _IFS0_CS1IF_MASK  }
    };

    InterruptVector SoftwareInterrupt::IntVec[] =
    {
      {  &ipcv[0][0], _IPC0_CS0IS_POSITION,  _IPC0_CS0IP_POSITION  },
      {  &ipcv[0][0], _IPC0_CS1IS_POSITION,  _IPC0_CS1IP_POSITION  }
    };

    SoftwareInterrupt softwareInterrupt0(0), softwareInterrupt1(1);

    InterruptMaskI2C I2C::Ien[] =
    {
      {  &ifsv[0][0],  &iecv[0][0],  { _IFS0_I2C3MIF_MASK, _IFS0_I2C3SIF_MASK, _IFS0_I2C3BIF_MASK } },
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_I2C4MIF_MASK, _IFS1_I2C4SIF_MASK, _IFS1_I2C4BIF_MASK } },
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_I2C5MIF_MASK, _IFS1_I2C5SIF_MASK, _IFS1_I2C5BIF_MASK } },
      {  &ifsv[0][0],  &iecv[0][0],  { _IFS0_I2C1MIF_MASK, _IFS0_I2C1SIF_MASK, _IFS0_I2C1BIF_MASK } },
      #ifdef _I2C2
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_I2C2MIF_MASK, _IFS1_I2C2SIF_MASK, _IFS1_I2C2BIF_MASK } },
      #endif
    };

    InterruptVector I2C::IntVec[] =
    {
      {   &ipcv[6][0],  _IPC6_I2C3IS_POSITION,  _IPC6_I2C3IP_POSITION  },
      {   &ipcv[7][0],  _IPC7_I2C4IS_POSITION,  _IPC7_I2C4IP_POSITION  },
      {   &ipcv[8][0],  _IPC8_I2C5IS_POSITION,  _IPC8_I2C5IP_POSITION  },
      {   &ipcv[6][0],  _IPC6_I2C1IS_POSITION,  _IPC6_I2C1IP_POSITION  },
      #ifdef _I2C2
      {   &ipcv[8][0],  _IPC8_I2C2IS_POSITION,  _IPC8_I2C2IP_POSITION  },
      #endif
    };


    InterruptMaskComm Spi::Ien[] =
    {
      {  &ifsv[0][0],  &iecv[0][0],  { _IFS0_SPI3RXIF_MASK, _IFS0_SPI3TXIF_MASK, _IFS0_SPI3EIF_MASK } },
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_SPI2RXIF_MASK, _IFS1_SPI2TXIF_MASK, _IFS1_SPI2EIF_MASK } },
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_SPI4RXIF_MASK, _IFS1_SPI4TXIF_MASK, _IFS1_SPI4EIF_MASK } },
      #ifdef _SPI1
      {  &ifsv[0][0],  &iecv[0][0],  { _IFS0_SPI1RXIF_MASK, _IFS0_SPI1TXIF_MASK, _IFS0_SPI1EIF_MASK  } },  // Receive Done, Transfer Done, fault
      #endif
    };

    InterruptVector Spi::IntVec[] =
    {
      {   &ipcv[6][0],  _IPC6_SPI3IS_POSITION,      _IPC6_SPI3IP_POSITION      },
      {   &ipcv[7][0],  _IPC7_SPI2IS_POSITION,      _IPC7_SPI2IP_POSITION      },
      {   &ipcv[8][0],  _IPC8_SPI4IS_POSITION,      _IPC8_SPI4IP_POSITION      },
      #ifdef _SPI1
      {   &ipcv[5][0],  _IPC5_SPI1IS_POSITION,      _IPC5_SPI1IP_POSITION       },  // SPI 1
      #endif
    };


    InterruptMaskComm Uart::Ien[] =
    {
      {  &ifsv[0][0],  &iecv[0][0],  { _IFS0_U1RXIF_MASK, _IFS0_U1TXIF_MASK, _IFS0_U1EIF_MASK  } },
      {  &ifsv[2][0],  &iecv[2][0],  { _IFS2_U4RXIF_MASK, _IFS2_U4TXIF_MASK, _IFS2_U4EIF_MASK  } },
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_U3RXIF_MASK, _IFS1_U3TXIF_MASK, _IFS1_U3EIF_MASK  } },
      {  &ifsv[2][0],  &iecv[2][0],  { _IFS2_U6RXIF_MASK, _IFS2_U6TXIF_MASK, _IFS2_U6EIF_MASK  } },
      {  &ifsv[1][0],  &iecv[1][0],  { _IFS1_U2RXIF_MASK, _IFS1_U2TXIF_MASK, _IFS1_U2EIF_MASK  } },
      {  &ifsv[2][0],  &iecv[2][0],  { _IFS2_U5RXIF_MASK, _IFS2_U5TXIF_MASK, _IFS2_U5EIF_MASK  } },
    };

    InterruptVector Uart::IntVec[] =
    {
      {  &ipcv[6][0],   _IPC6_U1IS_POSITION,   _IPC6_U1IP_POSITION   },
      {  &ipcv[12][0],  _IPC12_U4IS_POSITION,  _IPC12_U4IP_POSITION  },
      {  &ipcv[7][0],   _IPC7_U3IS_POSITION,   _IPC7_U3IP_POSITION   },
      {  &ipcv[12][0],  _IPC12_U6IS_POSITION,  _IPC12_U6IP_POSITION  },
      {  &ipcv[8][0],   _IPC8_U2IS_POSITION,   _IPC8_U2IP_POSITION   },
      {  &ipcv[12][0],  _IPC12_U5IS_POSITION,  _IPC12_U5IP_POSITION  },
    };

    InterruptMask Timer::Ien[] =
    {
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T1IF_MASK        },  // Timer 1
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T2IF_MASK        },  // Timer 2
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T3IF_MASK        },  // TImer 3
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T4IF_MASK        },  // Timer 4
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T5IF_MASK        },  // Timer 5
    };

    InterruptVector Timer::IntVec[] =
    {
      {   &ipcv[1][0], _IPC1_T1IS_POSITION,    _IPC1_T1IP_POSITION     },  // Timer 1
      {   &ipcv[2][0], _IPC2_T2IS_POSITION,    _IPC2_T2IP_POSITION     },  // Timer 2
      {   &ipcv[3][0], _IPC3_T3IS_POSITION,    _IPC3_T3IP_POSITION     },  // TImer 3
      {   &ipcv[4][0], _IPC4_T4IS_POSITION,    _IPC4_T4IP_POSITION     },  // Timer 4
      {   &ipcv[5][0], _IPC5_T5IS_POSITION,    _IPC5_T5IP_POSITION     },  // Timer 5
    };

    InterruptMask OutputCompare::Ien[] =
    {
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC1IF_MASK       },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC2IF_MASK       },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC3IF_MASK       },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC4IF_MASK       },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC5IF_MASK       },
    };

    InterruptVector OutputCompare::IntVec[] =
    {
      {   &ipcv[1][0], _IPC1_OC1IS_POSITION,   _IPC1_OC1IP_POSITION    },
      {   &ipcv[2][0], _IPC2_OC2IS_POSITION,   _IPC2_OC2IP_POSITION    },
      {   &ipcv[3][0], _IPC3_OC3IS_POSITION,   _IPC3_OC3IP_POSITION    },
      {   &ipcv[4][0], _IPC4_OC4IS_POSITION,   _IPC4_OC4IP_POSITION    },
      {   &ipcv[5][0], _IPC5_OC5IS_POSITION,   _IPC5_OC5IP_POSITION    },
    };

    InterruptMask InputCapture::Ien[] =
    {
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_IC1IF_MASK       },  // Input Capture 1
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_IC2IF_MASK       },  // Input Capture 2
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_IC3IF_MASK       },  // Input Capture 3
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_IC4IF_MASK       },  // Input Capture 4
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_IC5IF_MASK       },  // Input Capture 5
    };

    InterruptMask InputCapture::IEen[] =
    {
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_IC1EIF_MASK       },  // Input Capture 1
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_IC2EIF_MASK       },  // Input Capture 2
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_IC3EIF_MASK       },  // Input Capture 3
      {   &ifsv[2][0],  &iecv[2][0],  _IFS2_IC4EIF_MASK       },  // Input Capture 4
      {   &ifsv[2][0],  &iecv[2][0],  _IFS2_IC5EIF_MASK       },  // Input Capture 5
    };

    InterruptVector InputCapture::IntVec[] =
    {
      {   &ipcv[1][0],  _IPC1_IC1IS_POSITION,   _IPC1_IC1IP_POSITION    },  // Input Capture 1
      {   &ipcv[2][0],  _IPC2_IC2IS_POSITION,   _IPC2_IC2IP_POSITION    },  // Input Capture 2
      {   &ipcv[3][0],  _IPC3_IC3IS_POSITION,   _IPC3_IC3IP_POSITION    },  // Input Capture 3
      {   &ipcv[4][0],  _IPC4_IC4IS_POSITION,   _IPC4_IC4IP_POSITION    },  // Input Capture 4
      {   &ipcv[5][0],  _IPC5_IC5IS_POSITION,   _IPC5_IC5IP_POSITION    },  // Input Capture 5
    };

    ExternalInt ExternalInt0(0);
    ExternalInt ExternalInt1(1);
    ExternalInt ExternalInt2(2);
    ExternalInt ExternalInt3(3);
    ExternalInt ExternalInt4(4);

    InterruptMask ExternalInt::Ien[] =
    {
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_INT0IF_MASK      },  // External Interrupt 0
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_INT1IF_MASK      },  // External Interrupt 1
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_INT2IF_MASK      },  // External Interrrupt 2
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_INT3IF_MASK      },  // External Interrupt 3
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_INT4IF_MASK      },  // External Interupt 4
    };

    InterruptVector ExternalInt::IntVec[] =
    {
      {   &ipcv[0][0], _IPC0_INT0IS_POSITION,  _IPC0_INT0IP_POSITION   },  // External Interrupt 0
      {   &ipcv[1][0], _IPC1_INT1IS_POSITION,  _IPC1_INT1IP_POSITION   },  // External Interrupt 1
      {   &ipcv[2][0], _IPC2_INT2IS_POSITION,  _IPC2_INT2IP_POSITION   },  // External Interrrupt 2
      {   &ipcv[3][0], _IPC3_INT3IS_POSITION,  _IPC3_INT3IP_POSITION   },  // External Interrupt 3
      {   &ipcv[4][0], _IPC4_INT4IS_POSITION,  _IPC4_INT4IP_POSITION   },  // External Interupt 4
    };

    InterruptMask DmaChannel::Ien[] =
    {
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA0IF_MASK  },  // DMA Channel 0
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA1IF_MASK  },  // DMA Channel 1
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA2IF_MASK  },  // DMA Channel 2
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA3IF_MASK  },  // DMA Channel 3
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA4IF_MASK  },  // DMA Channel 4
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA5IF_MASK  },  // DMA Channel 5
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA6IF_MASK  },  // DMA Channel 6
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_DMA7IF_MASK  },  // DMA Channel 7
    };

    InterruptVector DmaChannel::IntVec[] =
    {
      {   &ipcv[9][0],  _IPC9_DMA0IS_POSITION,  _IPC9_DMA0IP_POSITION   },  // DMA Channel 0
      {   &ipcv[9][0],  _IPC9_DMA1IS_POSITION,  _IPC9_DMA1IP_POSITION   },  // DMA Channel 1
      {   &ipcv[9][0],  _IPC9_DMA2IS_POSITION,  _IPC9_DMA2IP_POSITION   },  // DMA Channel 2
      {   &ipcv[9][0],  _IPC9_DMA3IS_POSITION,  _IPC9_DMA3IP_POSITION   },  // DMA Channel 3
      {   &ipcv[10][0], _IPC10_DMA4IS_POSITION, _IPC10_DMA4IP_POSITION  },  // DMA Channel 4
      {   &ipcv[10][0], _IPC10_DMA5IS_POSITION, _IPC10_DMA5IP_POSITION  },  // DMA Channel 5
      {   &ipcv[10][0], _IPC10_DMA6IS_POSITION, _IPC10_DMA6IP_POSITION  },  // DMA Channel 6
      {   &ipcv[10][0], _IPC10_DMA7IS_POSITION, _IPC10_DMA7IP_POSITION  },  // DMA Channel 7
    };

    InterruptMask Adc::Ien[] =
    {
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_AD1IF_MASK    },  // ADC 1 Convert Done
    };

    InterruptVector Adc::IntVec[] =
    {
      {   &ipcv[6][0],  _IPC6_AD1IS_POSITION,   _IPC6_AD1IP_POSITION    },  // ADC 1 Convert Done
    };
  }

#elif defined(__32MZ2048EFH100_H)

  namespace pic
  {

    extern volatile uint32_t ifsv[ 7][4];
    extern volatile uint32_t iecv[ 7][4];
    extern volatile uint32_t ipcv[54][4]; // WARNING, the is a hole at index 24

    InterruptMask Port::Ien[] =
    {
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNAIF_MASK }, // PortA
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNBIF_MASK },
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNCIF_MASK },
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNDIF_MASK },
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNEIF_MASK },
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNFIF_MASK },
      {  &ifsv[3][0],  &iecv[3][0],  _IFS3_CNGIF_MASK }, // PortG
    };

    InterruptVector Port::IntVec[] =
    {
      {   &ipcv[29][0], _IPC29_CNAIS_POSITION, _IPC29_CNAIP_POSITION     },  // PortA
      {   &ipcv[29][0], _IPC29_CNBIS_POSITION, _IPC29_CNBIP_POSITION     },
      {   &ipcv[30][0], _IPC30_CNCIS_POSITION, _IPC30_CNCIP_POSITION     },
      {   &ipcv[30][0], _IPC30_CNDIS_POSITION, _IPC30_CNDIP_POSITION     },
      {   &ipcv[30][0], _IPC30_CNEIS_POSITION, _IPC30_CNEIP_POSITION     },
      {   &ipcv[30][0], _IPC30_CNFIS_POSITION, _IPC30_CNFIP_POSITION     },
      {   &ipcv[31][0], _IPC31_CNGIS_POSITION, _IPC31_CNGIP_POSITION     },  // PortG
    };


    InterruptMaskComm Spi::Ien[] =
    {
      {  &ifsv[3][0],  &iecv[3][0],  { _IFS3_SPI1RXIF_MASK, _IFS3_SPI1TXIF_MASK, _IFS3_SPI1EIF_MASK } },
      {  &ifsv[4][0],  &iecv[4][0],  { _IFS4_SPI2RXIF_MASK, _IFS4_SPI2TXIF_MASK, _IFS4_SPI2EIF_MASK } },
      {  &ifsv[4][0],  &iecv[4][0],  { _IFS4_SPI3RXIF_MASK, _IFS4_SPI3TXIF_MASK, _IFS4_SPI3EIF_MASK } },
      {  &ifsv[5][0],  &iecv[5][0],  { _IFS5_SPI4RXIF_MASK, _IFS5_SPI4TXIF_MASK, _IFS5_SPI4EIF_MASK } },
      {  &ifsv[5][0],  &iecv[5][0],  { _IFS5_SPI5RXIF_MASK, _IFS5_SPI5TXIF_MASK, _IFS5_SPI5EIF_MASK } },
      {  &ifsv[5][0],  &iecv[5][0],  { _IFS5_SPI6RXIF_MASK, _IFS5_SPI6IF_MASK,   _IFS5_SPI6IF_MASK  } },  // tylko RX?
    };

    InterruptVectorComm Spi::IntVec[] =
    {
      {  {&ipcv[27][0],&ipcv[27][0],&ipcv[27][0]},  { _IPC27_SPI1RXIS_POSITION, _IPC27_SPI1TXIS_POSITION, _IPC27_SPI1EIS_POSITION },  { _IPC27_SPI1RXIP_POSITION, _IPC27_SPI1TXIP_POSITION, _IPC27_SPI1EIP_POSITION }  },
      {  {&ipcv[35][0],&ipcv[36][0],&ipcv[35][0]},  { _IPC35_SPI2RXIS_POSITION, _IPC36_SPI2TXIS_POSITION, _IPC35_SPI2EIS_POSITION },  { _IPC35_SPI2RXIP_POSITION, _IPC36_SPI2TXIP_POSITION, _IPC35_SPI2EIP_POSITION }  },  //txis36,txip36
      {  {&ipcv[38][0],&ipcv[39][0],&ipcv[38][0]},  { _IPC38_SPI3RXIS_POSITION, _IPC39_SPI3TXIS_POSITION, _IPC38_SPI3EIS_POSITION },  { _IPC38_SPI3RXIP_POSITION, _IPC39_SPI3TXIP_POSITION, _IPC38_SPI3EIP_POSITION }  },  //txis39, txip39
      {  {&ipcv[41][0],&ipcv[41][0],&ipcv[40][0]},  { _IPC41_SPI4RXIS_POSITION, _IPC41_SPI4TXIS_POSITION, _IPC40_SPI4EIS_POSITION },  { _IPC41_SPI4RXIP_POSITION, _IPC41_SPI4TXIP_POSITION, _IPC40_SPI4EIP_POSITION }  },  //eis40, eip40
      {  {&ipcv[44][0],&ipcv[44][0],&ipcv[44][0]},  { _IPC44_SPI5RXIS_POSITION, _IPC44_SPI5TXIS_POSITION, _IPC44_SPI5EIS_POSITION },  { _IPC44_SPI5RXIP_POSITION, _IPC44_SPI5TXIP_POSITION, _IPC44_SPI5EIP_POSITION }  },
      {  {&ipcv[46][0],&ipcv[46][0],&ipcv[46][0]},  { _IPC46_SPI6RXIS_POSITION, _IPC46_SPI6TXIS_POSITION, _IPC46_SPI6EIS_POSITION },  { _IPC46_SPI6RXIP_POSITION, _IPC46_SPI6TXIP_POSITION, _IPC46_SPI6EIP_POSITION }  },
    };

    InterruptMaskComm Uart::Ien[] =
    {
      {  &ifsv[3][0],  &iecv[3][0],  { _IFS3_U1RXIF_MASK, _IFS3_U1TXIF_MASK, _IFS3_U1EIF_MASK } },
      {  &ifsv[4][0],  &iecv[4][0],  { _IFS4_U2RXIF_MASK, _IFS4_U2TXIF_MASK, _IFS4_U2EIF_MASK } },
      {  &ifsv[4][0],  &iecv[4][0],  { _IFS4_U3RXIF_MASK, _IFS4_U3TXIF_MASK, _IFS4_U3EIF_MASK } },
      {  &ifsv[5][0],  &iecv[5][0],  { _IFS5_U4RXIF_MASK, _IFS5_U4TXIF_MASK, _IFS5_U4EIF_MASK } },
      {  &ifsv[5][0],  &iecv[5][0],  { _IFS5_U5RXIF_MASK, _IFS5_U5TXIF_MASK, _IFS5_U5EIF_MASK } },
      {  &ifsv[5][0],  &iecv[5][0],  { _IFS5_U6RXIF_MASK, _IFS5_U6TXIF_MASK, _IFS5_U6EIF_MASK } },
    };

    InterruptVectorComm Uart::IntVec[] =
    {
     { {&ipcv[28][0],&ipcv[28][0],&ipcv[28][0]}, { _IPC28_U1RXIS_POSITION, _IPC28_U1TXIS_POSITION, _IPC28_U1EIS_POSITION }, { _IPC28_U1RXIP_POSITION, _IPC28_U1TXIP_POSITION, _IPC28_U1EIP_POSITION } },
     { {&ipcv[36][0],&ipcv[36][0],&ipcv[36][0]}, { _IPC36_U2RXIS_POSITION, _IPC36_U2TXIS_POSITION, _IPC36_U2EIS_POSITION }, { _IPC36_U2RXIP_POSITION, _IPC36_U2TXIP_POSITION, _IPC36_U2EIP_POSITION } },
     { {&ipcv[39][0],&ipcv[39][0],&ipcv[39][0]}, { _IPC39_U3RXIS_POSITION, _IPC39_U3TXIS_POSITION, _IPC39_U3EIS_POSITION }, { _IPC39_U3RXIP_POSITION, _IPC39_U3TXIP_POSITION, _IPC39_U3EIP_POSITION } },
     { {&ipcv[42][0],&ipcv[43][0],&ipcv[42][0]}, { _IPC42_U4RXIS_POSITION, _IPC43_U4TXIS_POSITION, _IPC42_U4EIS_POSITION }, { _IPC42_U4RXIP_POSITION, _IPC43_U4TXIP_POSITION, _IPC42_U4EIP_POSITION } },
     { {&ipcv[45][0],&ipcv[45][0],&ipcv[44][0]}, { _IPC45_U5RXIS_POSITION, _IPC45_U5TXIS_POSITION, _IPC44_U5EIS_POSITION }, { _IPC45_U5RXIP_POSITION, _IPC45_U5TXIP_POSITION, _IPC44_U5EIP_POSITION } },
     { {&ipcv[47][0],&ipcv[47][0],&ipcv[47][0]}, { _IPC47_U6RXIS_POSITION, _IPC47_U6TXIS_POSITION, _IPC47_U6EIS_POSITION }, { _IPC47_U6RXIP_POSITION, _IPC47_U6TXIP_POSITION, _IPC47_U6EIP_POSITION } },
    };

    InterruptMask Timer::Ien[] =
    {
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T1IF_MASK        },  // Timer 1
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T2IF_MASK        },  // Timer 2
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T3IF_MASK        },  // TImer 3
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T4IF_MASK        },  // Timer 4
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T5IF_MASK        },  // Timer 5
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_T6IF_MASK        },  // Timer 6
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_T7IF_MASK        },  // Timer 7
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_T8IF_MASK        },  // Timer 8
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_T9IF_MASK        },  // Timer 9
    };

    InterruptVector Timer::IntVec[] =
    {
      {   &ipcv[1][0], _IPC1_T1IS_POSITION,    _IPC1_T1IP_POSITION     },  // Timer 1
      {   &ipcv[2][0], _IPC2_T2IS_POSITION,    _IPC2_T2IP_POSITION     },  // Timer 2
      {   &ipcv[3][0], _IPC3_T3IS_POSITION,    _IPC3_T3IP_POSITION     },  // TImer 3
      {   &ipcv[4][0], _IPC4_T4IS_POSITION,    _IPC4_T4IP_POSITION     },  // Timer 4
      {   &ipcv[6][0], _IPC6_T5IS_POSITION,    _IPC6_T5IP_POSITION     },  // Timer 5
      {   &ipcv[7][0], _IPC7_T6IS_POSITION,    _IPC7_T6IP_POSITION     },  // Timer 6
      {   &ipcv[8][0], _IPC8_T7IS_POSITION,    _IPC8_T7IP_POSITION     },  // Timer 7
      {   &ipcv[9][0], _IPC9_T8IS_POSITION,    _IPC9_T8IP_POSITION     },  // Timer 8
      {   &ipcv[10][0],_IPC10_T9IS_POSITION,   _IPC10_T9IP_POSITION    },  // Timer 9
    };

    InterruptMask OutputCompare::Ien[] =
    {
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC1IF_MASK },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC2IF_MASK },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC3IF_MASK },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC4IF_MASK },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC5IF_MASK },
      {   &ifsv[0][0],  &iecv[0][0],  _IFS0_OC6IF_MASK },
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_OC7IF_MASK },
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_OC8IF_MASK },
      {   &ifsv[1][0],  &iecv[1][0],  _IFS1_OC9IF_MASK },
    };

    InterruptVector OutputCompare::IntVec[] =
    {
      {   &ipcv[1][0],  _IPC1_OC1IS_POSITION,    _IPC1_OC1IP_POSITION    },
      {   &ipcv[3][0],  _IPC3_OC2IS_POSITION,    _IPC3_OC2IP_POSITION    },
      {   &ipcv[4][0],  _IPC4_OC3IS_POSITION,    _IPC4_OC3IP_POSITION    },
      {   &ipcv[5][0],  _IPC5_OC4IS_POSITION,    _IPC5_OC4IP_POSITION    },
      {   &ipcv[6][0],  _IPC6_OC5IS_POSITION,    _IPC6_OC5IP_POSITION    },
      {   &ipcv[7][0],  _IPC7_OC6IS_POSITION,    _IPC7_OC6IP_POSITION    },
      {   &ipcv[8][0],  _IPC8_OC7IS_POSITION,    _IPC8_OC7IP_POSITION    },
      {   &ipcv[9][0],  _IPC9_OC8IS_POSITION,    _IPC9_OC8IP_POSITION    },
      {   &ipcv[10][0], _IPC10_OC9IS_POSITION,   _IPC10_OC9IP_POSITION   },
    };

    InterruptMask DmaChannel::Ien[] =
    {
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA0IF_MASK  },  // DMA Channel 0
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA1IF_MASK  },  // DMA Channel 1
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA2IF_MASK  },  // DMA Channel 2
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA3IF_MASK  },  // DMA Channel 3
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA4IF_MASK  },  // DMA Channel 4
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA5IF_MASK  },  // DMA Channel 5
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA6IF_MASK  },  // DMA Channel 6
      {   &ifsv[4][0],  &iecv[4][0],  _IFS4_DMA7IF_MASK  },  // DMA Channel 7
    };

    InterruptVector DmaChannel::IntVec[] =
    {
      {   &ipcv[33][0], _IPC33_DMA0IS_POSITION, _IPC33_DMA0IP_POSITION  },  // DMA Channel 0
      {   &ipcv[33][0], _IPC33_DMA1IS_POSITION, _IPC33_DMA1IP_POSITION  },  // DMA Channel 1
      {   &ipcv[34][0], _IPC34_DMA2IS_POSITION, _IPC34_DMA2IP_POSITION  },  // DMA Channel 2
      {   &ipcv[34][0], _IPC34_DMA3IS_POSITION, _IPC34_DMA3IP_POSITION  },  // DMA Channel 3
      {   &ipcv[34][0], _IPC34_DMA4IS_POSITION, _IPC34_DMA4IP_POSITION  },  // DMA Channel 4
      {   &ipcv[34][0], _IPC34_DMA5IS_POSITION, _IPC34_DMA5IP_POSITION  },  // DMA Channel 5
      {   &ipcv[35][0], _IPC35_DMA6IS_POSITION, _IPC35_DMA6IP_POSITION  },  // DMA Channel 6
      {   &ipcv[35][0], _IPC35_DMA7IS_POSITION, _IPC35_DMA7IP_POSITION  },  // DMA Channel 7
    };

    ExternalInt ExternalInt0(0);
    ExternalInt ExternalInt1(1);
    ExternalInt ExternalInt2(2);
    ExternalInt ExternalInt3(3);
    ExternalInt ExternalInt4(4);

    InterruptMask ExternalInt::Ien[] =
    {
      {  &ifsv[0][0], &iecv[0][0], _IFS0_INT0IF_MASK  },  // External Interrupt 0
      {  &ifsv[0][0], &iecv[0][0], _IFS0_INT1IF_MASK  },  // External Interrupt 1
      {  &ifsv[0][0], &iecv[0][0], _IFS0_INT2IF_MASK  },  // External Interrupt 2
      {  &ifsv[0][0], &iecv[0][0], _IFS0_INT3IF_MASK  },  // External Interrupt 3
      {  &ifsv[0][0], &iecv[0][0], _IFS0_INT4IF_MASK  },  // External Interrupt 4
    };

    InterruptVector ExternalInt::IntVec[] =
    {
      {  &ipcv[0][0], _IPC0_INT0IS_POSITION, _IPC0_INT0IP_POSITION  },  // External Interrupt 0
      {  &ipcv[2][0], _IPC2_INT1IS_POSITION, _IPC2_INT1IP_POSITION  },  // External Interrupt 1
      {  &ipcv[3][0], _IPC3_INT2IS_POSITION, _IPC3_INT2IP_POSITION  },  // External Interrupt 2
      {  &ipcv[4][0], _IPC4_INT3IS_POSITION, _IPC4_INT3IP_POSITION  },  // External Interrupt 3
      {  &ipcv[5][0], _IPC5_INT4IS_POSITION, _IPC5_INT4IP_POSITION  },  // External Interrupt 4
    };

    InterruptMask Adc::Ien[] =
    {
      {   &ifsv[1][0], &iecv[1][0], _IFS1_ADCD0IF_MASK   },
      {   &ifsv[1][0], &iecv[1][0], _IFS1_ADCD1IF_MASK   },
      {   &ifsv[1][0], &iecv[1][0], _IFS1_ADCD2IF_MASK   },
      {   &ifsv[1][0], &iecv[1][0], _IFS1_ADCD3IF_MASK   },
      {   &ifsv[1][0], &iecv[1][0], _IFS1_ADCD4IF_MASK   },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD5IF_MASK   },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD6IF_MASK   },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD7IF_MASK   },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD8IF_MASK   },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD9IF_MASK   },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD10IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD11IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD12IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD13IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD14IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD15IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD16IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD17IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD18IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD19IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD20IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD21IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD22IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD23IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD24IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD25IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD26IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD27IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD28IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD29IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD30IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD31IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD32IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD33IF_MASK  },
      {   &ifsv[2][0], &iecv[2][0], _IFS2_ADCD34IF_MASK  },
      {   &ifsv[3][0], &iecv[3][0], _IFS3_ADCD43IF_MASK  },
      {   &ifsv[3][0], &iecv[3][0], _IFS3_ADCD44IF_MASK  },
    };

    InterruptVector Adc::IntVec[] =
    {
      {  &ipcv[14][0], _IPC14_ADCD0IS_POSITION , _IPC14_ADCD0IP_POSITION   },
      {  &ipcv[15][0], _IPC15_ADCD1IS_POSITION , _IPC15_ADCD1IP_POSITION   },
      {  &ipcv[15][0], _IPC15_ADCD2IS_POSITION , _IPC15_ADCD2IP_POSITION   },
      {  &ipcv[15][0], _IPC15_ADCD3IS_POSITION , _IPC15_ADCD3IP_POSITION   },
      {  &ipcv[15][0], _IPC15_ADCD4IS_POSITION , _IPC15_ADCD4IP_POSITION   },
      {  &ipcv[16][0], _IPC16_ADCD5IS_POSITION , _IPC16_ADCD5IP_POSITION   },
      {  &ipcv[16][0], _IPC16_ADCD6IS_POSITION , _IPC16_ADCD6IP_POSITION   },
      {  &ipcv[16][0], _IPC16_ADCD7IS_POSITION , _IPC16_ADCD7IP_POSITION   },
      {  &ipcv[16][0], _IPC16_ADCD8IS_POSITION , _IPC16_ADCD8IP_POSITION   },
      {  &ipcv[17][0], _IPC17_ADCD9IS_POSITION , _IPC17_ADCD9IP_POSITION   },
      {  &ipcv[17][0], _IPC17_ADCD10IS_POSITION, _IPC17_ADCD10IP_POSITION  },
      {  &ipcv[17][0], _IPC17_ADCD11IS_POSITION, _IPC17_ADCD11IP_POSITION  },
      {  &ipcv[17][0], _IPC17_ADCD12IS_POSITION, _IPC17_ADCD12IP_POSITION  },
      {  &ipcv[18][0], _IPC18_ADCD13IS_POSITION, _IPC18_ADCD13IP_POSITION  },
      {  &ipcv[18][0], _IPC18_ADCD14IS_POSITION, _IPC18_ADCD14IP_POSITION  },
      {  &ipcv[18][0], _IPC18_ADCD15IS_POSITION, _IPC18_ADCD15IP_POSITION  },
      {  &ipcv[18][0], _IPC18_ADCD16IS_POSITION, _IPC18_ADCD16IP_POSITION  },
      {  &ipcv[19][0], _IPC19_ADCD17IS_POSITION, _IPC19_ADCD17IP_POSITION  },
      {  &ipcv[19][0], _IPC19_ADCD18IS_POSITION, _IPC19_ADCD18IP_POSITION  },
      {  &ipcv[19][0], _IPC19_ADCD19IS_POSITION, _IPC19_ADCD19IP_POSITION  },
      {  &ipcv[19][0], _IPC19_ADCD20IS_POSITION, _IPC19_ADCD20IP_POSITION  },
      {  &ipcv[20][0], _IPC20_ADCD21IS_POSITION, _IPC20_ADCD21IP_POSITION  },
      {  &ipcv[20][0], _IPC20_ADCD22IS_POSITION, _IPC20_ADCD22IP_POSITION  },
      {  &ipcv[20][0], _IPC20_ADCD23IS_POSITION, _IPC20_ADCD23IP_POSITION  },
      {  &ipcv[20][0], _IPC20_ADCD24IS_POSITION, _IPC20_ADCD24IP_POSITION  },
      {  &ipcv[21][0], _IPC21_ADCD25IS_POSITION, _IPC21_ADCD25IP_POSITION  },
      {  &ipcv[21][0], _IPC21_ADCD26IS_POSITION, _IPC21_ADCD26IP_POSITION  },
      {  &ipcv[21][0], _IPC21_ADCD27IS_POSITION, _IPC21_ADCD27IP_POSITION  },
      {  &ipcv[21][0], _IPC21_ADCD28IS_POSITION, _IPC21_ADCD28IP_POSITION  },
      {  &ipcv[22][0], _IPC22_ADCD29IS_POSITION, _IPC22_ADCD29IP_POSITION  },
      {  &ipcv[22][0], _IPC22_ADCD30IS_POSITION, _IPC22_ADCD30IP_POSITION  },
      {  &ipcv[22][0], _IPC22_ADCD31IS_POSITION, _IPC22_ADCD31IP_POSITION  },
      {  &ipcv[22][0], _IPC22_ADCD32IS_POSITION, _IPC22_ADCD32IP_POSITION  },
      {  &ipcv[23][0], _IPC23_ADCD33IS_POSITION, _IPC23_ADCD33IP_POSITION  },
      {  &ipcv[23][0], _IPC23_ADCD34IS_POSITION, _IPC23_ADCD34IP_POSITION  },
      {  &ipcv[25][0], _IPC25_ADCD43IS_POSITION, _IPC25_ADCD43IP_POSITION  },
      {  &ipcv[25][0], _IPC25_ADCD44IS_POSITION, _IPC25_ADCD44IP_POSITION  },
    };

    //struct Adc Adc1;
  }

#else
#error "Select processor"
#endif

  ::pic::InterruptMask CoreTimer::Ien[] =
  {
    {   &pic::ifsv[0][0],  &pic::iecv[0][0],  _IFS0_CTIF_MASK        },  // Core Timer Interrupt
  };

  ::pic::InterruptVector CoreTimer::IntVec[] =
  {
    {   &pic::ipcv[0][0],  _IPC0_CTIS_POSITION,    _IPC0_CTIP_POSITION     },  // Core Timer Interrupt
  };



/* Standard (R4000) cache operations. Taken from "MIPS R4000 Microprocessor User's Manual" 2nd edition: */
#define CACHE_I         (0)     /* primary instruction */
#define CACHE_D         (1)     /* primary data */
#define CACHE_SI        (2)     /* secondary instruction */
#define CACHE_SD        (3)     /* secondary data (or combined instruction/data) */

#define INDEX_INVALIDATE                (0)     /* also encodes WRITEBACK if CACHE_D or CACHE_SD */
#define INDEX_LOAD_TAG                  (1)
#define INDEX_STORE_TAG                 (2)
#define CREATE_DIRTY_EXCLUSIVE          (3)     /* CACHE_D and CACHE_SD only */
#define HIT_INVALIDATE                  (4)
#define CACHE_FILL                      (5)     /* CACHE_I only */
#define HIT_WRITEBACK_INVALIDATE        (5)     /* CACHE_D and CACHE_SD only */
#define HIT_WRITEBACK                   (6)     /* CACHE_I, CACHE_D and CACHE_SD only */
#define HIT_SET_VIRTUAL                 (7)     /* CACHE_SI and CACHE_SD only */

#define BUILD_CACHE_OP(o,c)             (((o) << 2) | (c))

/* Individual cache operations: */

#define INDEX_INVALIDATE_I              BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_I)
#define INDEX_WRITEBACK_INVALIDATE_D    BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_D)
#define INDEX_INVALIDATE_SI             BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_SI)
#define INDEX_WRITEBACK_INVALIDATE_SD   BUILD_CACHE_OP(INDEX_INVALIDATE,CACHE_SD)

#define INDEX_LOAD_TAG_I                BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_I)
#define INDEX_LOAD_TAG_D                BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_D)
#define INDEX_LOAD_TAG_SI               BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_SI)
#define INDEX_LOAD_TAG_SD               BUILD_CACHE_OP(INDEX_LOAD_TAG,CACHE_SD)

#define INDEX_STORE_TAG_I               BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_I)
#define INDEX_STORE_TAG_D               BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_D)
#define INDEX_STORE_TAG_SI              BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_SI)
#define INDEX_STORE_TAG_SD              BUILD_CACHE_OP(INDEX_STORE_TAG,CACHE_SD)

#define CREATE_DIRTY_EXCLUSIVE_D        BUILD_CACHE_OP(CREATE_DIRTY_EXCLUSIVE,CACHE_D)
#define CREATE_DIRTY_EXCLUSIVE_SD       BUILD_CACHE_OP(CREATE_DIRTY_EXCLUSIVE,CACHE_SD)


#define HIT_INVALIDATE_I                BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_I)
#define HIT_INVALIDATE_D                BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_D)
#define HIT_INVALIDATE_SI               BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_SI)
#define HIT_INVALIDATE_SD               BUILD_CACHE_OP(HIT_INVALIDATE,CACHE_SD)

#define CACHE_FILL_I                    BUILD_CACHE_OP(CACHE_FILL,CACHE_I)

#define HIT_WRITEBACK_INVALIDATE_D      BUILD_CACHE_OP(HIT_WRITEBACK_INVALIDATE,CACHE_D)
#define HIT_WRITEBACK_INVALIDATE_SD     BUILD_CACHE_OP(HIT_WRITEBACK_INVALIDATE,CACHE_SD)

#define HIT_WRITEBACK_I                 BUILD_CACHE_OP(HIT_WRITEBACK,CACHE_I)
#define HIT_WRITEBACK_D                 BUILD_CACHE_OP(HIT_WRITEBACK,CACHE_D)
#define HIT_WRITEBACK_SD                BUILD_CACHE_OP(HIT_WRITEBACK,CACHE_SD)

#define HIT_SET_VIRTUAL_SI              BUILD_CACHE_OP(HIT_SET_VIRTUAL,CACHE_SI)
#define HIT_SET_VIRTUAL_SD              BUILD_CACHE_OP(HIT_SET_VIRTUAL,CACHE_SD)


#if __XC32_VERSION__
// idiotyzmy z xc32
extern "C" int open( const char *, int, int ) {  return -1;  }
#warning "tu xc32 bredzi: #pragma message: Warning - Using _cache(op,addr) on an uncached target device"
#endif

namespace pic
{
  void __attribute__((nomips16)) cacheFlushData( const void *_addr, size_t n )
  {
    char *addr = (char *)_addr;
    char *eaddr = ((char *)_addr)+n;

    // WriteBack and Invalidate RAM entries in the cache
    while( addr < eaddr )
    {
      __builtin_mips_cache( HIT_WRITEBACK_D, addr );
      addr += 16;
    }
  }

  void __attribute__((nomips16)) cacheInvalidateData( const void *_addr, size_t n )
  {
    char *addr = (char *)_addr;
    char *eaddr = ((char *)_addr)+n;

    // WriteBack and Invalidate RAM entries in the cache
    while( addr < eaddr )
    {
      __builtin_mips_cache( HIT_INVALIDATE_D, addr );
      addr += 16;
    }
  }

#if __PIC32MZ__
  extern uint32_t vectorsOffsets[256];
#endif

  void setupVectorsOffsets()
  {
#if __PIC32MZ__
    for( unsigned n=0; n<256; ++n )
      ::pic::vectorsOffsets[n] = 0x200U + 8*n;
#endif
  }

}

#if !__PIC32MZ__
uint32_t __attribute__((nomips16)) _ReadCoreTimer()
{
  uint32_t v;
  asm volatile( "mfc0 %0, $9" : "=r"(v) );
  return v;
}
#endif
