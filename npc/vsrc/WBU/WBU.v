`include "GlobalDefine.vh"
module WBU(
  /* verilator lint_off UNUSEDSIGNAL */
  input  clk,
  input  rst_n,
  /* verilator lint_on UNUSEDSIGNAL */


  input mem_to_reg_in,
  input [`DATA_WIDTH - 1 : 0] mem_data_in,
  input [`DATA_WIDTH - 1 : 0] ex_data_in,
  output [`DATA_WIDTH - 1 : 0] wb_data_out

);

  assign wb_data_out = mem_to_reg_in ? mem_data_in : ex_data_in;

endmodule
