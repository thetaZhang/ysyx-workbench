#ifndef PROBE_H
#define PROBE_H

#include <common.h>

word_t get_pc();
word_t get_reg(int index);
word_t get_inst();
void set_gpr(word_t* gpr);
#endif
