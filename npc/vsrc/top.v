`include "GlobalDefine.vh"
module top(
  input clk,
  input rst_n
);



  wire [  `ADDR_WIDTH - 1 : 0] pc;
  wire  [`INST_WIDTH - 1 : 0] inst;
  wire                         inst_ce;

  wire  [`DATA_WIDTH - 1 : 0] data_rd;
  wire  [`ADDR_WIDTH - 1 : 0] data_addr;
  wire                        data_we;
  wire                        data_ce;
  wire  [7 : 0]               data_wmask;
  wire  [`DATA_WIDTH - 1 : 0] data_wr;


    core core_u (
      .clk           (clk),
      .rst_n         (rst_n),

      // inst_mem
      .inst_ce_out   (inst_ce),
      .pc            (pc),
      .inst_in       (inst),

      //data_mem
      .data_ce_out   (data_ce),
      .data_we_out   (data_we),
      .data_addr_out (data_addr),
      .data_wmask_out(data_wmask),
      .data_wr_out   (data_wr),
      .data_rd_in   (data_rd)
  );

  // inst_mem
  InstMem inst_mem_u (
      .clk          (clk),
      .ce           (inst_ce),
      .inst_addr_in (pc),
      .inst_out     (inst)
  );

  // data_mem
  DataMem data_mem_u (
      .clk          (clk),
      .ce_in        (data_ce),
      .we_in        (data_we),
      .data_addr_in (data_addr),
      .data_wr_in   (data_wr),
      .wmask_in     (data_wmask),
      .data_rd_out  (data_rd)
  );


endmodule
