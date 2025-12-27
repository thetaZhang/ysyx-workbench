// top of the tinyrisc CPU
`include "GlobalDefine.vh"
module core (
    input                        clk,
    input                        rst_n,

    // inst_mem
    output                          ifu_reqValid,
    output [`ADDR_WIDTH - 1 : 0]    ifu_addr,
    input                           ifu_respValid,
    input   [`DATA_WIDTH - 1 : 0]   ifu_rdata,

    //data_mem
    output        lsu_reqValid,
    output [`ADDR_WIDTH - 1 :0] lsu_addr,
    output        lsu_wen,
    output [`DATA_WIDTH - 1 :0] lsu_wdata,
    output [ 7:0] lsu_wmask,
    input         lsu_respValid,
    input  [`DATA_WIDTH - 1 :0] lsu_rdata
);

  wire [  `ADDR_WIDTH - 1 : 0] pc_next;
  wire [  `ADDR_WIDTH - 1 : 0] pc;
  wire [`PC_SEL_WIDTH - 1 : 0] pc_sel;
  wire [`ALU_OP_WIDTH - 1 : 0] alu_op;
  wire                         alu_zero_preset;
  wire [  `DATA_WIDTH - 1 : 0] rs1_data;
  wire [  `DATA_WIDTH - 1 : 0] rs2_data;
  wire [  `DATA_WIDTH - 1 : 0] rd_data;
  wire [  `DATA_WIDTH - 1 : 0] imm;
  wire [ `ALU_SRC_WIDTH - 1 : 0] alu_src;
  wire [  `DATA_WIDTH - 1 : 0] ex_data_out;
  wire [  `DATA_WIDTH - 1 : 0] mem_data;
  wire                         mem_to_reg;


  wire                            mem_write;
  wire                            mem_read;
  wire  [`MEM_MODE_WIDTH - 1 : 0] mem_width;

  wire                           reg_we;
  wire [`REG_ADDR_WIDTH - 1 : 0] rd_addr;
  wire [`REG_ADDR_WIDTH - 1 : 0] rs1_addr;
  wire [`REG_ADDR_WIDTH - 1 : 0] rs2_addr;

  wire [  `DATA_WIDTH - 1 : 0]   inst_in;
  wire                           inst_en;

  wire mem_ready;
  wire exec_ready;


  export "DPI-C" function inst_probe;

  function int unsigned inst_probe();
    return inst_in;
  endfunction


  

  IFU ifu (
      .clk   (clk),
      .rst_n (rst_n),
      .pc_in (pc_next),
      .pc_out(pc),
      .inst_out(inst_in),
      .inst_en_out(inst_en),
      .exec_ready_in(exec_ready),
      .ifu_reqValid(ifu_reqValid),
      .ifu_addr(ifu_addr),
      .ifu_respValid(ifu_respValid),
      .ifu_rdata(ifu_rdata)
  );


  // ID

  IDU idu (
      .clk            (clk),
      .rst_n          (rst_n),
      .inst_in        (inst_in),
      .inst_en_in     (inst_en),
      .imm_out        (imm),
      .alu_op_out     (alu_op),
      .pc_sel_out     (pc_sel),
      .alu_src_out    (alu_src),
      .mem_read_out    (mem_read),
      .mem_write_out   (mem_write),
      .mem_to_reg_out (mem_to_reg),
      .alu_zero_preset(alu_zero_preset),
      .mem_width_out  (mem_width),
      .reg_we_out     (reg_we),
      .rd_addr_out    (rd_addr),
      .rs1_addr_out   (rs1_addr),
      .rs2_addr_out   (rs2_addr)
  );

  RegFile regfile_u (
      .clk  (clk),
      .rst_n(rst_n),

      .reg_we_in(reg_we && exec_ready),
      .addr_wr  (rd_addr),
      .data_wr  (rd_data),

      .addr_rd_1(rs1_addr),
      .data_rd_1(rs1_data),

      .addr_rd_2(rs2_addr),
      .data_rd_2(rs2_data)
  );

  // EX
  EXU exu (
      .rs1_data_in    (rs1_data),
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

  
  LSU lsu (
      .clk           (clk),
      .rst_n         (rst_n),
      .mem_data_out  (mem_data),
      .mem_width_in  (mem_width),
      .data_wr_in    (rs2_data),
      .data_addr_in  (ex_data_out[`ADDR_WIDTH - 1 : 0]),
      .mem_write_in  (mem_write),
      .mem_read_in   (mem_read),
      .mem_ready_out (mem_ready),
      .lsu_reqValid  (lsu_reqValid),
      .lsu_addr      (lsu_addr),
      .lsu_wen       (lsu_wen),
      .lsu_wdata     (lsu_wdata),
      .lsu_wmask     (lsu_wmask),
      .lsu_respValid (lsu_respValid),
      .lsu_rdata     (lsu_rdata)
  );

  // WB
  WBU wbu (
      .clk            (clk),
      .rst_n          (rst_n),
      .mem_to_reg_in  (mem_to_reg),
      .mem_en_in      (mem_read || mem_write),
      .mem_ready_in   (mem_ready),
      .mem_data_in    (mem_data),
      .ex_data_in     (ex_data_out),
      .wb_data_out    (rd_data),
      .exec_ready_out (exec_ready)
  );


endmodule
