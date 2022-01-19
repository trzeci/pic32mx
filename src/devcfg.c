#include <stdint.h>

struct DEVCFG
{
  struct
  {
    uint32_t USERID:16;
    uint32_t FSRSSEL:3;
    uint32_t d1:5;
    uint32_t FMIIEN:1;
    uint32_t FETHIO:1;
    uint32_t FCANIO:1;
    uint32_t d2:3;
    uint32_t FUSBIDIO:1;
    uint32_t FVBUSIO:1;
  } devcfg3;
  struct
  {
    uint32_t FPLLIDIV:3;
    uint32_t d1:1;
    uint32_t FPLLMUL:3;
    uint32_t d2:1;
    uint32_t UPLLIDIV:3;
    uint32_t d3:4;
    uint32_t UPLLEN:1;
    uint32_t FPLLODIV:3;
    uint32_t d4:13;
  } devcfg2;
  struct
  {
    uint32_t FNOSC:3;
    uint32_t d1:2;
    uint32_t FSOSCEN:1;
    uint32_t d2:1;
    uint32_t IESO:1;
    uint32_t POSCMOD:2;
    uint32_t OSCIOFNC:1;
    uint32_t d3:1;
    uint32_t FPBDIV:2;
    uint32_t FCKSM:2;
    uint32_t WDTPS:5;
    uint32_t d4:2;
    uint32_t FWDTEN:1;
    uint32_t d5:8;
  } devcfg1;
  struct
  {
    uint32_t DEBUG:2;
    uint32_t d1:1;
    uint32_t ICESEL:1;
    uint32_t d2:8;
    uint32_t PWP:8;
    uint32_t d3:4;
    uint32_t BWP:1;
    uint32_t d4:3;
    uint32_t CP:1;
    uint32_t d5:2;
    uint32_t ZERO:1;
  } devcfg0;
} DEVCFG __attribute__((section(".devcfg"),used)) =
{
  // ones
  .devcfg3.d1=0x1FU,
  .devcfg3.d2=0x07U,

  .devcfg3.USERID   = 0xFFFFU, // USERID
  .devcfg3.FSRSSEL  = 7, // SRS assigned to priority 7
  .devcfg3.FMIIEN   = 1, // MII enabled
  .devcfg3.FETHIO   = 1, // default ETH pins
  .devcfg3.FCANIO   = 1, // default CAN IO pins
  .devcfg3.FUSBIDIO = 0, // USBID pin   is port function
  .devcfg3.FVBUSIO  = 0, // USB VBUS_ON is port function

  // ones
  .devcfg2.d1=1,
  .devcfg2.d2=1,
  .devcfg2.d3=0xFU,
  .devcfg2.d4=0x1FFFU,

  // XTAL 8Mhz /2 *20 = 80MHz Fcy
  .devcfg2.FPLLIDIV  = 1, // PLL input div by 2
  .devcfg2.FPLLMUL   = 5, // PLL mult by 20
  .devcfg2.UPLLIDIV  = 7, // USB PLL input div by 12
  .devcfg2.UPLLEN    = 0, // disable USB PLL
  .devcfg2.FPLLODIV  = 0, // PLL out div by 1

  // ones
  .devcfg1.d1=3,
  .devcfg1.d2=1,
  .devcfg1.d3=1,
  .devcfg1.d4=3,
  .devcfg1.d5=0xFFU,

  .devcfg1.FNOSC    = 3,  // Primary oscillator with PLL
  .devcfg1.FSOSCEN  = 0,  // Secondary oscillator disable
  .devcfg1.IESO     = 0,  // two-spped startup disabled
  .devcfg1.POSCMOD  = 1,  // 0=ext, 1=XT, 2=HS, 3=disabled
  .devcfg1.OSCIOFNC = 0,  // CLKO disabled
  .devcfg1.FPBDIV   = 1,  // Fpb = Fcy/2
  .devcfg1.FCKSM    = 3,  // clock switching disabled
  .devcfg1.WDTPS    = 0x1FU,  // WDT 1:1048576
  .devcfg1.FWDTEN   = 0,  // WDT disabled

  // ones
  .devcfg0.d1=1,
  .devcfg0.d2=0xFFU,
  .devcfg0.d3=0xFU,
  .devcfg0.d4=0x7U,
  .devcfg0.d5=3,

  .devcfg0.ZERO=0,    // DEVCFG valid
  .devcfg0.CP=1,      // no code protection
  .devcfg0.BWP=1,     // no write protexction over bootloader
  .devcfg0.PWP=0xFFU, // no write protexction over code
  .devcfg0.ICESEL=1,  // debugger on PGC2/PGD2
  .devcfg0.DEBUG=3,   // no debug
};
