#include <memory/host.h>
#include <memory/paddr.h>

#include <common.h>

static uint8_t pmem[MSIZE] = {};

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MBASE; }

static word_t pmem_read(paddr_t addr) {
  word_t ret = host_read(guest_to_host(addr), 4);
  return ret;
}

static void pmem_write(paddr_t addr, word_t data, uint8_t mask) {
  for (int i = 0; i < 4; i++) {
    if (mask & (1 << i)) {
      host_write(guest_to_host(addr + i), 1, (data >> (i * 8)) & 0xff);
    }
  }
}

void init_mem() {
  memset(pmem, rand(), MSIZE);
}
