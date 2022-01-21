#include "readHex.h"


uint8_t MemMap::hex2int( char h )
{
  if( h >= 'a' )
    return h - 'a' + 10;
  else
  if( h >= 'A' )
    return h - 'A' + 10;
  else
    return h - '0';
}


uint8_t MemMap::hex2int( const char *h )
{
  return hex2int(h[0])*16 + hex2int(h[1]);
}


void MemMap::push( size_t address, std::string data )
{
  //std::cout << "push " << "@"+int_to_hex(address)+"+"+int_to_hex(data.size()) << std::endl;
  if( contents.empty() )
  {
    contents[ address ] = data;
  }
  else
  {
    auto it = contents.lower_bound( address );
    auto prev = ( it == contents.begin() ? contents.end() : std::prev( it ) );
    if( it != contents.end() )
    {
      if( address + data.size() > it->first )
        throw std::string( "overlapping regions " ) + "@"+int_to_hex(address)+"+"+int_to_hex(data.size()) + " and " + "@"+int_to_hex(it->first);
      if( address + data.size() == it->first )
      {
        data.append( it->second );
        contents.erase( it );
      }
    }
    if( prev != contents.end() )
    {
      if( prev->first + prev->second.size() > address )
        throw std::string( "overlapping regions" ) + "@"+int_to_hex(prev->first)+"+"+int_to_hex(prev->second.size()) + " and " + "@"+int_to_hex(address);
      if( prev->first + prev->second.size() == address )
        prev->second.append( data );
      else
        contents.emplace( address, data );
    }
    else
      contents.emplace( address, data );
  }
}


void MemMap::dump()
{
  for( auto p : contents )
  {
    std::cout << "  data " << "@"+int_to_hex((uint32_t)p.first)+"+"+int_to_hex((uint32_t)p.second.size()) << std::endl;
  }
}


void MemMap::readHex( std::string fname )
{
  FILE *f = fopen( fname.c_str(), "r" );
  if( !f )
    perror( fname.c_str() ), exit( 1 );
  readHex( f );
  ::fclose( f );
}


void MemMap::readHex( FILE *f )
{
  size_t ext_lin_addr=0;
  size_t ext_seg_addr=0;
  char buf[256]={'\0',};
  size_t line=0;
  while( fgets( buf, sizeof(buf), f ) )
  {
    ++ line;
    //printf( "[%s]\n", buf );
    if( buf[0] != ':' )
      throw std::string( "HEX file format error - missing ':'" );
    std::basic_string<uint8_t> x;
    for( char *p=buf+1; std::isxdigit(p[0]) && std::isxdigit(p[1]); p+=2 )
      x += hex2int( p );
    size_t chars = x[0];
    uint8_t crc = x[0];
    size_t address = (size_t)x[1]*256U+(size_t)x[2]; // 2 bytes
    crc += x[1];
    crc += x[2];
    uint8_t record_type = x[3];
    crc += x[3];
    std::string sline;
    for( size_t i=0; i<chars; i++ )
    {
      sline += x[4+i];
      crc += x[4+i];
    }
    crc += x[4+chars];
    if( crc )
      throw std::string( "crc error in hex file:" ) + std::to_string( (unsigned)crc );
    if( record_type == 0 ) // dane
    {
      push( ext_lin_addr + ext_seg_addr + address, sline );
      //printf( "DATA@%x %zu\n", ext_lin_addr + ext_seg_addr + address, sline.length() );
    }
    else
    if( record_type == 1 ) // koniec
      break;
    else
    if( record_type == 2 ) // ext seg addr
    {
      ext_seg_addr = ((size_t)((uint8_t)sline[0])*256U+(size_t)((uint8_t)sline[1]))<<8;
      ext_lin_addr = 0;
      //printf("SEG:%x\n",ext_seg_addr);
    }
    else
    if( record_type == 3 ) // start address
      ; // ignore
    else
    if( record_type == 4 ) // ext lin addr
    {
      ext_lin_addr = ((size_t)((uint8_t)sline[0])*256U+(size_t)((uint8_t)sline[1]))<<16;
      ext_seg_addr = 0;
      //printf("LIN:%x\n",ext_lin_addr);
    }
    else
    if( record_type == 5 ) // entry point
      ;
    else
      throw std::string( "unknown record type hex file:" ) + std::to_string( (long long unsigned)record_type ) + " in line:" + std::to_string(line) + "[" + buf + "]";
  }
}


std::map<size_t,std::string> MemMap::toPages( size_t page_size )
{
  std::map<size_t,std::string> p;
  std::string page;
  size_t addr = 0;
  auto c = contents;
  while( !c.empty() )
  {
    std::pair<size_t,std::string> d = *c.begin();

    if( !page.empty() && addr != d.first - ( d.first%page_size ) )
    {
      p.emplace( addr, page );
      page = std::string();
      addr = 0;
    }

    if( page.empty() )
    {
      page = std::string( page_size, '\xFF' );
      addr = d.first - ( d.first%page_size );
    }

    size_t pos=0;
    for( ; pos<d.second.size() && (d.first%page_size)+pos < page_size; ++pos )
      page[(d.first%page_size)+pos] = d.second[pos];

    if( pos < d.second.size() )
    {
      d.first += pos;
      d.second.erase(0,pos);
      c.erase( c.begin() );
      c.insert( d );
    }
    else
    {
      c.erase( c.begin() );
    }
  }
  if( !page.empty() )
    p.emplace( addr, page );
  return p;
}


std::string dumpPage( std::string p )
{
  std::stringstream s;
  size_t i=0;
  for( auto c : p )
  {
    s << int_to_hex((uint8_t)c);
    if( i%32 == 31 )
      s << std::endl;
    ++i;
  }
  return s.str();
}


#if TEST

static uint8_t hex2int( char h )
{
  if( h >= 'a' )
    return h - 'a' + 10;
  else
  if( h >= 'A' )
    return h - 'A' + 10;
  else
    return h - '0';
}

static uint8_t hex2int( const char *h )
{
  return hex2int(h[0])*16 + hex2int(h[1]);
}

template <class K,class V>
void dump( const std::map<K,V> &m )
{
  for( auto p : m )
  {
    std::cout << "  data " << "@"+int_to_hex(p.first)+"+"+int_to_hex(p.second.size()) << std::endl;
  }
}

int main( int argc, char **argv )
{
  MemMap m;
  try
  {
    m.readHex( argv[1] );
    m.dump();
    puts("");
    auto p = m.toPages( 4096 );
    dump(p);
    puts("");
  }
  catch( std::string err )
  {
    std::cout << "ERR:" << err << std::endl;
  }
  return 0;
}

#endif
