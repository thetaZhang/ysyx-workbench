/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  int reg_num = ARRLEN(cpu.gpr);
  char reg_name[4];
  for (int i = 0; i < reg_num; i ++) {
    if (ref_r->gpr[i] != cpu.gpr[i]) {
      isa_reg_get_name(i, reg_name);
      printf("reg %s is different after executing instruction at pc = 0x%08x, right = 0x%08x, wrong = 0x%08x\n",
          reg_name, pc, ref_r->gpr[i], cpu.gpr[i]);
      return false;
    }
  }

  if (ref_r->pc != cpu.pc) {
    printf("pc is different after executing instruction at pc = 0x%08x, right = 0x%08x, wrong = 0x%08x\n",
        pc, ref_r->pc, cpu.pc);
    return false;
  }
  return true;
}

void isa_difftest_attach() {
}
