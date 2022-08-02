/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-26 21:52:21
 * @FilePath: /ysyx-workbench/nemu/include/device/map.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DEVICE_MAP_H__
#define __DEVICE_MAP_H__

#include <cpu/difftest.h>

typedef void (*io_callback_t)(uint32_t, int, bool);

uint8_t *new_space(int size);

typedef struct
{
  const char *name;
  // we treat ioaddr_t as paddr_t here
  paddr_t low;
  paddr_t high;
  void *space;
  io_callback_t callback;
} IOMap;

static inline bool map_inside(IOMap *map, paddr_t addr)
{
  return (addr >= map->low && addr <= map->high);
}

static inline int find_mapid_by_addr(IOMap *maps, int size, paddr_t addr)
{
  int i;
  for (i = 0; i < size; i++)
  {
    if (map_inside(maps + i, addr)) // addr在第几个maps的空间中
    {
      difftest_skip_ref(); // is_skip_ref设为1。//由于状态机视角下的nemu状态不唯一，所以跳过difftest
      return i;
    }
  }
  return -1;
}

void add_pio_map(const char *name, ioaddr_t addr,
                 void *space, uint32_t len, io_callback_t callback);
void add_mmio_map(const char *name, paddr_t addr,
                  void *space, uint32_t len, io_callback_t callback);

word_t map_read(paddr_t addr, int len, IOMap *map);
void map_write(paddr_t addr, int len, word_t data, IOMap *map);

#endif
