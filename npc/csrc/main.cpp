
#include "macro.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <stdio.h>
#include <assert.h>

#include <common.h>
#include "memory/paddr.h"

#include str(TOP_MODULE_HEADER)
#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))

void init_monitor(int, char *[]);
// extern word_t reg_probe(word_t raddr);

extern "C" void npc_trap(){
  svSetScope(svGetScopeFromName("TOP.top.ID_u.regfile_u"));
  uint32_t code = reg_probe(10);
  svSetScope(svGetScopeFromName("TOP.top.IF_u"));
  uint32_t pc = pc_probe();
  Log("npc: %s at pc = " FMT_WORD,
          ((code == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
            ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
            pc);
  Verilated::gotFinish(true);
  exit(code);
}

int main(int argc, char** argv){
  init_monitor(argc, argv);
	VerilatedContext* contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  Vtop* top = new Vtop{contextp};
  Verilated::traceEverOn(true);
  VerilatedVcdC* tfp = new VerilatedVcdC;
  top->trace(tfp, 0);
  tfp->open("build/waveform.vcd");
  uint64_t main_time = 0;
  top->clk = 0;
  top->rst_n = 0;
  while (!contextp->gotFinish()) { 
    top->clk = !top->clk;
    top->eval();
    if (main_time == 10) {
      top->rst_n = 1;
    }
    // if (top->inst_ce_out) {
    //   top->inst_in = pmem_read(top->inst_addr_out, 4);
    //   printf("pc = 0x%08x, inst = 0x%08x\n", top->inst_addr_out, top->inst_in);
    // }
    tfp->dump(main_time);
    main_time++;
    // if (main_time > 100) {
    //   printf("Time out!\n");
    //   break;
    // }
  }
  tfp->close();
  delete top;
  delete contextp;
  return 0;
}
