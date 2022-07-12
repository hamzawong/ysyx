/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-11 11:47:22
 * @FilePath: /nemu/tools/gen-expr/gen-expr.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AETO
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
    "#include <stdio.h>\n"
    "int main() { "
    "  unsigned result = %s; "
    "  printf(\"%%u\", result); "
    "  return 0; "
    "}";
static void gen(char *s);
static void gen_num();
static void gen_rand_op();
static char *op[] = {"+", "-", "*", "/"};

uint32_t choose(uint32_t n)
{
  return rand() % n;
}

static void gen_rand_expr(int initialize)
{
  if (initialize)
    buf[0] = '\0';
  switch (choose(4))
  {
  case 0:
  case 1:
    gen_num();
    break;
  case 2:
    gen("(");
    gen_rand_expr(0);
    gen(")");
    break;
  default:
    gen_rand_expr(0);
    gen_rand_op();
    gen_rand_expr(0);
    break;
  }
}

static void gen(char *s)
{
  strcat(buf, s);
}

static void gen_num()
{
  int32_t rnd;
  rnd = choose(1000);
  char temp[32] = {};
  sprintf(temp, "%u", rnd); //把rnd写入temp
  strcat(buf, temp);        //把temp连接到buf
}

static void gen_rand_op()
{
  char *rand_op = op[choose(4)];
  strcat(buf, rand_op);
}

int main(int argc, char *argv[])
{
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1)
  {
    sscanf(argv[1], "%d", &loop); //给loop赋值
  }
  int i;
  for (i = 0; i < loop; i++)
  {
    gen_rand_expr(1);
    if (strlen(buf) > 31)
    {
      continue;
    }

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp); //把code_buf写入fp
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    ret = system("cp /tmp/.code.c /tmp/test.c");
    if (ret != 0)
      continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result); //计算结果存到result
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
