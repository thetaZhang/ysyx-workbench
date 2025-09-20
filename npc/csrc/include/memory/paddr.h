#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include <common.h>
#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))


#define MSIZE 0x80000000
#define MBASE 0x80000000

#define PMEM_LEFT  ((paddr_t)MBASE)
#define PMEM_RIGHT ((paddr_t)MBASE + MSIZE - 1)
#define RESET_VECTOR (PMEM_LEFT)

void init_mem();


uint8_t* guest_to_host(paddr_t paddr);
paddr_t host_to_guest(uint8_t *haddr);

static inline bool in_pmem(paddr_t addr) {
  return addr - MBASE < MSIZE;
}


extern "C" word_t pmem_read(paddr_t addr);
extern "C" void pmem_write(paddr_t addr, word_t data, uint8_t mask);

#endif
