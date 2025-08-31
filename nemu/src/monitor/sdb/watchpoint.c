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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char* expr;
  word_t last_val;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

static WP* new_wp(){
  assert(free_);
  WP* ret = free_;
  free_ = free_ -> next;
  ret -> next = head;
  head = ret;
  return ret;
}

static void free_wp(WP *wp){

  if (wp == head){
    head = head -> next;
  }
  else {
    WP* p = head;
    while (p && p->next != wp) {
      p = p->next;
    }
    assert(p);
    p -> next = wp -> next;
  }

  wp -> next = free_;
  free_ = wp;
}

void wp_add(char* expression){
  
  bool success = true;
  word_t val = expr(expression, &success);
  
  if (!success) {
    printf("Failed to evaluate watchpoint expression: %s\n", expression);
    return;
  }
  WP* wp = new_wp();
  printf("here/%u\n", val);
  strcpy(wp->expr, expression);
  wp->last_val = val;
  printf("Watchpoint %d: %s\n", wp->NO, wp->expr);
}

void wp_remove(int no){
  assert(no >= 0 && no < NR_WP);
  free_wp(&wp_pool[no]);
  printf("Watchpoint %d removed\n", no);
}

void wp_display(){
  WP* p = head;
  if (!p){
    printf("No watchpoints\n");
    return;
  }
  printf("%-8s%-8s\n", "Num", "What");
  while (p) {
    printf("%-8d%-8s\n", p->NO, p->expr);
    p = p -> next;
  }
}

bool wp_difftest(){
  WP* p = head;
  bool success = true;
  bool triggered = false;
  while(p){
    word_t val = expr(p->expr, &success);
    if (!success) {
      printf("Failed to evaluate watchpoint %d: %s\n", p->NO, p->expr);
      return false;
    }
    if (val != p->last_val) {
      printf("Watchpoint %d triggered: %s, old: %u, new: %u\n", p->NO, p->expr, p->last_val, val);
      p->last_val = val;
      triggered = true;
    }
    p = p -> next;
  }
  return triggered;
}