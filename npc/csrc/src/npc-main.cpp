
#include "macro.h"

#include <stdio.h>
#include <assert.h>

#include <common.h>
#include "memory/paddr.h"



void init_monitor(int, char *[]);
// extern word_t reg_probe(word_t raddr);
void sdb_mainloop();
void init_cpu(int argc, char** argv);
void deinit_cpu();


int main(int argc, char** argv){
  init_cpu(argc, argv);
  init_monitor(argc, argv);
  sdb_mainloop();
  deinit_cpu();
	// VerilatedContext* contextp = new VerilatedContext;
  // contextp->commandArgs(argc, argv);
  // Vtop* top = new Vtop{contextp};
  // #ifdef WAVE
  // Verilated::traceEverOn(true);
  // VerilatedVcdC* tfp = new VerilatedVcdC;
  // top->trace(tfp, 0);
  // tfp->open("build/waveform.vcd");
  // #endif
  // int main_time = 0;
  // top->clk = 0;
  // top->rst_n = 0;
  // while (!contextp->gotFinish()) { 
  //   top->clk = !top->clk;
  //   top->eval();
  //   if (main_time == 10) {
  //     top->rst_n = 1;
  //   }
    // if (top->inst_ce_out) {
    //   top->inst_in = pmem_read(top->inst_addr_out, 4);
    //   printf("pc = 0x%08x, inst = 0x%08x\n", top->inst_addr_out, top->inst_in);
    // }
    // #ifdef WAVE
    // tfp->dump(main_time);
    // #endif
    // main_time++;
    // if (main_time > 100) {
    //   printf("Time out!\n");
    //   break;
    // }
  // }
  // #ifdef WAVE
  // tfp->close();
  // #endif
  // delete top;
  // delete contextp;
  return 0;
}
