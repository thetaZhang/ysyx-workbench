// instruction fetch
`include "GlobalDefine.vh"
module IFU (
  input clk,
  input rst_n,
  input [`ADDR_WIDTH - 1 : 0] pc_in,
  output reg [`ADDR_WIDTH - 1 : 0] pc_out,
  output reg [`DATA_WIDTH - 1 : 0] inst_out,
  output inst_en_out,
  input exec_ready_in,

  // bus master interface
  output                         ifu_reqValid,
  output [`ADDR_WIDTH - 1 : 0]   ifu_addr,
  input                          ifu_respValid,
  input  [`DATA_WIDTH - 1 : 0]   ifu_rdata
);

// IF fsm
reg [1:0] if_state;
reg [1:0] if_next_state;


localparam IF_IDLE = 2'b00;
localparam IF_WAIT_FETCH = 2'b01;
localparam IF_WAIT_EXEC = 2'b10;

always @(posedge clk or negedge rst_n) begin
  if (!rst_n)
    if_state <= IF_IDLE;
  else
    if_state <= if_next_state;
end

always @(*) begin
  case (if_state)
    IF_IDLE: if_next_state = IF_WAIT_FETCH;
    IF_WAIT_FETCH: if_next_state = ifu_respValid ? IF_WAIT_EXEC : IF_WAIT_FETCH;
    IF_WAIT_EXEC: if_next_state = exec_ready_in ? IF_IDLE : IF_WAIT_EXEC;
    default: if_next_state = IF_IDLE;
  endcase
end

// pc load
always @(posedge clk or negedge rst_n) begin
  if (!rst_n)
    pc_out <= `INIT_INST_ADDR;
  else if (if_state == IF_WAIT_EXEC && exec_ready_in)
    pc_out <= pc_in;
end

// instruction fetch in reg
always @(posedge clk or negedge rst_n) begin
  if (!rst_n)
    inst_out <= 0;
  else if (ifu_respValid)
    inst_out <= ifu_rdata;
end


assign inst_en_out = if_state[1];

// bus interface 
assign ifu_reqValid = (if_state == IF_IDLE);

assign ifu_addr = pc_out;




export "DPI-C" function pc_probe;

function int unsigned pc_probe();
  return pc_out;
endfunction

export "DPI-C" function if_state_probe;

function byte unsigned if_state_probe();
  return {6'b0,if_state};
endfunction

endmodule
