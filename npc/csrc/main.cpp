
#include "macro.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <stdio.h>
#include <assert.h>
#include "memory/paddr.h"
#include str(TOP_MODULE_HEADER)
#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))



void exit(int code){
  //assert(code == 0);
  printf("Exiting with code %d\n", code);
  Verilated::gotFinish(true);
  return;
}

int main(int argc, char** argv){
	VerilatedContext* contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  Vtop* top = new Vtop{contextp};
  //Verilated::traceEverOn(true);
  //VerilatedVcdC* tfp = new VerilatedVcdC;
  //top->trace(tfp, 0);
  //tfp->open("build/waveform.vcd");
  uint64_t main_time = 0;
  top->clk = 0;
  top->rst_n = 0;
  for ( int i = 0; i < 10; i++) {
    pmem[i] = 0x00100093;
  }
  pmem[10] = 0x00100073;
  for ( int i = 11; i < PMEM_SIZE / 4; i++) {
    pmem[i] = 0x00100093;
  }
  while (!contextp->gotFinish()) { 
    top->clk = !top->clk;
    top->eval();
    if (main_time == 10) {
      top->rst_n = 1;
    }
    if (top->inst_ce_out) {
      top->inst_in = pmem_read(top->inst_addr_out, 4);
      printf("pc = 0x%08x, inst = 0x%08x\n", top->inst_addr_out, top->inst_in);
    }
    //tfp->dump(main_time);
    main_time++;
    // if (main_time > 100) {
    //   printf("Time out!\n");
    //   break;
    // }
  }
  //tfp->close();
  delete top;
  delete contextp;
  return 0;
}
