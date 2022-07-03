#include <common.h>
#ifndef CONFIG_TARGET_AM
#include <time.h>
#endif

void init_rand() {
  srand(MUXDEF(CONFIG_TARGET_AM, 0, time(0)));//MUXDEF(if(a),a:...,!a:...)//srand(seed)产生随机数种子，后面调用rand可得到随机数
}
