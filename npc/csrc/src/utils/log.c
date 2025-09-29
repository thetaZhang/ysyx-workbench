/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <common.h>

extern uint64_t g_nr_guest_inst;

#ifndef CONFIG_TARGET_AM

#ifdef CONFIG_ITRACE

static IRingBuf iringbuf = {
  .length = 16,
  .start = 0,
  .end = 0,
};

void iringbuf_push(char* logbuf){
  if(iringbuf.end == iringbuf.start){
    iringbuf.start = (iringbuf.start + 1) % iringbuf.length;
  }
  strcpy(iringbuf.buf[iringbuf.end], logbuf);
  iringbuf.end = (iringbuf.end + 1) % iringbuf.length;

}

void iringbuf_display(){
  for (int i = 0;i < iringbuf.length;i++){
    int idx = (iringbuf.start + i) % iringbuf.length;
    printf("%s\n", iringbuf.buf[idx]);
  }
}

void iringbuf_free(){
  for (int i = 0;i < iringbuf.length;i++){
    free(iringbuf.buf[i]);
  }
  free(iringbuf.buf);
}

#endif

FILE *log_fp = NULL;

void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
#ifdef CONFIG_ITRACE
  iringbuf.buf = (char**)malloc(sizeof(char*) * iringbuf.length);
  Assert(iringbuf.buf, "Can not malloc for instruction ring buffer");
  for (int i = 0;i < iringbuf.length;i++){
    iringbuf.buf[i] = (char*)malloc(sizeof(char) * 128);
    Assert(iringbuf.buf[i], "Can not malloc for instruction ring buffer");
    memset(iringbuf.buf[i], 0, sizeof(char) * 128);
  }
#endif
}

bool log_enable() {
  return MUXDEF(CONFIG_TRACE, (g_nr_guest_inst >= CONFIG_TRACE_START) &&
         (g_nr_guest_inst <= CONFIG_TRACE_END), false);
}
#endif
