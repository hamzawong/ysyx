/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-02 17:53:12
 * @FilePath: /ysyx-workbench/nemu/src/memory/paddr.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <memory/host.h>
#include <memory/paddr.h>
#include <device/mmio.h>
#include <isa.h>

#if defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {}; //开辟pmem内存块，大小为CONFIG_MSIZE（128Mbytes）。以PG_ALIGN（4Kbytes）对齐。
#endif

uint8_t *guest_to_host(paddr_t paddr) //返回一个指针
{
  return pmem + (paddr - CONFIG_MBASE); // (paddr - CONFIG_MBASE)是guest的地址偏移，加上pmem则对应到host的地址
}

paddr_t host_to_guest(uint8_t *haddr)
{
  return (haddr - pmem) + CONFIG_MBASE; // haddr-pmem为地址增量，+CONFIG_MBASE为对应guest的地址
}

static word_t pmem_read(paddr_t addr, int len)
{
  word_t ret = host_read(guest_to_host(addr), len); //取出host的pmem地址里的值
  return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data)
{
  host_write(guest_to_host(addr), len, data);
}

static void out_of_bound(paddr_t addr)
{
  panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " FMT_WORD,
        addr, (paddr_t)CONFIG_MBASE, (paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1, cpu.pc);
}

void init_mem()
{
#if defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
#ifdef CONFIG_MEM_RANDOM
  uint32_t *p = (uint32_t *)pmem;
  int i;
  for (i = 0; i < (int)(CONFIG_MSIZE / sizeof(p[0])); i++)
  {
    p[i] = rand();
  }
#endif
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]",
      (paddr_t)CONFIG_MBASE, (paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1);
}

word_t paddr_read(paddr_t addr, int len)
{
  if (likely(in_pmem(addr)))                         // in_pmem()判断addr是否在内存范围内//likely()预测
    return pmem_read(addr, len);                     //取出host中pmem的值
  IFDEF(CONFIG_DEVICE, return mmio_read(addr, len)); // CONFIG_DEVICE=y
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data)
{
  if (likely(in_pmem(addr)))
  {
    pmem_write(addr, len, data);
    return;
  }
  IFDEF(CONFIG_DEVICE, mmio_write(addr, len, data); return );
  out_of_bound(addr);
}
