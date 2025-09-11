// Arithmetic Logical Unit
`include "GlobalDefine.vh"
module ALU #(
    parameter DATA_WIDTH = 32
)(
    input [3 : 0] op_ctrl,

    input [DATA_WIDTH - 1 : 0] data_in_1,
    input [DATA_WIDTH - 1 : 0] data_in_2,

    output [DATA_WIDTH - 1 : 0] data_out,

    output zero
);

// ALUopcpde
localparam NONE = `ALU_NONE;
localparam ADD = `ALU_ADD;
localparam SUB = `ALU_SUB;
localparam AND = `ALU_AND;
localparam OR  = `ALU_OR;
localparam XOR = `ALU_XOR;
localparam SLL = `ALU_SLL;
localparam SRL = `ALU_SRL;
localparam LT  = `ALU_LT;
localparam LTU = `ALU_LTU;
localparam SRA = `ALU_SRA;



wire [DATA_WIDTH - 1 : 0] res_add;
wire [DATA_WIDTH - 1 : 0] res_sub;
wire [DATA_WIDTH - 1 : 0] res_and;
wire [DATA_WIDTH - 1 : 0] res_or;
wire [DATA_WIDTH - 1 : 0] res_xor;
wire [DATA_WIDTH - 1 : 0] res_sll;
wire [DATA_WIDTH - 1 : 0] res_srl;
wire [DATA_WIDTH - 1 : 0] res_sra;
wire [DATA_WIDTH - 1 : 0] res_lt;
wire [DATA_WIDTH - 1 : 0] res_ltu;

assign res_add = data_in_1 + data_in_2;
assign res_sub = data_in_1 - data_in_2;
assign res_and = data_in_1 & data_in_2;
assign res_or  = data_in_1 | data_in_2;
assign res_xor = data_in_1 ^ data_in_2;
assign res_sll = data_in_1 << data_in_2[4:0];
assign res_srl = data_in_1 >> data_in_2[4:0];
assign res_sra = $signed(data_in_1) >>> data_in_2[4:0];
assign res_lt  = ($signed(data_in_1) < $signed(data_in_2)) ? {{(DATA_WIDTH - 1){1'b0}},1'b1} : {DATA_WIDTH{1'b0}};
assign res_ltu = (data_in_1 < data_in_2) ? {{(DATA_WIDTH - 1){1'b0}},1'b1} : {DATA_WIDTH{1'b0}};


assign data_out = (op_ctrl == NONE) ? data_in_2 :
                  (op_ctrl == ADD) ? res_add :
                  (op_ctrl == SUB) ? res_sub :
                  (op_ctrl == AND) ? res_and :
                  (op_ctrl == OR)  ? res_or :
                  (op_ctrl == XOR) ? res_xor :
                  (op_ctrl == SLL) ? res_sll :
                  (op_ctrl == SRL) ? res_srl :
                  (op_ctrl == SRA) ? res_sra :
                  (op_ctrl == LT)  ? res_lt :
                  (op_ctrl == LTU) ? res_ltu : 0;

assign zero = ~(|data_out);

endmodule
