/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:49:39
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-27 10:12:03
 * @FilePath: /ysyx-workbench/abstract-machine/am/src/platform/nemu/ioe/timer.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <am.h>
#include <nemu.h>

void __am_timer_init()
{
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime)
{
  uptime->us = 0;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc)
{
  rtc->second = 0;
  rtc->minute = 0;
  rtc->hour = 0;
  rtc->day = 0;
  rtc->month = 0;
  rtc->year = 1900;
}
