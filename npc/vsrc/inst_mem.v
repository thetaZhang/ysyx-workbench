module inst_mem(
  input ce,
  input [`ADDR_WIDTH - 1 : 0] inst_addr_in,
  output[`INST_WIDTH - 1 : 0] inst_out
);

  import "DPI-C" function int pmem_read(input int raddr);

  assign inst_out = ce ? pmem_read(inst_addr_in) : `INST_WIDTH'b0;


endmodule
