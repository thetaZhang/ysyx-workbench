// instruction decode
`include "GlobalDefine.vh"
module IDU (
    input clk,
    /* verilator lint_off UNUSEDSIGNAL */
    input rst_n,
     /* verilator lint_off UNUSEDSIGNAL */

    input [`INST_WIDTH - 1 : 0] inst_in,
    input                       inst_en_in,

    output [`DATA_WIDTH - 1 : 0] imm_out,

    output [`ALU_OP_WIDTH - 1 : 0] alu_op_out,
    output [`PC_SEL_WIDTH - 1 : 0] pc_sel_out,
    output [ `ALU_SRC_WIDTH - 1 : 0] alu_src_out,
    output                         mem_write_out,
    output                         mem_read_out,
    output                         mem_to_reg_out,
    output                         alu_zero_preset,
    output [`MEM_MODE_WIDTH - 1 : 0] mem_width_out,

    //regfile
    output                           reg_we_out,
    output [`REG_ADDR_WIDTH - 1 : 0] rd_addr_out,
    output [`REG_ADDR_WIDTH - 1 : 0] rs1_addr_out,
    output [`REG_ADDR_WIDTH - 1 : 0] rs2_addr_out

);

  
  
  

  import "DPI-C" function void npc_trap();

  always @(posedge clk) begin
    if (inst_in == `INST_EBREAK) begin
      // $display("ebreak inst, exiting simulation.");
      npc_trap();
    end
  end



  ImmGen #(
      .INST_WIDTH(`INST_WIDTH),
      .IMM_WIDTH (`DATA_WIDTH)
  ) immgen_u (
      .inst_in(inst_in),
      .imm_out(imm_out)
  );


  InstDecoder #(
      .INST_WIDTH(`INST_WIDTH),
      .REG_ADDR_WIDTH(`REG_ADDR_WIDTH)
  ) inst_decoder_u (
      .inst_in (inst_in),
      .rs1_addr(rs1_addr_out),
      .rs2_addr(rs2_addr_out),
      .rd_addr (rd_addr_out)
  );

  Controler #(
      .INST_WIDTH  (`INST_WIDTH),
      .ALU_OP_WIDTH(`ALU_OP_WIDTH),
      .PC_SEL_WIDTH(`PC_SEL_WIDTH)
  ) controler_u (
      .inst_in(inst_in),
      .inst_en_in(inst_en_in),
      .is_mem_read    (mem_read_out),
      .is_mem_write   (mem_write_out),
      .alu_op         (alu_op_out),
      .PC_sel         (pc_sel_out),
      .alu_src        (alu_src_out),
      .alu_zero_preset(alu_zero_preset),
      .is_reg_write   (reg_we_out),
      .is_mem_to_reg  (mem_to_reg_out),
      .mem_width_out  (mem_width_out)
  );


endmodule
