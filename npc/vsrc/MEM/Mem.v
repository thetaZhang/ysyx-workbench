`include "GlobalDefine.vh"
`include "DPI-C.vh"
/* verilator lint_off UNUSEDSIGNAL */
module Mem #(
  parameter ADDR_WIDTH = `ADDR_WIDTH,
  parameter DATA_WIDTH = `DATA_WIDTH
)(
  input clk,
  input rst_n,
  // slave Interface
  input                        reqValid,
  input [ADDR_WIDTH - 1:0]     addr,
  input                        wen,
  input [DATA_WIDTH - 1:0]     wdata,
  input [8 - 1:0] wmask,
  output reg                   respValid,
  output reg [DATA_WIDTH - 1:0]rdata

);

wire lfsr_en;
reg reqValid_last;
wire [15 : 0] lfsr_out;
reg  [3 : 0] delay_cnt;
reg delay_en;

reg [DATA_WIDTH - 1 : 0] data_reg;
reg [ADDR_WIDTH - 1 : 0] addr_reg;
reg [7 : 0] wmask_reg;
reg wen_reg;

always @(posedge clk) begin
  if (reqValid && !wen)
    rdata <= pmem_read(addr);
  else 
    rdata <= 32'b0;
  
  if (reqValid && wen) 
    pmem_write(addr, wdata, wmask);
  
  respValid <= reqValid;
end

// always @(posedge clk or negedge rst_n) begin
//   if (!rst_n) begin
//     data_reg <= {DATA_WIDTH{1'b0}};
//     addr_reg <= {ADDR_WIDTH{1'b0}};
//     wmask_reg <= 8'b0;
//     wen_reg <= 1'b0;
//   end
//   else if (reqValid) begin
//     data_reg <= wdata;
//     addr_reg <= addr;
//     wmask_reg <= wmask;
//     wen_reg <= wen;
//   end
// end

// always @(posedge clk) begin
//   respValid <= delay_en;
//   rdata <= 32'b0;
//   if (delay_en && !wen_reg)
//     rdata <= pmem_read(addr_reg);
//   else if (delay_en && wen_reg) begin
//     pmem_write(addr_reg, data_reg, wmask_reg);
//   end
    
// end

// always @(posedge clk or negedge rst_n) begin
//   if (!rst_n)
//     reqValid_last <= 1'b0;
//   else
//     reqValid_last <= reqValid;
// end

// always @(posedge clk or negedge rst_n) begin
//   delay_en <= 1'b0;
//   if (!rst_n) begin
//     delay_cnt <= 0;
//     delay_en <= 1'b0;
//   end
//   else if (delay_cnt == ((lfsr_out[3:0] == 0) ? lfsr_out[3:0] + 1 : lfsr_out[3:0])) begin
//     delay_cnt <= 0;
//     delay_en <= 1'b1;
//   end
//   else if (lfsr_en || delay_cnt != 0) begin
//     delay_cnt <= delay_cnt + 1;
    
//   end
// end

// assign lfsr_en = reqValid && !reqValid_last;

// LFSR lfsr_inst (
//   .clk(clk),
//   .rst_n(rst_n),
//   .en(lfsr_en),
//   .lfsr_out(lfsr_out)
// );


endmodule
