#ifndef __UART_H__
#define __UART_H__

#include <string>
#include <memory>

struct Uart
{
  struct State;

  int fd=-1;

  std::shared_ptr<State> open( int _fd );
  void open( std::string dev );
  void close();

  void setbaud( unsigned baudrate );

  std::shared_ptr<State> getState();
  void setState( std::shared_ptr<State> );

  void setRaw();

  bool getCTS();
  bool getDSR();

  void setCrNl();
  void setBrk0();

  void setRS( int flag, bool v );

  char readC( unsigned timeout=5 );
  void writeC( char c );
  void writeS( std::string );
  void accept( char c );

  template <typename T>
  void putN( T v )
  {
    for( uint8_t i=0; i<sizeof(T); ++i )
    {
      writeC( (char)(uint8_t)(v&0xFFU) );
      v >>= 8;
    }
  }

  template <typename T>
  T getN()
  {
    T v=0;
    for( uint8_t i=0; i<sizeof(T); ++i )
    {
      v >>= 8;
      T c = (T)(uint8_t)readC();
      v |= c << ((sizeof(T)-1)*8);
    }
    return v;
  }
};

#endif
