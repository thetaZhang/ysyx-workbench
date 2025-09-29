// top of the tinyrisc CPU
`include "GlobalDefine.vh"
module top (
    input                        clk,
    input                        rst_n
);

  wire [  `ADDR_WIDTH - 1 : 0] pc;
  wire [  `ADDR_WIDTH - 1 : 0] pc_next;
  wire [`PC_SEL_WIDTH - 1 : 0] pc_sel;
  wire [`ALU_OP_WIDTH - 1 : 0] alu_op;
  wire                         alu_zero_preset;
  wire [  `DATA_WIDTH - 1 : 0] rs1_data;
  wire [  `DATA_WIDTH - 1 : 0] rs1_data_in;
  wire [  `DATA_WIDTH - 1 : 0] rs2_data;
  wire [  `DATA_WIDTH - 1 : 0] rd_data;
  wire [  `DATA_WIDTH - 1 : 0] imm;
  wire                         alu_src;
  wire [  `DATA_WIDTH - 1 : 0] ex_data_out;
  wire [  `DATA_WIDTH - 1 : 0] mem_data_out;
  wire                         mem_to_reg;
  wire [  `INST_WIDTH - 1 : 0] inst_in;
  wire [  `ADDR_WIDTH - 1 : 0] inst_addr_out;
  wire                         inst_ce;

  wire  [`DATA_WIDTH - 1 : 0] data_rd;
  wire  [`ADDR_WIDTH - 1 : 0] data_addr;
  wire                        data_we;
  wire                        data_ce;
  wire  [7 : 0]               data_wmask;
  wire  [`DATA_WIDTH - 1 : 0] data_wr;
  wire  [`MEM_MODE_WIDTH - 1 : 0] mem_width;


  export "DPI-C" function inst_probe;

  function int unsigned inst_probe();
    return inst_in;
  endfunction

  // inst_mem
  InstMem inst_mem_u (
      .ce           (inst_ce),
      .inst_addr_in (inst_addr_out),
      .inst_out     (inst_in)
  );

  // data_mem
  DataMem data_mem_u (
      .clk          (clk),
      .rst_n        (rst_n),
      .ce_in        (data_ce),
      .we_in        (data_we),
      .data_addr_in (data_addr),
      .data_wr_in   (data_wr),
      .wmask_in     (data_wmask),
      .data_rd_out  (data_rd)
  );

  // IF

  assign inst_addr_out = pc;

  tinyrisc_IF IF_u (
      .clk   (clk),
      .rst_n (rst_n),
      .pc_in (pc_next),
      .pc_out(pc),
      .inst_ce_out(inst_ce)
  );


  // ID

  tinyrisc_ID ID_u (
      .clk            (clk),
      .rst_n          (rst_n),
      .inst_in        (inst_in),
      .imm_out        (imm),
      .rs1_data_out   (rs1_data),
      .rs2_data_out   (rs2_data),
      .rd_data_in     (rd_data),
      .alu_op_out     (alu_op),
      .pc_sel_out     (pc_sel),
      .alu_src_out    (alu_src),
      .data_we_out    (data_we),
      .data_ce_out    (data_ce),
      .mem_to_reg_out (mem_to_reg),
      .alu_zero_preset(alu_zero_preset),
      .mem_width_out  (mem_width)
  );
  assign rs1_data_in =((inst_in & `U_TYPE_MASK) == `INST_AUIPC) ?  pc : rs1_data;
  // EX
  tinyrisc_EX EX_u (
      .rs1_data_in    (rs1_data_in),
      .rs2_data_in    (rs2_data),
      .imm_in         (imm),
      .alu_op_in      (alu_op),
      .alu_src_in     (alu_src),
      .alu_zero_preset(alu_zero_preset),
      .ex_data_out    (ex_data_out),
      .pc_sel_in      (pc_sel),
      .pc_in          (pc),
      .pc_out         (pc_next)
  );

  // MEM
  assign data_addr = ex_data_out[`ADDR_WIDTH - 1 : 0];
  assign data_wr   = rs2_data;
  assign data_wmask = (mem_width == `MEM_WORD) ? 8'h0f :
                      (mem_width == `MEM_HALF) ? 8'h03 :
                      (mem_width == `MEM_BYTE) ? 8'h01 :
                      (mem_width == `MEM_HALF_U) ? 8'h03 :
                      (mem_width == `MEM_BYTE_U) ? 8'h01 : 8'h0f;

  assign mem_data_out = (mem_width == `MEM_WORD) ? data_rd :
                        (mem_width == `MEM_HALF) ? {{(`DATA_WORD - `DATA_HALF){data_rd[`DATA_HALF - 1]}}, data_rd[`DATA_HALF - 1 : 0]} :
                        (mem_width == `MEM_BYTE) ? {{(`DATA_WORD - `DATA_BYTE){data_rd[`DATA_BYTE - 1]}}, data_rd[`DATA_BYTE - 1 : 0]} :
                        (mem_width == `MEM_BYTE_U) ? {24'b0, data_rd[`DATA_BYTE - 1 : 0]} :
                        (mem_width == `MEM_HALF_U) ? {16'b0, data_rd[`DATA_HALF - 1 : 0]} : data_rd;

  // WB
  assign rd_data       = mem_to_reg ? mem_data_out : ex_data_out;



endmodule
