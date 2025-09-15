#include "memory/paddr.h"
#include <cstdint>
#include <assert.h>


uint32_t pmem[PMEM_SIZE / 4];

uint32_t pmem_read(uint32_t addr, int len) {
  switch (len) {
    case 1: return pmem[addr / 4] >> ((addr % 4) * 8) & 0xff;
    case 2: return pmem[addr / 4] >> ((addr % 4) * 8) & 0xffff;
    case 4: return pmem[addr / 4];
    default: assert(0);
  }
}
