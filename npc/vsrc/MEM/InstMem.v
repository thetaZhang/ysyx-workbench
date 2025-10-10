`include "GlobalDefine.vh"
`include "DPI-C.vh"
module InstMem(

  input clk,

  input ce,
  input [`ADDR_WIDTH - 1 : 0] inst_addr_in,
  output reg [`INST_WIDTH - 1 : 0] inst_out
);


  always @(posedge clk) begin
    if (ce)
      inst_out <= pmem_read(inst_addr_in);
  end

endmodule
