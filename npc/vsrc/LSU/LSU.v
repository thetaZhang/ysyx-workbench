`include "GlobalDefine.vh"
module LSU (

  input                        clk,
  input                        rst_n,


  output  reg [`DATA_WIDTH - 1 : 0] mem_data_out,
  input   [`MEM_MODE_WIDTH - 1 : 0] mem_width_in,
  input  [`DATA_WIDTH - 1 : 0] data_wr_in,
  input  [`ADDR_WIDTH - 1 : 0] data_addr_in,
  input                       mem_write_in,
  input                       mem_read_in,
  output reg                  mem_ready_out,

  // data_mem interface
  output        lsu_reqValid,
  output [`ADDR_WIDTH - 1 : 0] lsu_addr,
  output        lsu_wen,
  output [`DATA_WIDTH - 1 : 0] lsu_wdata,
  output [ 7:0] lsu_wmask,
  input         lsu_respValid,
  input  [`DATA_WIDTH - 1 : 0] lsu_rdata
);



  // fsm
  reg [1:0] ls_state;
  reg [1:0] ls_next_state;

  localparam LS_IDLE = 2'b00;
  localparam LS_WAIT = 2'b01;
  localparam LS_DONE = 2'b11;

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n)
      ls_state <= LS_IDLE;
    else
      ls_state <= ls_next_state;
  end

  always @(*) begin
    case (ls_state)
      LS_IDLE: ls_next_state = (mem_read_in || mem_write_in) ? LS_WAIT : LS_IDLE;
      LS_WAIT: ls_next_state = lsu_respValid ? LS_DONE : LS_WAIT;
      LS_DONE: ls_next_state = LS_IDLE;
      default: ls_next_state = LS_IDLE;
    endcase
  end

  // bus interface
  assign lsu_reqValid = (mem_read_in || mem_write_in) & ~ls_state[0];
  assign lsu_addr     = data_addr_in;
  assign lsu_wen      = mem_write_in;
  assign lsu_wdata    = data_wr_in;
  assign lsu_wmask    = (mem_width_in == `MEM_WORD) ? 8'h0f :
                      (mem_width_in == `MEM_HALF) ? 8'h03 :
                      (mem_width_in == `MEM_BYTE) ? 8'h01 :
                      (mem_width_in == `MEM_HALF_U) ? 8'h03 :
                      (mem_width_in == `MEM_BYTE_U) ? 8'h01 : 8'h0f;
  

  // internal signal

  always @(posedge clk or negedge rst_n) begin
    if (!rst_n) begin
      mem_ready_out <= 1'b0;
      mem_data_out <= 0;
  end
    else if (lsu_respValid) begin
      mem_ready_out <= 1'b1;
      mem_data_out <= (mem_width_in == `MEM_WORD) ? lsu_rdata :
                   (mem_width_in == `MEM_HALF) ? {{(`DATA_WORD - `DATA_HALF){lsu_rdata[`DATA_HALF - 1]}}, lsu_rdata[`DATA_HALF - 1 : 0]} :
                   (mem_width_in == `MEM_BYTE) ? {{(`DATA_WORD - `DATA_BYTE){lsu_rdata[`DATA_BYTE - 1]}}, lsu_rdata[`DATA_BYTE - 1 : 0]} :
                   (mem_width_in == `MEM_BYTE_U) ? {24'b0, lsu_rdata[`DATA_BYTE - 1 : 0]} :
                   (mem_width_in == `MEM_HALF_U) ? {16'b0, lsu_rdata[`DATA_HALF - 1 : 0]} : lsu_rdata;
    end
    else begin
      mem_ready_out <= 1'b0;
      mem_data_out <= 0;
    end
  end



endmodule
