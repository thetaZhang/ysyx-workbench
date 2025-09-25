#include <cpu/cpu.h>
#include <cpu/decode.h>
// #include <cpu/difftest.h>
#include <locale.h>
#include <utils.h>

#include "verilated.h"
#include "verilated_vcd_c.h"
#include str(TOP_MODULE_HEADER)
#include "svdpi.h"
#include str(concat(TOP_MODULE,__Dpi.h))

// #include "sdb.h" // for watchpoints

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INST_TO_PRINT 10

CPU_state cpu = {};
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;

Vtop* top;
VerilatedContext* contextp;
int main_time = 0;

void device_update();

//bool wp_difftest();

// static void trace_and_difftest(Decode *_this, vaddr_t dnpc) {
// #ifdef CONFIG_ITRACE_COND
//   if (ITRACE_COND) { log_write("%s\n", _this->logbuf); iringbuf_push(_this->logbuf); }
// #endif
  // if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
  
  //IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));

// #ifdef CONFIG_WATCHPOINT
//   if (wp_difftest()) {
//     nemu_state.state = NEMU_STOP;
//     printf("Hit watchpoint at pc = " FMT_WORD "\n", _this->pc);
//   }
// #endif
// }

static void exec_once() {
    top->clk = !top->clk;
    top->eval();
    main_time++;
  

// #ifdef CONFIG_ITRACE
//   char *p = s->logbuf;
//   p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc);
//   int ilen = s->snpc - s->pc;
//   int i;
//   uint8_t *inst = (uint8_t *)&s->isa.inst;
// #ifdef CONFIG_ISA_x86
//   for (i = 0; i < ilen; i ++) {
// #else
//   for (i = ilen - 1; i >= 0; i --) {
// #endif
//     p += snprintf(p, 4, " %02x", inst[i]);
//   }
//   int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
//   int space_len = ilen_max - ilen;
//   if (space_len < 0) space_len = 0;
//   space_len = space_len * 3 + 1;
//   memset(p, ' ', space_len);
//   p += space_len;

//   // void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
//   // disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
//   //     MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst, ilen);
// #endif
}

static void execute(uint64_t n) {
  for (;n > 0; n --) {
    exec_once();
    g_nr_guest_inst ++;
    // trace_and_difftest(&s, cpu.pc);
    if (npc_state.state != NPC_RUNNING) break;
    IFDEF(CONFIG_DEVICE, device_update());
  }
}

static void statistic() {
  IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
  Log("host time spent = " NUMBERIC_FMT " us", g_timer);
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
  if (g_timer > 0) Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
  else Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg() {
  isa_reg_display();
  IFDEF(CONFIG_ITRACE, iringbuf_display();iringbuf_free());
  statistic();
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n) {
  g_print_step = (n < MAX_INST_TO_PRINT);
  switch (npc_state.state) {
    case NPC_END: case NPC_ABORT: case NPC_QUIT:
      printf("Program execution has ended. To restart the program, exit NPC and run again.\n");
      return;
    default: npc_state.state = NPC_RUNNING;
  }

  uint64_t timer_start = get_time();

  execute(n);

  uint64_t timer_end = get_time();
  g_timer += timer_end - timer_start;

  switch (npc_state.state) {
    case NPC_RUNNING: npc_state.state = NPC_STOP; break;
    case NPC_END: case NPC_ABORT:{
      Log("npc: %s at pc = " FMT_WORD,
          (npc_state.state == NPC_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
           (npc_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
            ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
          npc_state.halt_pc);
      if (npc_state.halt_ret != 0 || npc_state.state == NPC_ABORT) {
        IFDEF(CONFIG_ITRACE, iringbuf_display();iringbuf_free());
      }
    }
      // fall through
    case NPC_QUIT: statistic();
  }
}

extern "C" void npc_trap(){
  svSetScope(svGetScopeFromName("TOP.top.ID_u.regfile_u"));
  uint32_t code = reg_probe(10);
  svSetScope(svGetScopeFromName("TOP.top.IF_u"));
  uint32_t pc = pc_probe();
  npc_state.state = NPC_END;
  npc_state.halt_pc = pc;
  npc_state.halt_ret = code;
  Verilated::gotFinish(true);
  //exit(code);
}

void init_cpu(int argc, char** argv){
  contextp = new VerilatedContext;
  contextp->commandArgs(argc, argv);
  top = new Vtop{contextp};
  top->clk = 0;
  top->rst_n = 0;
  while (1) { 
    top->clk = !top->clk;
    top->eval();
    if (main_time == 10) {
      top->rst_n = 1;
      main_time++;
      break;
    }
    main_time++;
  }
}

void deinit_cpu(){
  delete top;
  delete contextp;
}