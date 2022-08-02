/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-26 15:40:04
 * @FilePath: /ysyx-workbench/nemu/src/device/io/mmio.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <device/map.h>

#define NR_MAP 16

static IOMap maps[NR_MAP] = {};
static int nr_map = 0;

static IOMap *fetch_mmio_map(paddr_t addr)
{
  int mapid = find_mapid_by_addr(maps, nr_map, addr); //找到map的ID
  return (mapid == -1 ? NULL : &maps[mapid]);         //返回需要读写的maps的地址
}

/* device interface */
void add_mmio_map(const char *name, paddr_t addr, void *space, uint32_t len, io_callback_t callback)
{
  assert(nr_map < NR_MAP);
  maps[nr_map] = (IOMap){.name = name, .low = addr, .high = addr + len - 1, .space = space, .callback = callback};
  Log("Add mmio map '%s' at [" FMT_PADDR ", " FMT_PADDR "]",
      maps[nr_map].name, maps[nr_map].low, maps[nr_map].high);

  nr_map++;
}

/* bus interface */
word_t mmio_read(paddr_t addr, int len)
{
  return map_read(addr, len, fetch_mmio_map(addr));
}

void mmio_write(paddr_t addr, int len, word_t data)
{
  map_write(addr, len, data, fetch_mmio_map(addr));
}
