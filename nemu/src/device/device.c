/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-01 10:30:30
 * @FilePath: /ysyx-workbench/nemu/src/device/device.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <common.h>
#include <utils.h>
#include <device/alarm.h>
#ifndef CONFIG_TARGET_AM
#include <SDL2/SDL.h>
#endif

void init_map();
void init_serial();
void init_timer();
void init_vga();
void init_i8042();
void init_audio();
void init_disk();
void init_sdcard();
void init_alarm();

void send_key(uint8_t, bool);
void vga_update_screen();

void device_update()
{
  static uint64_t last = 0;
  uint64_t now = get_time();
  if (now - last < 1000000 / TIMER_HZ) // TODO?？10000000/60什么意思
  {
    return;
  }
  last = now; //更新时间

  IFDEF(CONFIG_HAS_VGA, vga_update_screen()); // false

#ifndef CONFIG_TARGET_AM
  SDL_Event event;
  while (SDL_PollEvent(&event))
  {
    switch (event.type)
    {
    case SDL_QUIT:
      nemu_state.state = NEMU_QUIT;
      break;
#ifdef CONFIG_HAS_KEYBOARD
    // If a key was pressed
    case SDL_KEYDOWN:
    case SDL_KEYUP:
    {
      uint8_t k = event.key.keysym.scancode;
      bool is_keydown = (event.key.type == SDL_KEYDOWN);
      send_key(k, is_keydown);
      break;
    }
#endif
    default:
      break;
    }
  }
#endif
}

void sdl_clear_event_queue()
{
#ifndef CONFIG_TARGET_AM
  SDL_Event event;
  while (SDL_PollEvent(&event))
    ;
#endif
}

void init_device()
{
  IFDEF(CONFIG_TARGET_AM, ioe_init());
  init_map();

  IFDEF(CONFIG_HAS_SERIAL, init_serial());
  IFDEF(CONFIG_HAS_TIMER, init_timer());
  IFDEF(CONFIG_HAS_VGA, init_vga());
  IFDEF(CONFIG_HAS_KEYBOARD, init_i8042());
  IFDEF(CONFIG_HAS_AUDIO, init_audio());
  IFDEF(CONFIG_HAS_DISK, init_disk());
  IFDEF(CONFIG_HAS_SDCARD, init_sdcard());

  IFNDEF(CONFIG_TARGET_AM, init_alarm());
}
