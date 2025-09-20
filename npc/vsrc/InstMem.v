`include "GlobalDefine.vh"
`include "DPI-C.vh"
module InstMem(
  input ce,
  input [`ADDR_WIDTH - 1 : 0] inst_addr_in,
  output reg [`INST_WIDTH - 1 : 0] inst_out
);


  always @(*) begin
    if (ce)
      inst_out = pmem_read(inst_addr_in);
    else
      inst_out = `INST_WIDTH'b0;
  end

endmodule
