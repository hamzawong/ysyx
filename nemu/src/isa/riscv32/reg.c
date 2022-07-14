/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-13 16:48:07
 * @FilePath: /ysyx-workbench/nemu/src/isa/riscv32/reg.c
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
}

word_t isa_reg_str2val(const char *s, bool *success)
{
  if (strcmp("pc", s) == 0)
  {
    *success = true;
    return cpu.pc;
  }
  for (int i = 0; i < 32; i++)
  {
    if (strcmp(regs[i], s) == 0)
    {
      *success = true;
      return gpr(i);
    }
  }
  *success = false;
  return 0;
}
