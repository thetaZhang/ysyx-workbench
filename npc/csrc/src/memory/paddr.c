#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <cpu/probe.h>



static uint8_t pmem[CONFIG_MSIZE] = {};

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static void out_of_bound(paddr_t addr) {
  uint32_t pc = get_pc();
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
      addr, PMEM_LEFT, PMEM_RIGHT, pc);
}

word_t pmem_read(paddr_t addr) {
  IFDEF(CONFIG_MTRACE, log_write("paddr_read: addr = " FMT_PADDR ", len = %d\n", addr, 4));
  if (likely(in_pmem(addr))) return host_read(guest_to_host(addr), 4);
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, 4));
  out_of_bound(addr);
  return 0;
}

void pmem_write(paddr_t addr, word_t data, uint8_t mask) {
  int len = __builtin_popcount(mask & 0x0F);
  IFDEF(CONFIG_MTRACE, log_write("paddr_write: addr = " FMT_PADDR ", len = %d, data = " FMT_WORD "\n", addr, len, data));
  if (likely(in_pmem(addr))) {
    for (int i = 0; i < sizeof(word_t)/sizeof(uint8_t); i++) {
      if (mask & (1 << i)) {
        host_write(guest_to_host(addr + i), 1, (data >> (i * 8)) & 0xff);
      }
    }
    return;
  }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return);
  out_of_bound(addr);
}

void init_mem() {
  memset(pmem, rand(), CONFIG_MSIZE);
}
