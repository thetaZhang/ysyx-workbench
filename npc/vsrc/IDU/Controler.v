// main controller
`include "GlobalDefine.vh"
`include "InstPattern.vh"

module Controler #(
  parameter INST_WIDTH = 32,
  parameter ALU_OP_WIDTH = 4,
  parameter PC_SEL_WIDTH = 2
)(
    input [INST_WIDTH - 1 : 0] inst_in,
    input                      inst_en_in,
    output is_mem_read,
    output is_mem_write,
    output [ALU_OP_WIDTH - 1 : 0] alu_op,
    output [PC_SEL_WIDTH - 1 : 0] PC_sel,
    output [ `ALU_SRC_WIDTH - 1 : 0] alu_src,
    output alu_zero_preset,
    output is_reg_write,
    output is_mem_to_reg,
    output [`MEM_MODE_WIDTH - 1 : 0] mem_width_out
);


  assign is_mem_read = ((`I_TYPE_LD_INPUT) ? 1'b1 : 1'b0) & inst_en_in;
  assign is_mem_write = ((`S_TYPE_INPUT) ? 1'b1 : 1'b0) & inst_en_in;

  assign is_mem_to_reg = (`I_TYPE_LD_INPUT) ? 1'b1 : 1'b0;


  assign alu_src = (`R_TYPE_INPUT) ? {`ALU_B_SRC_REG, `ALU_A_SRC_REG } :
                   (`I_TYPE_INPUT) ? {`ALU_B_SRC_IMM, `ALU_A_SRC_REG } :
                   (`S_TYPE_INPUT) ? {`ALU_B_SRC_IMM, `ALU_A_SRC_REG } :
                   (`B_TYPE_INPUT) ? {`ALU_B_SRC_REG, `ALU_A_SRC_REG } :
                   ((inst_in & `U_TYPE_MASK) == `INST_LUI) ? {`ALU_B_SRC_IMM, `ALU_A_SRC_REG } :
                   ((inst_in & `U_TYPE_MASK) == `INST_AUIPC) ? {`ALU_B_SRC_IMM, `ALU_A_SRC_PC } :
                   ((inst_in & `U_TYPE_MASK) == `INST_JAL) ? {`ALU_B_SRC_IMM, `ALU_A_SRC_REG } : {`ALU_B_SRC_REG, `ALU_A_SRC_REG};

assign is_reg_write = ((`R_TYPE_INPUT) ? 1'b1 :
                       (`I_TYPE_INPUT) ? 1'b1 :
                       (`S_TYPE_INPUT) ? 1'b0 :
                       (`B_TYPE_INPUT) ? 1'b0 :
                       (`U_TYPE_INPUT) ? 1'b1 : 1'b0) & inst_en_in;

  assign PC_sel = (`B_TYPE_INPUT) ? `PC_BRANCH :
                ((inst_in & `U_TYPE_MASK) == `INST_JAL) ? `PC_JUMP :
                ((inst_in & `I_TYPE_MASK) == `INST_JALR) ?`PC_JUMP_R : `PC_PLUS4;

  assign alu_op = ((inst_in & `R_TYPE_MASK) == `INST_ADD) ? `ALU_ADD :
                  ((inst_in & `R_TYPE_MASK) == `INST_SUB) ? `ALU_SUB :
                  ((inst_in & `R_TYPE_MASK) == `INST_AND) ? `ALU_AND :
                  ((inst_in & `R_TYPE_MASK) == `INST_OR ) ? `ALU_OR  :
                  ((inst_in & `R_TYPE_MASK) == `INST_XOR) ? `ALU_XOR :
                  ((inst_in & `R_TYPE_MASK) == `INST_SLL) ? `ALU_SLL :
                  ((inst_in & `R_TYPE_MASK) == `INST_SRL) ? `ALU_SRL :
                  ((inst_in & `R_TYPE_MASK) == `INST_SRA) ? `ALU_SRA :
                  ((inst_in & `R_TYPE_MASK) == `INST_SLT) ? `ALU_LT  :
                  ((inst_in & `R_TYPE_MASK) == `INST_SLTU) ? `ALU_LTU :
                  ((inst_in & `I_TYPE_MASK) == `INST_ADDI) ? `ALU_ADD :
                  ((inst_in & `I_TYPE_MASK) == `INST_SLTI) ? `ALU_LT  :
                  ((inst_in & `I_TYPE_MASK) == `INST_SLTIU) ? `ALU_LTU :
                  ((inst_in & `I_TYPE_MASK) == `INST_XORI) ? `ALU_XOR :
                  ((inst_in & `I_TYPE_MASK) == `INST_ORI ) ? `ALU_OR  :
                  ((inst_in & `I_TYPE_MASK) == `INST_ANDI) ? `ALU_AND :
                  ((inst_in & `I_TYPE_SHF_MASK) == `INST_SLLI) ? `ALU_SLL :
                  ((inst_in & `I_TYPE_SHF_MASK) == `INST_SRLI) ? `ALU_SRL :
                  ((inst_in & `I_TYPE_SHF_MASK) == `INST_SRAI) ? `ALU_SRA :
                  ((inst_in & `I_TYPE_MASK) == `INST_LW) ? `ALU_ADD :
                  ((inst_in & `I_TYPE_MASK) == `INST_LH) ? `ALU_ADD :
                  ((inst_in & `I_TYPE_MASK) == `INST_LB) ? `ALU_ADD :
                  ((inst_in & `I_TYPE_MASK) == `INST_LHU) ? `ALU_ADD :
                  ((inst_in & `I_TYPE_MASK) == `INST_LBU) ? `ALU_ADD :
                  ((inst_in & `I_TYPE_MASK) == `INST_JALR) ? `ALU_ADD :
                  ((inst_in & `S_TYPE_MASK) == `INST_SW) ? `ALU_ADD :
                  ((inst_in & `S_TYPE_MASK) == `INST_SH) ? `ALU_ADD :
                  ((inst_in & `S_TYPE_MASK) == `INST_SB) ? `ALU_ADD :
                  ((inst_in & `B_TYPE_MASK) == `INST_BLT) ? `ALU_LT  :
                  ((inst_in & `B_TYPE_MASK) == `INST_BEQ) ? `ALU_SUB :
                  ((inst_in & `B_TYPE_MASK) == `INST_BNE) ? `ALU_SUB :
                  ((inst_in & `B_TYPE_MASK) == `INST_BLTU) ? `ALU_LTU :
                  ((inst_in & `B_TYPE_MASK) == `INST_BGE) ? `ALU_LT  :
                  ((inst_in & `B_TYPE_MASK) == `INST_BGEU) ? `ALU_LTU :
                  ((inst_in & `U_TYPE_MASK) == `INST_LUI) ? `ALU_NONE :
                  ((inst_in & `U_TYPE_MASK) == `INST_AUIPC) ? `ALU_ADD :
                  ((inst_in & `U_TYPE_MASK) == `INST_JAL) ? `ALU_NONE :`ALU_NONE;

assign alu_zero_preset = ((inst_in & `B_TYPE_MASK) == `INST_BEQ) ? 1'b1 :
                         ((inst_in & `B_TYPE_MASK) == `INST_BLT) ? 1'b0 :
                         ((inst_in & `B_TYPE_MASK) == `INST_BNE) ? 1'b0 :
                         ((inst_in & `B_TYPE_MASK) == `INST_BLTU) ? 1'b0 :
                         ((inst_in & `B_TYPE_MASK) == `INST_BGE) ? 1'b1 :
                         ((inst_in & `B_TYPE_MASK) == `INST_BGEU) ? 1'b1 : 1'b0;

assign mem_width_out = ((inst_in & `I_TYPE_MASK) == `INST_LW) ? `MEM_WORD :
                   ((inst_in & `I_TYPE_MASK) == `INST_LH) ? `MEM_HALF :
                   ((inst_in & `I_TYPE_MASK) == `INST_LB) ? `MEM_BYTE :
                   ((inst_in & `I_TYPE_MASK) == `INST_LHU) ? `MEM_HALF_U :
                   ((inst_in & `I_TYPE_MASK) == `INST_LBU) ? `MEM_BYTE_U :
                   ((inst_in & `S_TYPE_MASK) == `INST_SW) ? `MEM_WORD :
                   ((inst_in & `S_TYPE_MASK) == `INST_SH) ? `MEM_HALF :
                   ((inst_in & `S_TYPE_MASK) == `INST_SB) ? `MEM_BYTE : `MEM_WORD;
endmodule
