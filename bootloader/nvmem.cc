#include "nvmem.h"
#include <stdint.h>
#include <stddef.h>
#include <pic_hardware.h>
#include <cstring>

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

static void __attribute__((nomips16)) cacheInvalidateData( const void *_addr, size_t n )
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

static void __attribute__((nomips16)) cacheFlushData( const void *_addr, size_t n )
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


#define NVMOP_ROW_PGM           0x3      // Row program operation, PIC32MX row=512B, MZ:2kB
#define NVMOP_PAGE_ERASE        0x4      // Page erase operation, MX=4kB, MZ=16kB
#define NVMOP_NOP                 0      // NOP operation


void __attribute__((nomips16)) NVMemStartOperation( unsigned nvmop )
{
  ::pic::NVM.startOperation( nvmop );
}

bool __attribute__((nomips16)) NVMemWaitOperation()
{
  while( ::pic::NVM.isWriting() );  // Wait for WR bit to clear
  ::pic::NVM.stop();
  return ::pic::NVM.isError();
}

bool __attribute__((nomips16)) NVMemOperation( unsigned nvmop )
{
  NVMemStartOperation(nvmop);
  return NVMemWaitOperation();
}

void __attribute__((nomips16)) NVMemStartErasePage( void* address )
{
  ::pic::NVM.ADDR = KVA_TO_PA((unsigned int)address);
  NVMemStartOperation(NVMOP_PAGE_ERASE);
}


bool __attribute__((nomips16)) NVMemWriteRow( void* address, void* data )
{
  static uint32_t __attribute__((aligned(::pic::Flash::rowSize))) buffer[::pic::Flash::rowSize/4];
  memcpy( buffer, data, sizeof(buffer) );
  cacheFlushData( buffer, sizeof(buffer) );

  ::pic::NVM.ADDR = KVA_TO_PA((unsigned int)address);
  ::pic::NVM.SOURCE_ADDR = KVA_TO_PA((unsigned int)buffer);

  bool res = NVMemOperation(NVMOP_ROW_PGM);
  cacheInvalidateData( address, ::pic::Flash::rowSize );
  return res;
}
