#include "uart.h"

#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <asm-generic/termbits.h>
#include <sys/ioctl.h>
#include <unistd.h>


void Uart::setRS( int flag, bool v )
{
  ioctl( fd, (v?TIOCMBIS:TIOCMBIC), &flag );//Set RTS/DTR pin
}

struct Uart::State : public termios2 {};

std::shared_ptr<Uart::State> Uart::getState()
{
  std::shared_ptr<Uart::State> p = std::make_shared<Uart::State>();
  if( ioctl( fd, TCGETS2, (termios2*)p.get() ) )
    throw std::string("TCGETS2:") + strerror(errno);
  return p;
}

void Uart::setState( std::shared_ptr<Uart::State> p )
{
  if( ioctl( fd, TCSETS2, (termios2*)p.get() ) )
    throw std::string("TCSETS2:") + strerror(errno);
}


void Uart::setRaw()
{
  auto opts = getState();

  opts->c_iflag = IGNBRK;
  opts->c_lflag = 0;
  opts->c_oflag = 0;
  opts->c_cflag = CREAD | CLOCAL | CS8 | CSTOPB | B115200;

  opts->c_cc[VTIME] = 0;
  opts->c_cc[VMIN]  = 1;

  setState( opts );
}

void Uart::setCrNl()
{
  auto opts = getState();
  opts->c_oflag |= OPOST | ONLCR;
  setState( opts );
}

void Uart::setBrk0()
{
  auto opts = getState();
  opts->c_iflag &= ~( IGNBRK | BRKINT );
  setState( opts );
}


bool Uart::getCTS()
{
  int s=0;
  ioctl( fd, TIOCMGET, &s );
  return !!(s&TIOCM_CTS);
}

bool Uart::getDSR()
{
  int s=0;
  ioctl( fd, TIOCMGET, &s );
  return !!(s&TIOCM_DSR);
}


static int serial_translate_baud( unsigned inrate )
{
  switch( inrate )
  {
    case       0: return       B0;
    case     300: return     B300;
    case    1200: return    B1200;
    case    2400: return    B2400;
    case    4800: return    B4800;
    case    9600: return    B9600;
    case   19200: return   B19200;
    case   38400: return   B38400;
    case   57600: return   B57600;
    case  115200: return  B115200;
    case  230400: return  B230400;
    case  460800: return  B460800;
    case  500000: return  B500000;
    case 1000000: return B1000000;
    case 1500000: return B1500000;
    case 2000000: return B2000000;
    case 2500000: return B2500000;
    case 3000000: return B3000000;
    case 3500000: return B3500000;
    case 4000000: return B4000000;
    default: return -1; // do custom divisor
  }
}


void Uart::setbaud( unsigned baudrate )
{
  int baudratecode = serial_translate_baud(baudrate);
  struct termios2 t;
  if( ioctl( fd, TCGETS2, &t ) )
    throw std::string("TCGETS2:") + strerror(errno);
  if( baudratecode>0 )
  {
    t.c_ispeed = t.c_ospeed = 0;
    t.c_cflag &= ~CBAUD;
    t.c_cflag |= baudratecode;
  }
  else
  {
    t.c_ispeed = t.c_ospeed = baudrate;
    t.c_cflag &= ~CBAUD;
    t.c_cflag |= BOTHER;
  }
  if( ioctl( fd, TCSETS2, &t ) )
    throw std::string("TCSETS2:") + strerror(errno);
}


std::shared_ptr<Uart::State> Uart::open( int _fd )
{
  fd = _fd;
  auto p = getState();
  try
  {
    setRaw();
    setbaud( 115200 );
  }
  catch(...)
  {
    fd = -1;
    throw;
  }
  return p;
}

void Uart::open( std::string dev )
{
  int f = ::open( dev.c_str(), O_RDWR | O_NDELAY | O_NOCTTY );
  if( f < 0 )
    throw std::string(dev) + ":" + strerror(errno);
  open( f );
}


void Uart::close()
{
  if( fd != -1 )
  {
    ::close( fd );
    fd = -1;
  }
}


char Uart::readC( unsigned timeout )
{
  struct timeval tv = { (time_t)timeout, 0 };
  int i;
  char c=0;

  {
    fd_set fr;
    FD_ZERO( &fr );
    FD_SET( fd, &fr );
    i = select( fd+1, &fr, NULL, NULL, &tv );
  }

  if( i < 0 )
    throw std::string("select:") + strerror(errno);
  else
  if( !i )
    throw std::string("timeout");
  else
  {
    i = ::read( fd, &c, 1 );
    if( i <= 0 )
      throw std::string("read:") + strerror(errno);
    //printf( "gotchar %c %02x\n",*c , (unsigned)(*c));
    return c;
  }
}

void Uart::accept( char c )
{
  char c2;
  while( (c2=readC()) != c )
    ;//printf("accept %c: got %c(%u)\n",c,c2,(unsigned)(uint8_t)c2);
}

void Uart::writeC( char c )
{
  ::write( fd, &c, 1 );
}

void Uart::writeS( std::string _s )
{
  const char *s = _s.data();
  size_t l = _s.size();
  while(l)
  {
    int n = ::write( fd, s, l>512 ? 512 : l );
    //printf("write %d/%zu %d\n",n,l,errno);
    if( n < 0 )
    {
      if( errno == EAGAIN )
        ::usleep(1000);
      else
        throw std::string("write:") + strerror(errno);
    }
    else
    {
      s += n;
      l -= n;
    }
  }
}
