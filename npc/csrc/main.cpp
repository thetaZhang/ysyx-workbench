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
  while (!contextp->gotFinish()) { 
    int a = rand() & 1;
    int b = rand() & 1;
    top->a = a;
    top->b = b;
    top->eval();
    tfp->dump(main_time++);
    printf("a=%d b=%d f=%d\n", a, b, top->f);
    assert(top->f == (a ^ b));
    if (main_time > 50) break;
  }
  tfp->close();
  delete top;
  delete contextp;
  return 0;
}
