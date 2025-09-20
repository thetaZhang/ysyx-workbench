#include <memory/host.h>
#include <memory/paddr.h>

#include <common.h>

static uint8_t pmem[MSIZE] = {};

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MBASE; }

extern "C" word_t pmem_read(paddr_t addr) {
  //printf("pmem_read addr = 0x%08x\n", addr);
  word_t ret;
  if (likely(in_pmem(addr))) {
    ret = host_read(guest_to_host(addr), 4);
    //printf("pmem_read ret = 0x%08x\n", ret);
  } else {
    printf("pmem_read out of bound addr = 0x%08x\n", addr);
    ret = 0;
  }
  return ret;
}

extern "C" void pmem_write(paddr_t addr, word_t data, uint8_t mask) {
  if (likely(in_pmem(addr))) {
    for (int i = 0; i < sizeof(word_t)/sizeof(uint8_t); i++) {
      if (mask & (1 << i)) {
        host_write(guest_to_host(addr + i), 1, (data >> (i * 8)) & 0xff);
      }
    }
  }
  
}

void init_mem() {
  memset(pmem, rand(), MSIZE);
}
