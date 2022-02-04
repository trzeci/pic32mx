#include "uart.h"
#include "boot.h"
#include "../bootloader/crc16.h"
#include "readHex.h"

#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <errno.h>


void BootMgr::open( std::string dev )
{
  if( fd != -1 )
    close();
  Uart::open(dev);
  bootEnable();
}

void BootMgr::close()
{
  if( fd != -1 && bootloader_mode )
    bootDisable();
  if( fd != -1 )
  {
    ::tcflush( fd, TCIOFLUSH );
    ::close( fd );
    fd = -1;
  }
}

BootMgr::~BootMgr()
{
  close();
}


// VBB
// SBBBB
// PBBBB....

static void dsleep(size_t d){  ::usleep(d*10000);  }

#define RS_RESET TIOCM_DTR
#define RS_EN    TIOCM_RTS
#define POL_RESET true
#define POL_EN    true

void BootMgr::bootDisable()
{
  bootloader_mode = false;
  setRS( RS_RESET, POL_RESET  );
  dsleep(1);
  setRS( RS_EN,    !POL_EN     );
  dsleep(2);
  setRS( RS_RESET, !POL_RESET );
  dsleep(4);
}

void BootMgr::bootEnable()
{
  setRS( RS_RESET, POL_RESET  );
  dsleep(1);
  setRS( RS_EN,    POL_EN    );
  dsleep(5);
  tcflush( fd, TCIOFLUSH );
  setRS( RS_RESET, !POL_RESET );
  dsleep(1);
  accept( '$' );
  puts( "Prompt" );
  writeC( 'L' );
  bootloader_mode = true;
  accept( 'L' );
  puts( "Set to bootload" );
  accept( '$' );
  writeC( 'V' );
  {
    version = (uint8_t)readC();
    printf( "Bootloader version %u\n", version );
    page_size = 1UL << (uint8_t)readC();
    printf( "device page size %zu\n", page_size );
    accept( '$' );
  }
  dsleep(3);
}

void BootMgr::logEnable()
{
  setRS( RS_EN, POL_EN );
}

void BootMgr::bootLog()
{
  Uart log;
  auto log_state = log.open(0);
  log.setRaw();
  log.setCrNl();
  //log.setBrk0();
  usleep(100000);
  for(;;)
  {
    fd_set rd_set;
    FD_SET( 0,  &rd_set );
    FD_SET( fd, &rd_set );
    struct timeval tv = { 1, 0 };
    int rv = select( fd+1, &rd_set, 0, 0, &tv );
    if( rv == -1 )
    {
      throw std::string("select:") + strerror(errno);
    }
    if( FD_ISSET( fd, &rd_set ) )
    {
      char c;
      int rv = read( fd, &c, 1 );
      if( rv == 1 )
      {
        putchar(c);
        fflush(stdout);
      }
      else
      if( rv == 0 )
      {
        puts("EOF");
        break;
      }
      else
      if( errno == EAGAIN )
      {
        ;
      }
      else
      {
        throw std::string("read:") + strerror(errno);
      }
    }
    if( FD_ISSET( 0, &rd_set ) )
    {
      char c;
      int rv = read( 0, &c, 1 );
      if( rv == 1 )
      {
        switch(c)
        {
          case 0:   // break
          case 3:   // ctrl-c
          case 4:   // ctrl-d
          case '\x1c': // ctrl-backslash
            puts( "leaving" );
            c = 0;
            break;
          default:
            write( fd, &c, 1 );
            break;
        }
        if( !c )
          break;
      }
      else
      if( rv == 0 )
      {
        puts("input EOF");
        break;
      }
      else
      if( errno == EAGAIN )
      {
        ;
      }
      else
      {
        puts( strerror(errno) );
        break;
      }
    }
  }
  log.setState( log_state );
}


void BootMgr::loadFile( std::string filename )
{
  MemMap m;
  m.readHex( filename );
  m.dump();
  auto pages = m.toPages(page_size);
  unsigned npage=1;
  for( auto page : pages )
  {
    printf( "boot-flash:\t%3u/%3u @%08X %u\r", npage, (unsigned)pages.size(), (uint32_t)page.first, (uint32_t)page.second.size() ); fflush( stdout );
    writeC('P');
    putN<uint32_t>( page.first ); // page address
    accept('A');
    writeS( page.second );
    ++npage;

    uint16_t crc = crc16( (const unsigned char *)page.second.data(), page.second.size() );
    accept('C');
    uint16_t crc2 = getN<uint16_t>();
    accept('$');

    if( crc != crc2 )
    {
      fprintf( stderr, "boot-flash: crc mismatch %04X:%04X %zu@x%08X\n", crc, crc2, page.second.size(), (unsigned)page.first );
      dumpPage( page.second );
      throw std::string("CRC");
    }
  }
  puts("");
}


void BootMgr::changeSpeed( uint32_t spd )
{
  writeC( 'S' );
  putN<uint32_t>( spd );
  dsleep(2);
  Uart::setbaud( spd );
  ::tcflush( fd, TCIOFLUSH );
  accept('$'); // bootloader sends this after 100ms
  printf( "Speed set to %u baud\n", spd );
}
