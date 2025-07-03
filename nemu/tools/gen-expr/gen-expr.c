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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static char *buf_ptr = NULL;
static char *buf_end = buf + (sizeof(buf)/sizeof(buf[0]));

static int choose(int n) {
  return rand() % n;
}

static void gen_space(){
  int num = choose(4);
  if (buf_ptr + 1 < buf_end){
    int len = snprintf(buf_ptr, buf_end - buf_ptr, "%*s", num, "");
    if (len > 0) {
      buf_ptr += len;
    }
  }
}

static void gen_num(){
  int num = choose(INT8_MAX);
  if (buf_ptr + 1 < buf_end){
    int len = 0;
    if (choose(2) == 0) {
      len = snprintf(buf_ptr, buf_end - buf_ptr, "%d", num);
    }
    else{
      len = snprintf(buf_ptr, buf_end - buf_ptr, "0x%x", num);
    }
    if (len > 0) {
      buf_ptr += len;
    }
  }
  gen_space();
}

static void gen_char(char c) {
  if (buf_ptr + 1 < buf_end) {
    int len = snprintf(buf_ptr, buf_end - buf_ptr, "%c", c);
    if (len > 0) {
      buf_ptr += len;
    }
  }
}

static int gen_rand_op() {
  static const char ops[] = {'+', '-', '*', '/'};
  if (buf_ptr + 1 < buf_end) {
    int op_index = choose(sizeof(ops) / sizeof(ops[0]));
    int len = snprintf(buf_ptr, buf_end - buf_ptr, " %c ", ops[op_index]);
    if (len > 0) {
      buf_ptr += len;
    }
  }
}

static void gen_rand_expr() {
  switch (choose(3))
  {
  case 0: gen_num(); break;
  case 1: gen_char('('); gen_rand_expr(); gen_char(')'); break;
  default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
  } 
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr -Wall -Werror");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
