#include <common.h>
#include <cpu/probe.h>
#include str(TOP_MODULE_HEADER)  //these macro str and concat are defined in common.h, so include it first
#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))


word_t get_pc() {
  svSetScope(svGetScopeFromName("TOP.top.core_u.ifu"));
  return pc_probe();
}

word_t get_reg(int index) {
  assert(index >= 0 && index < 32);
  svSetScope(svGetScopeFromName("TOP.top.core_u.regfile_u"));
  return reg_probe(index);
}

word_t get_inst() {
  svSetScope(svGetScopeFromName("TOP.top.core_u"));
  return inst_probe();
}



if_state_t get_if_state() {
  svSetScope(svGetScopeFromName("TOP.top.core_u.ifu"));
  return (if_state_probe()) ? IF_WAIT : IF_IDLE;
}

void set_gpr(word_t* gpr) {
  assert(gpr != NULL);
  for (int i = 0;i < 32;i++){
    gpr[i] = get_reg(i);
  }
}