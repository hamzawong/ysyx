/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-02 17:35:54
 * @FilePath: /ysyx-workbench/nemu/include/cpu/ifetch.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CPU_IFETCH_H__

#include <memory/vaddr.h>

static inline uint32_t inst_fetch(vaddr_t *pc, int len)
{
  uint32_t inst = vaddr_ifetch(*pc, len);
  (*pc) += len; // TODO为什么不是pc+4//传入的是地址，所以*pc其实就是地址
  return inst;
}

#endif
