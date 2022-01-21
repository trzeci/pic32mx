#ifndef __READ_HEX_H__
#define __READ_HEX_H__

#include <string>
#include <map>
#include <iostream>
#include <iomanip>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


struct MemMap
{
  static uint8_t hex2int( char h );
  static uint8_t hex2int( const char *h );

  template <typename T>
  static inline std::string int_to_hex(T val, size_t width=sizeof(T)*2)
  {
      std::stringstream ss;
      ss << std::setfill('0') << std::setw(width) << std::hex << (val|0);
      return ss.str();
  }


  std::map<size_t,std::string> contents;

  void push( size_t address, std::string data );
  void dump();
  void readHex( std::string fname );
  void readHex( FILE *f );

  std::map<size_t,std::string> toPages( size_t page_size );
};


template <typename T>
static inline std::string int_to_hex(T val, size_t width=sizeof(T)*2)
{
  std::stringstream ss;
  ss << std::setfill('0') << std::setw(width) << std::hex << (val|0);
  return ss.str();
}

std::string dumpPage( std::string p );

#endif
