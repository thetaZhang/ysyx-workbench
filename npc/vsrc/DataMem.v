`include "GlobalDefine.vh"
`include "DPI-C.vh"
module DataMem(
  input                        clk,
  input                        rst_n,
  input  [`DATA_WIDTH - 1 : 0] data_wr_in,
  input  [`ADDR_WIDTH - 1 : 0] data_addr_in,
  input                        we_in,
  input                        ce_in,
  input  [7 : 0]               wmask_in,
  output reg [`DATA_WIDTH - 1 : 0] data_rd_out
);


  always @(*) begin
    if (ce_in && !we_in)
      data_rd_out = pmem_read(data_addr_in);
    else
      data_rd_out = `DATA_WIDTH'b0;
  end
  

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      // do nothing
    end
    else if (ce_in && we_in)
      pmem_write(data_addr_in, data_wr_in, wmask_in);
  end

endmodule
