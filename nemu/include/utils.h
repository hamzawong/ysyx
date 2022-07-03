/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-06-29 14:58:00
 * @FilePath: /ysyx-workbench/nemu/include/utils.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AEz
 */
#ifndef __UTILS_H__
#define __UTILS_H__

#include <common.h>

// ----------- state -----------

enum
{
  NEMU_RUNNING,
  NEMU_STOP,
  NEMU_END,
  NEMU_ABORT,
  NEMU_QUIT
};

typedef struct
{
  int state;         //状态
  vaddr_t halt_pc;   //虚拟地址
  uint32_t halt_ret; //??
} NEMUState;

extern NEMUState nemu_state;

// ----------- timer -----------

uint64_t get_time();

// ----------- log -----------

#define ANSI_FG_BLACK "\33[1;30m"
#define ANSI_FG_RED "\33[1;31m"
#define ANSI_FG_GREEN "\33[1;32m"
#define ANSI_FG_YELLOW "\33[1;33m"
#define ANSI_FG_BLUE "\33[1;34m"
#define ANSI_FG_MAGENTA "\33[1;35m"
#define ANSI_FG_CYAN "\33[1;36m"
#define ANSI_FG_WHITE "\33[1;37m"
#define ANSI_BG_BLACK "\33[1;40m"
#define ANSI_BG_RED "\33[1;41m"
#define ANSI_BG_GREEN "\33[1;42m"
#define ANSI_BG_YELLOW "\33[1;43m"
#define ANSI_BG_BLUE "\33[1;44m"
#define ANSI_BG_MAGENTA "\33[1;35m"
#define ANSI_BG_CYAN "\33[1;46m"
#define ANSI_BG_WHITE "\33[1;47m"
#define ANSI_NONE "\33[0m"

#define ANSI_FMT(str, fmt) fmt str ANSI_NONE

#define log_write(...) IFDEF(         \
    CONFIG_TARGET_NATIVE_ELF,         \
    do {                              \
      extern FILE *log_fp;            \
      extern bool log_enable();       \
      if (log_enable())               \
      {                               \
        fprintf(log_fp, __VA_ARGS__); \
        fflush(log_fp);               \
      }                               \
    } while (0))

#define _Log(...)           \
  do                        \
  {                         \
    printf(__VA_ARGS__);    \
    log_write(__VA_ARGS__); \
  } while (0)

#endif
