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

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

#define TOKEN_NUM 32

enum {
  TK_NOTYPE = 256, TK_EQ, TK_NUM, TK_HEX, TK_REG

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},          // minus
  {"\\*", '*'},        // multiply
  {"/", '/'},          // divide
  {"==", TK_EQ},        // equal
  {"\\(", '('},        // left parenthesis
  {"\\)", ')'},        // right parenthesis
  {"\\b[0-9]+\\b", TK_NUM},     // number (0-9)
  {"\\$(\\$0|ra|[sgt]p|t[0-6]|a[0-7]|s([0-9]|1[0-1])|x([0-9]|1[0-9]|2[0-9]|31))", TK_REG},
  {"\\b0[xX][0-9a-fA-F]+\\b", TK_HEX}, // hexadecimal number
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[TOKEN_NUM] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;
        
        if (nr_token >= TOKEN_NUM) {
          printf("Too many tokens, max is %d\n", TOKEN_NUM);
          return false;
        }
        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case TK_NUM: case TK_HEX: case TK_REG:
            if (substr_len >= sizeof(tokens[nr_token].str)) {
              printf("Token too long at position %d\n%s\n%*.s^\n", position, e, position, "");
              return false;
            }
          default: {
            strncpy(tokens[nr_token].str, substr_start, substr_len);
            tokens[nr_token].str[substr_len] = '\0';
            tokens[nr_token].type = rules[i].token_type;
            nr_token++;
            break;
          }
        }
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_match(int p, int q, char* e) {
  int count = 0;
  for (int i = p; i <= q; i++) {
    if (tokens[i].type == '(') count++;
    else if (tokens[i].type == ')') count--;
    if (count < 0) {
      printf("Mismatched parentheses at positions %d\n%s\n%*.s^\n", p, e, p, "");
      return false;
    }
  }
  return count == 0;   
}

bool check_parentheses(int p, int q, char* e, bool *success) {
  if (!check_match(p, q, e)){
    *success = false;
    return false;
  }
  else if (tokens[p].type != '(' || tokens[q].type != ')') {
    return false;
  }
  else {
    if (check_match(p + 1, q - 1, e)) {
      return true;
    } else {
      return false;
    }
  }
}

int find_major(int p, int q, char* e) {
  int ret = -1;
  int par_count = 0;
  int last_op = 0;
  for (int i = p; i <= q; i++) {
    switch (tokens[i].type) {
      case TK_NUM: case TK_HEX: case TK_REG:
        break;
      case '(': 
        par_count++; break;
      case ')': {
        if (par_count <= 0) {
          printf("Mismatched parentheses in expression\n%s\n%*.s^\n", e, i, "");
          ret = -1;
          return ret;
        }
        par_count--; 
        break;
      }
      case '+': case '-': {
        ret = (last_op <= 2 && par_count == 0) ? i : ret;
        last_op = (par_count == 0) ? 2 : last_op;
        break;
      }
      case '*': case '/': {
        ret = (last_op <= 1 && par_count == 0) ? i : ret;
        last_op = (par_count == 0) ? 1 : last_op;
        break;
      }
      default: {
        printf("Invalid token type at position %d: %s\n%s\n%*.s^\n", i, tokens[i].str, e, i, "");
        ret = -1;
        return ret;
      }
    }
    //printf("%d, %d, %d\n", par_count, last_op, ret);
  }

  if (par_count != 0) {
    printf("Mismatched parentheses in expression\n%s\n%*.s^\n", e, p, "");
    ret = -1;
    return ret;
  }
  return ret;
}

word_t eval(int p, int q, char* e, bool *success){
  if (p > q){
    *success = false;
    printf("Invalid expression\n");
    return 0;
  }
  else if (p == q) {
    switch (tokens[p].type) {
      case TK_NUM:
        return strtol(tokens[p].str, NULL, 10);
      case TK_HEX:
        return strtol(tokens[p].str + 2, NULL, 16);
      case TK_REG: {
        TODO();
        break;
      }
      default:{
        *success = false;
        printf("Invalid token type at position %d: %s\n%s\n%*.s^\n", p, tokens[p].str, e, p, "");
        return 0;
      }
    }  
  }
  else if (check_parentheses(p, q, e, success)) {
    return eval(p + 1, q - 1, e, success);
  }
  else {
    if (!*success) {
      printf("Invalid expression, parentheses mismatch\n");
      return 0;
    }
    int op = find_major(p, q, e);
    if (op < 0) {
      *success = false;
      printf("Invalid expression, can't find major operator\n");
      return 0;
    }
    word_t val1 = eval(p, op - 1, e, success);
    if (!*success) {
      printf("Failed to evaluate left operand from position %d to %d\n%s\n%*.s^\n", p, op - 1, e, p, "");
      return 0;
    }
    word_t val2 = eval(op + 1, q, e, success);
    if (!*success) {
      printf("Failed to evaluate right operand from position %d to %d\n%s\n%*.s^\n", op + 1, q, e, op + 1, "");
      return 0;
    }

    word_t res;

    switch (tokens[op].type) {
      case '+':{
        res = val1 + val2;
        return res;
      }
      case '-':{
        res = val1 - val2;
        printf("val1: %u, val2: %u, res: %u\n", val1, val2, res);
        return res;
      }
      case '*':{
        res = val1 * val2;
        return res;
      }
      case '/': {
        if (val2 == 0) {
          *success = false;
          printf("Division by zero at position %d\n%s\n%*.s^\n", op, e, op, "");
          return 0;
        }
        res = (sword_t)val1 / (sword_t)val2;
        return res;
      }
      case TK_EQ:
        return val1 == val2;
      default: {
        *success = false;
        printf("Invalid operator at position %d: %s\n%s\n%*.s^\n", op, tokens[op].str, e, op, "");
        return 0;
      }
    }
  }


}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }
  //printf("eval\n");
  return eval(0, nr_token - 1, e, success);
}
