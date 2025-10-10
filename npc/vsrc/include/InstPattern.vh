`ifndef INSTPATTERN_VH
`define INSTPATTERN_VH 
`define R_TYPE_INPUT ((inst_in & `R_TYPE_MASK) == `INST_ADD) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_SUB) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_AND) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_OR ) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_XOR) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_SLL) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_SRL) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_SRA) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_SLT) || \
                     ((inst_in & `R_TYPE_MASK) == `INST_SLTU)

`define I_TYPE_INPUT ((inst_in & `I_TYPE_MASK) == `INST_ADDI) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_SLTI) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_SLTIU) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_XORI) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_ORI ) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_ANDI) || \
                     ((inst_in & `I_TYPE_SHF_MASK) == `INST_SLLI) || \
                     ((inst_in & `I_TYPE_SHF_MASK) == `INST_SRLI) || \
                     ((inst_in & `I_TYPE_SHF_MASK) == `INST_SRAI) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_LW  ) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_LH  ) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_LB  ) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_LHU ) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_LBU ) || \
                     ((inst_in & `I_TYPE_MASK) == `INST_JALR)

`define I_TYPE_LD_INPUT ((inst_in & `I_TYPE_MASK) == `INST_LW  ) || \
                         ((inst_in & `I_TYPE_MASK) == `INST_LH  ) || \
                         ((inst_in & `I_TYPE_MASK) == `INST_LB  ) || \
                         ((inst_in & `I_TYPE_MASK) == `INST_LHU ) || \
                         ((inst_in & `I_TYPE_MASK) == `INST_LBU )

`define S_TYPE_INPUT ((inst_in & `S_TYPE_MASK) == `INST_SW  ) || \
                     ((inst_in & `S_TYPE_MASK) == `INST_SH  ) || \
                     ((inst_in & `S_TYPE_MASK) == `INST_SB  )

`define B_TYPE_INPUT ((inst_in & `B_TYPE_MASK) == `INST_BLT) || \
                     ((inst_in & `B_TYPE_MASK) == `INST_BEQ) || \
                     ((inst_in & `B_TYPE_MASK) == `INST_BNE) || \
                     ((inst_in & `B_TYPE_MASK) == `INST_BLTU) || \
                     ((inst_in & `B_TYPE_MASK) == `INST_BGE) || \
                     ((inst_in & `B_TYPE_MASK) == `INST_BGEU)

`define U_TYPE_INPUT ((inst_in & `U_TYPE_MASK) == `INST_JAL) || \
                     ((inst_in & `U_TYPE_MASK) == `INST_LUI) || \
                     ((inst_in & `U_TYPE_MASK) == `INST_AUIPC)
`endif
