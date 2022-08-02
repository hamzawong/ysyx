/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-25 17:06:21
 * @FilePath: /ysyx-workbench/nemu/src/memory/vaddr.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <isa.h>
#include <memory/paddr.h>

word_t vaddr_ifetch(vaddr_t addr, int len)
{
  return paddr_read(addr, len);
}

word_t vaddr_read(vaddr_t addr, int len)
{
  // printf("read: %lx\n", addr);
  return paddr_read(addr, len);
}

void vaddr_write(vaddr_t addr, int len, word_t data)
{
  // printf("write: %lx\n", addr);
  paddr_write(addr, len, data);
}
