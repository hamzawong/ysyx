/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-02 17:54:19
 * @FilePath: /ysyx-workbench/nemu/src/isa/riscv32/init.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <isa.h>
#include <memory/paddr.h>

// this is not consistent with uint8_t
// but it is ok since we do not access the array directly
static const uint32_t img[] = {
    0x8000b702, // lui t0,0x80000
    0x0002a023, // sw  zero,0(t0)
    0x0002a503, // lw  a0,0(t0)
    0x00100073, // ebreak (used as nemu_trap)
};

static void restart()
{
  /* Set the initial program counter. */
  cpu.pc = RESET_VECTOR;

  /* The zero register is always 0. */
  cpu.gpr[0] = 0;
}

void init_isa()
{
  /* Load built-in image. */
  memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img)); //把img写到host里

  /* Initialize this virtual computer system. */
  restart();
}
