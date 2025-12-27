`include "GlobalDefine.vh"
module top(
  input clk,
  input rst_n
);



  wire                          ifu_reqValid;
  wire [`ADDR_WIDTH - 1 : 0]    ifu_addr;
  wire                          ifu_respValid;
  wire  [`DATA_WIDTH - 1 : 0]   ifu_rdata;


  wire                     lsu_reqValid;
  wire [`ADDR_WIDTH - 1 : 0] lsu_addr;
  wire                     lsu_wen;
  wire [`DATA_WIDTH - 1 : 0] lsu_wdata;
  wire [ 7:0]              lsu_wmask;
  wire                     lsu_respValid;
  wire  [`DATA_WIDTH - 1 : 0] lsu_rdata;



    core core_u (
      .clk           (clk),
      .rst_n         (rst_n),

      // inst_mem
      .ifu_reqValid (ifu_reqValid),
      .ifu_addr     (ifu_addr),
      .ifu_respValid(ifu_respValid),
      .ifu_rdata    (ifu_rdata),

      //data_mem
      .lsu_reqValid (lsu_reqValid),
      .lsu_addr     (lsu_addr),
      .lsu_wen      (lsu_wen),
      .lsu_wdata    (lsu_wdata),
      .lsu_wmask    (lsu_wmask),
      .lsu_respValid(lsu_respValid),
      .lsu_rdata    (lsu_rdata)
  );

  // inst_mem
  Mem inst_mem_u (
      .clk        (clk),
      .rst_n      (rst_n),
      .reqValid   (ifu_reqValid),
      .addr       (ifu_addr),
      .wen        (0),
      .wdata      (0),
      .wmask      (0),
      .respValid  (ifu_respValid),
      .rdata      (ifu_rdata)

  );

  // data_mem
  Mem data_mem_u (
      .clk        (clk),
      .rst_n      (rst_n),
      .reqValid   (lsu_reqValid),
      .addr       (lsu_addr),
      .wen        (lsu_wen),
      .wdata      (lsu_wdata),
      .wmask      (lsu_wmask),
      .respValid  (lsu_respValid),
      .rdata      (lsu_rdata)

  );



endmodule
