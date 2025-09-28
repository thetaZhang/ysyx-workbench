#include <common.h>
#include str(TOP_MODULE_HEADER)  //these macro str and concat are defined in common.h, so include it first
#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))


word_t get_pc() {
  svSetScope(svGetScopeFromName("TOP.top.IF_u"));
  return pc_probe();
}

word_t get_reg(int index) {
  assert(index >= 0 && index < 32);
  svSetScope(svGetScopeFromName("TOP.top.ID_u.regfile_u"));
  return reg_probe(index);
}

word_t get_inst() {
  svSetScope(svGetScopeFromName("TOP.top"));
  return inst_probe();
}