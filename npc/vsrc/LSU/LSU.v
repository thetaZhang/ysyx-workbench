`include "GlobalDefine.vh"
module LSU (
  /* verilator lint_off UNUSEDSIGNAL */
  input                        clk,
  input                        rst_n,
  /* verilator lint_on UNUSEDSIGNAL */

  output  [`DATA_WIDTH - 1 : 0] mem_data_out,
  input   [`MEM_MODE_WIDTH - 1 : 0] mem_width_in,

  input  [`DATA_WIDTH - 1 : 0] data_wr_in,
  input  [`ADDR_WIDTH - 1 : 0] data_addr_in,

  input                       mem_write_in,
  input                       mem_read_in,

  output                      mem_ready_out,

  // data_mem interface
  output  [`DATA_WIDTH - 1 : 0] data_wr_out,
  output  [`ADDR_WIDTH - 1 : 0] data_addr_out,
  output                        we_out,
  output                        ce_out,
  output  [7 : 0]               wmask_out,
  input   [`DATA_WIDTH - 1 : 0] data_rd_in
);

  // fsm
  reg ls_state;
  reg ls_next_state;

  localparam LS_IDLE = 1'b0;
  localparam LS_WAIT_RD = 1'b1;

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
      ls_state <= LS_IDLE;
    else
      ls_state <= ls_next_state;
  end

  always @(*) begin
    case (ls_state)
      LS_IDLE: ls_next_state = (mem_read_in) ? LS_WAIT_RD : LS_IDLE;
      LS_WAIT_RD: ls_next_state = LS_IDLE;
      default: ls_next_state = LS_IDLE;
    endcase
  end
  
  assign mem_ready_out = ~mem_read_in | ls_state;

  // interface data build

  assign mem_data_out = (mem_width_in == `MEM_WORD) ? data_rd_in :
                        (mem_width_in == `MEM_HALF) ? {{(`DATA_WORD - `DATA_HALF){data_rd_in[`DATA_HALF - 1]}}, data_rd_in[`DATA_HALF - 1 : 0]} :
                        (mem_width_in == `MEM_BYTE) ? {{(`DATA_WORD - `DATA_BYTE){data_rd_in[`DATA_BYTE - 1]}}, data_rd_in[`DATA_BYTE - 1 : 0]} :
                        (mem_width_in == `MEM_BYTE_U) ? {24'b0, data_rd_in[`DATA_BYTE - 1 : 0]} :
                        (mem_width_in == `MEM_HALF_U) ? {16'b0, data_rd_in[`DATA_HALF - 1 : 0]} : data_rd_in;


  assign wmask_out = (mem_width_in == `MEM_WORD) ? 8'h0f :
                      (mem_width_in == `MEM_HALF) ? 8'h03 :
                      (mem_width_in == `MEM_BYTE) ? 8'h01 :
                      (mem_width_in == `MEM_HALF_U) ? 8'h03 :
                      (mem_width_in == `MEM_BYTE_U) ? 8'h01 : 8'h0f;

  assign data_addr_out = data_addr_in[`ADDR_WIDTH - 1 : 0];
  assign data_wr_out   = data_wr_in;


  assign we_out = mem_write_in;
  assign ce_out = (mem_read_in | mem_write_in) & ~ls_state;

endmodule
