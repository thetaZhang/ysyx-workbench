#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include <cstdint>

#define PMEM_SIZE (128 * 1024 * 1024)

extern uint32_t pmem[];

uint32_t pmem_read(uint32_t addr, int len);

#endif