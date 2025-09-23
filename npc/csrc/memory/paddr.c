#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>

#include <common.h>

#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))

static uint8_t pmem[MSIZE] = {};

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MBASE; }

static void out_of_bound(paddr_t addr) {
  svSetScope(svGetScopeFromName("TOP.top.IF_u"));
  uint32_t pc = pc_probe();
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, pc);
}

extern "C" word_t pmem_read(paddr_t addr) {
  if (likely(in_pmem(addr))) return host_read(guest_to_host(addr), 4);
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, 4););
  out_of_bound(addr);
  return 0;
}

extern "C" void pmem_write(paddr_t addr, word_t data, uint8_t mask) {
  if (likely(in_pmem(addr))) {
    for (int i = 0; i < sizeof(word_t)/sizeof(uint8_t); i++) {
      if (mask & (1 << i)) {
        host_write(guest_to_host(addr + i), 1, (data >> (i * 8)) & 0xff);
      }
    }
    return;
  }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, 4, data); return;);
  out_of_bound(addr);
}

void init_mem() {
  memset(pmem, rand(), MSIZE);
}
