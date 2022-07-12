/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-12 14:56:07
 * @FilePath: /ysyx-workbench/nemu/src/isa/riscv64/reg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <isa.h>
#include "local-include/reg.h"

const char *regs[] = {
    "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
    "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
    "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
    "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"};

void isa_reg_display()
{
  printf("hsay:in isa_reg_display()\n");
  int len_regs = sizeof(regs) / sizeof(regs[0]);
  for (int i = 0; i < len_regs; i++)
  {
    printf("Value of regs[%d]\t%s\t = %#x\n", i, regs[i], *regs[i]);
  }
  return;
}

word_t isa_reg_str2val(const char *s, bool *success)
{
  return 0;
}
