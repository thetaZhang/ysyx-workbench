`include "GlobalDefine.vh"
module WBU(
  /* verilator lint_off UNUSEDSIGNAL */
  input  clk,
  input  rst_n,
  /* verilator lint_on UNUSEDSIGNAL */


  input mem_to_reg_in,
  input mem_en_in,
  input mem_ready_in,
  input [`DATA_WIDTH - 1 : 0] mem_data_in,
  input [`DATA_WIDTH - 1 : 0] ex_data_in,
  output [`DATA_WIDTH - 1 : 0] wb_data_out,
  output exec_ready_out

);

  assign wb_data_out = mem_to_reg_in ? mem_data_in : ex_data_in;

  // always @(posedge clk or negedge rst_n) begin
  //   if (!rst_n)
  //     exec_ready_out <= 1'b0;
  //   else if (mem_en_in)
  //     exec_ready_out <= mem_ready_in;
  //   else
  //     exec_ready_out <= 1'b1; 
  // end

  assign exec_ready_out = mem_en_in ? mem_ready_in : 1'b1;

endmodule
