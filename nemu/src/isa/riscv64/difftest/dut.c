/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-23 19:25:53
 * @FilePath: /ysyx-workbench/nemu/src/isa/riscv64/difftest/dut.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <isa.h>
#include <cpu/difftest.h>
#include "../local-include/reg.h"

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) // TODO: 比较ref.gpr和DUT的gpr
{
  for (int i = 0; i < 32; ++i)
  {
    if (ref_r->gpr[i] != cpu.gpr[i])
    {
      printf("hsay: gpr[%d] neq\n", i);
      printf("right is %lx, wrong is %lx\n", ref_r->gpr[i], cpu.gpr[i]);
      return false;
    }
  }

  if (ref_r->pc == pc)
  {
    return true;
  }
  else
  {
    printf("ref_pc = %lx dut_pc%lx\n", ref_r->pc, pc);
    printf("hsay: pc neq\n");
    return false;
  }
}

void isa_difftest_attach()
{
}
