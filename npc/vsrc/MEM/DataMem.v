`include "GlobalDefine.vh"
`include "DPI-C.vh"
module DataMem(
  input                        clk,
  input  [`DATA_WIDTH - 1 : 0] data_wr_in,
  input  [`ADDR_WIDTH - 1 : 0] data_addr_in,
  input                        we_in,
  input                        ce_in,
  input  [7 : 0]               wmask_in,
  output reg [`DATA_WIDTH - 1 : 0] data_rd_out
);



  always @(posedge clk) begin
    if (ce_in) begin
      data_rd_out <= (!we_in) ? pmem_read(data_addr_in) : `DATA_WIDTH'b0;
      if (we_in)
        pmem_write(data_addr_in, data_wr_in, wmask_in);
    end
  end

endmodule
