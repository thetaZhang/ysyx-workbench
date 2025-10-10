`ifndef DPI_C_VH
`define DPI_C_VH

import "DPI-C" function int unsigned pmem_read(input int unsigned raddr);
import "DPI-C" function void pmem_write(input int unsigned waddr, input int unsigned wdata, input byte unsigned wmask);

`endif
