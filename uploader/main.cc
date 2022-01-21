#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>

#include "uart.h"
#include "boot.h"


unsigned speed = 0;
bool _log = false;
std::string hexfile;


void help( const char *argv0 )
{
  fputs( argv0, stderr );
  fputs("\n"
        "\t [--115200|--500kbit|--1Mbit]\n"
        "\t  --load <file.hex>\n"
        , stderr );
  throw std::string("");
}


int main( int argc, char const * argv[] )
{
  try
  {
    if(argc < 2)
      help(argv[0]);

    setlinebuf(stdout);
    setlinebuf(stderr);

    int arg = 1;
    while(arg < argc)
    {
      if( !strncmp(argv[arg], "--", 2 ) )
      {
        if(!strcmp(argv[arg], "--500kbit"))
        {
          speed = 500000;
          puts("500kbit");
        }
        else
        if(!strcmp(argv[arg], "--1Mbit"))
        {
          speed = 1000000;
          puts("1Mbit");
        }
        else
        if( isdigit(argv[arg][2]) )
        {
          speed = ::atol( argv[arg]+2 );
          puts("argv[arg]+2");
        }
        else
        if(!strcmp(argv[arg], "--38400"))
        {
          speed = 38400;
          puts("38400");
        }
        else
        if(!strcmp(argv[arg], "--115200"))
        {
          speed = 115200;
          puts("115200");
        }
        else
        if(!strcmp(argv[arg], "--log"))
        {
          _log = true;
          puts("with logger");
        }
        else
        {
          help(argv[0]);
        }
      }
      else
      {
        if( hexfile.empty() )
          hexfile = argv[arg];
        else
          help(argv[0]);
      }
      arg++;
    }

    BootMgr bus;
    bus.open();
    bus.bootEnable();

    if( speed )
      bus.changeSpeed( speed );

    printf( "Uploading file:\t%s\n", hexfile.c_str() );
    bus.loadFile( hexfile );

    if( _log )
    {
      bus.bootDisable();
      bus.logEnable();
      puts("log ...");
      bus.bootLog();
    }

    if( bus.fd >= 0 )
    {
      bus.bootDisable();
      bus.close();
    }

    return 0;
  }
  catch( std::string s )
  {
    printf( "%s\n", s.c_str() );
    return 1;
  }
}

