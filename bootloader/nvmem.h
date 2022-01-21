#ifndef __NVMEM_H__
#define __NVMEM_H__

#define KVA_TO_PA(v)    ((uint32_t)(v) & 0x1fffffff)
#define PA_TO_KVA0(pa)  ((void *) ((pa) | 0x80000000))
#define PA_TO_KVA1(pa)  ((void *) ((pa) | 0xa0000000))

void __attribute__((nomips16)) NVMemStartOperation(unsigned nvmop);
bool __attribute__((nomips16)) NVMemWaitOperation();
bool __attribute__((nomips16)) NVMemOperation(unsigned nvmop);
void __attribute__((nomips16)) NVMemStartErasePage(void* address);
bool __attribute__((nomips16)) NVMemWriteRow(void* address, void* data);

#endif
