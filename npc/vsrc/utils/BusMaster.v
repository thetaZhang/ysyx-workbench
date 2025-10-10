// /*
// +--------+ reqValid           ---> +-------+
// |        | addr[log2(N)-1:0]  ---> |       |
// |        | wen                ---> |       |
// | Master | wdata[31:0]        ---> | Slave |
// |        | wmask[3:0]         ---> |       |
// |        | <---          respValid |       |
// |        | <---        rdata[31:0] |       |
// +--------+                         +-------+
// */

// module BusMaster #(
//   parameter ADDR_WIDTH = 32,
//   parameter DATA_WIDTH = 32
// )(
//   input clk,
//   input rst_n,

//   // Master Interface
//   output                       reqValid_out,
//   output [ADDR_WIDTH - 1 : 0]  addr_out,
//   output                       we_out,
//   output [DATA_WIDTH - 1 : 0]  wdata_out,
//   output [(DATA_WIDTH/8) - 1 : 0] wmask_out,

//   input                        respValid_in,
//   input  [DATA_WIDTH - 1 : 0]  rdata_in,

//   // data and control signals
//   input                        valid_in,
//   input                        we_in,
//   input [ADDR_WIDTH - 1 : 0]   addr_in,
//   input [DATA_WIDTH - 1 : 0]   wdata_in,
//   input [(DATA_WIDTH/8) - 1 : 0] wmask_in,
//   output [DATA_WIDTH - 1 : 0]  rdata_out,
//   output                       ready_out

// );


// reg port_state;
// wire port_next_state;


// assign addr_out = addr_in;
// assign wdata_out = wdata_in;
// assign rdata_out = rdata_in;
// assign we_out = we_in;
// assign reqValid_out = valid_in;
// assign ready_out = port_state;



// // fsm
// localparam PORT_IDLE = 1'b1;
// localparam PORT_WAIT = 1'b0;

// always @(posedge clk or negedge rst_n) begin
//   if (!rst_n) 
//     port_state <= PORT_IDLE;
//   else
//     port_state <= port_next_state;
// end

// always @(*) begin
//   case (port_state)
//     PORT_IDLE: begin
//         port_next_state = reqValid_out ? PORT_WAIT : PORT_IDLE;
//     end
//     PORT_WAIT: begin
//       port_next_state = respValid_in ? PORT_IDLE : PORT_WAIT;
//     end
//     default: port_next_state = PORT_IDLE;
//   endcase
// end



// endmodule
