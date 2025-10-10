// instruction fetch
`include "GlobalDefine.vh"
module IFU (
  input clk,
  input rst_n,
  input [`ADDR_WIDTH - 1 : 0] pc_in,
  output [`ADDR_WIDTH - 1 : 0] pc_out,
  output inst_ce_out,
  output inst_en_out,

  input mem_ready_in
);


/* timing diagram

           --\ /----------\ /----------\ /----------\ /----------\ /--
 ifu_raddr    X 0x80000000 X     (1)    X 0x80000004 X            X
           --/ \----------/ \----------/ \----------/ \----------/ \--
           --\ /----------\ /----------\ /----------\ /----------\ /--
 ifu_rdata    X            X 0x00000413 X     (2)    X 0x80051137 X
           --/ \----------/ \----------/ \----------/ \----------/ \--

*/

// IF fsm

wire if_state;
wire if_next_state;


localparam IF_IDLE = 1'b0;
localparam IF_WAIT = 1'b1;

DffNegRst #(1, IF_WAIT) if_state_reg (clk, rst_n, if_next_state, if_state); // initial state is WAIT, so that first clock pc will not add 4 and cant fetch first inst

assign if_next_state = ((if_state == IF_IDLE) && mem_ready_in) ? IF_WAIT : IF_IDLE;


// pc set
DffNegRstEn #(`ADDR_WIDTH, `INIT_INST_ADDR) pc_reg (clk, rst_n, (~if_state && mem_ready_in), pc_in, pc_out);

// inst ce, state code as output
assign inst_ce_out = if_state;

// inst en, enable cpu run, it should be 1 at the posedge when if_state is IDLE, to enable reg and mem
assign inst_en_out = ~if_state;


export "DPI-C" function pc_probe;

function int unsigned pc_probe();
  return pc_out;
endfunction

export "DPI-C" function if_state_probe;

function bit if_state_probe();
  return if_state;
endfunction

endmodule
