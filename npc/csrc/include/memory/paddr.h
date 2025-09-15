#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include <common.h>

#define MSIZE 0x80000000
#define MBASE 0x80000000


void init_mem();


uint8_t* guest_to_host(paddr_t paddr);

paddr_t host_to_guest(uint8_t *haddr);


word_t paddr_read(paddr_t addr);
void paddr_write(paddr_t addr, word_t data, uint8_t mask);

#endif
