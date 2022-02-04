#ifndef __BOOT_H__
#define __BOOT_H__


#include "uart.h"


struct BootMgr : public Uart
{
  ~BootMgr();

  bool bootloader_mode = false;
  uint8_t version=0;
  size_t page_size=0;

  void open( std::string dev = "/dev/ttyUSB0" );
  void close();

  void changeSpeed( uint32_t spd );

  void bootDisable();
  void bootEnable();

  void logEnable();
  void bootLog();

  void loadFile( std::string filename );
};


#endif
