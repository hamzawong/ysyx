/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-15 17:27:13
 * @FilePath: /ysyx-workbench/nemu/include/cpu/ifetch.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CPU_IFETCH_H__

#include <memory/vaddr.h>

static inline uint32_t inst_fetch(vaddr_t *pc, int len)
{
  uint32_t inst = vaddr_ifetch(*pc, len); //*pc是指令的地址
  (*pc) += len;                           // 传入的是地址，所以(*pc)是指针的值//每条指令4字节//这里即改变snpc的值
  return inst;                            //返回取出的指令
}

#endif
