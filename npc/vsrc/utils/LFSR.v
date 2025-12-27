module LFSR #(
  parameter WIDTH = 16,
  parameter POLY  = 16'hB400,
  parameter SEED  = 16'hACE1
)(
  input                       clk,
  input                       rst_n,
  input                       en,
  output reg [WIDTH - 1 : 0]  lfsr_out
);

wire [WIDTH - 1 : 0] lfsr_next;
wire feedback;

assign feedback = ^(lfsr_out & POLY);
assign lfsr_next = {lfsr_out[WIDTH - 2 : 0], feedback};

always @(posedge clk or negedge rst_n) begin
  if (!rst_n) 
    lfsr_out <= SEED;
  else if (en)
    lfsr_out <= lfsr_next;
end


endmodule
