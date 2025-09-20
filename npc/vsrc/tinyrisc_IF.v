// instruction fetch
`include "GlobalDefine.vh"
module tinyrisc_IF (
  input clk,
  input rst_n,
  input [`ADDR_WIDTH - 1 : 0] pc_in,
  output [`ADDR_WIDTH - 1 : 0] pc_out,
  output inst_ce_out
);


DffNegRst #(`ADDR_WIDTH, `INIT_INST_ADDR) pc_reg_u (clk, rst_n, pc_in, pc_out);

DffNegRst #(1, 1'b1) inst_ce_reg_u (clk, rst_n, 1'b1, inst_ce_out);

export "DPI-C" function pc_probe;

function int unsigned pc_probe();
  return pc_out;
endfunction


endmodule
