#include "Vtop.h"
#include "verilated.h"
#include "verilated_fst_c.h"
#include <stdio.h>
#include <assert.h>



int main(int argc, char** argv){
	VerilatedContext* contextp = new VerilatedContext;	 contextp->commandArgs(argc, argv);
  Vtop* top = new Vtop{contextp};
  Verilated::traceEverOn(true);
  VerilatedFstC* tfp = new VerilatedFstC;
  top->trace(tfp, 0);
  tfp->open("build/waveform.fst");
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
    //   top->inst_in += pmem_read(top->inst_addr_out);
    // }
    tfp->dump(main_time++);
  }
  tfp->close();
  delete top;
  delete contextp;
  return 0;
}
