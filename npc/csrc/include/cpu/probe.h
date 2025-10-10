#ifndef PROBE_H
#define PROBE_H

#include <common.h>

typedef enum {
  IF_IDLE = 0,
  IF_WAIT
} if_state_t;

word_t get_pc();
word_t get_reg(int index);
word_t get_inst();
if_state_t get_if_state();
void set_gpr(word_t* gpr);
#endif
