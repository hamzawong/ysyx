/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-06-30 16:53:43
 * @FilePath: /ysyx-workbench/nemu/include/memory/host.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#ifndef __MEMORY_HOST_H__
#define __MEMORY_HOST_H__

#include <common.h>

static inline word_t host_read(void *addr, int len)
{
  switch (len)
  {
  case 1:
    return *(uint8_t *)addr;
  case 2:
    return *(uint16_t *)addr;
  case 4:                     // len=4
    return *(uint32_t *)addr; //把地址转成指针，再取值
    IFDEF(CONFIG_ISA64, case 8
          : return *(uint64_t *)addr);
  default:
    MUXDEF(CONFIG_RT_CHECK, assert(0), return 0);
  }
}

static inline void host_write(void *addr, int len, word_t data)
{
  switch (len)
  {
  case 1:
    *(uint8_t *)addr = data;
    return;
  case 2:
    *(uint16_t *)addr = data;
    return;
  case 4:
    *(uint32_t *)addr = data;
    return;
    IFDEF(CONFIG_ISA64, case 8
          : *(uint64_t *)addr = data;
          return );
    IFDEF(CONFIG_RT_CHECK, default
          : assert(0));
  }
}

#endif
