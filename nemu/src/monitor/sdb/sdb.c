#include <isa.h>
#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
// hamza
#include "/home/hamza/prj/ysyx-workbench/nemu/include/memory/host.h"
#include "/home/hamza/prj/ysyx-workbench/nemu/include/memory/paddr.h"
#include "/home/hamza/prj/ysyx-workbench/nemu/include/memory/vaddr.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char *rl_gets()
{
  static char *line_read = NULL;

  if (line_read)
  {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read)
  {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args)
{
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args)
{
  return -1; //-1
}
/*TODO:hamza,sdb*/
// static void execute(uint64_t n);
static int cmd_si(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " "); // strtok()继续使用的时候，str必须写NULL
  printf("hsay:cmd_si\n");
  printf("hsay:arg=%s\n", arg);
  if (arg == NULL)
  {
    printf("hsay:arg=NULL\n");
    cpu_exec(1);
  }
  else // TODO此处需要判断是否为数字字符串
  {
    int arg_d = atoi(arg); // atoi()将数字字符串转int//如果不是数字字符串则返回0
    cpu_exec(arg_d);
    return 0;
  }
  return 0;
}

static int cmd_info(char *args)
{
  printf("hsay:in cmd_info()\n");
  /* extract the first argument */
  char *arg = strtok(NULL, " "); // strtok()继续使用的时候，str必须写NULL
  if (*arg == 'r')
  {
    printf("hsay:before isa_reg_display()\n");
    isa_reg_display();
    printf("hsay:after isa_reg_display()\n");
    return 0;
  }
  else if (*arg == 'w')
  {
    watchpoint_display();
    return 0;
  }
  else
  {
    printf("hsay:Option NOT found!\n");
    return 0;
  }
  return 0;
}

static int cmd_x(char *args)
{
  printf("hsay:in cmd_x()\n");
  /* extract the first argument */
  char *arg1 = strtok(NULL, " "); // strtok()继续使用的时候，str必须写NULL
  int len = atoi(arg1);           //扫描长度
  printf("hsay:len=%d\n", len);
  char *arg2 = strtok(NULL, " "); // strtok()继续使用的时候，str必须写NULL
  long unsigned int addr = 0;
  sscanf(arg2, "%lx", &addr); //把arg2存的字符串转成16进制数存入addr
  printf("hsay:addr=%#lx\n", addr);
  for (int i = 0; i < len; i++)
  {
    long unsigned int ret = vaddr_ifetch(addr + 4 * i, 4); //成功读出//TODO:超出mem范围将会崩溃，暂留//地址必须是4的整数倍（int）
    long unsigned int MBASE = 0x80000000;
    printf("mem[%ld]\t= %#010lx\n", (addr + 4 * i - MBASE) / 4, ret); // 010中的0是表示前面补零，10表示长度，需注意，10包括了0x这两位
  }
  return 0;
}

static int cmd_p(char *args)
{
  printf("cmd_p\n");
  return 0;
}

static int cmd_w(char *args)
{
  printf("cmd_w\n");
  return 0;
}

static int cmd_d(char *args)
{
  printf("cmd_d\n");
  return 0;
}
/*zamah*/
static int cmd_help(char *args);

static struct
{
  const char *name;
  const char *description;
  int (*handler)(char *); //函数指针
} cmd_table[] = {
    {"help", "Display informations about all supported commands", cmd_help},
    {"c", "Continue the execution of the program", cmd_c},
    {"q", "Exit NEMU", cmd_q},

    /* TODO: Add more commands */
    {"si", "Execute N cmd by single step and pause", cmd_si},
    {"info", "Print infomation of register or watchpoint", cmd_info},
    {"x", "Read N bytes memery begin with EXPR", cmd_x},
    {"p", "Calculate EXPR", cmd_p},
    {"w", "Watchpoint seting", cmd_w},
    {"d", "Delete watchpoint", cmd_d},

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args)
{
  /* extract the first argument */
  char *arg = strtok(NULL, " "); // strtok()继续使用的时候，str必须写NULL
  int i;

  if (arg == NULL)
  { //未给出arg，输出所有cmd的描述
    /* no argument given */
    for (i = 0; i < NR_CMD; i++)
    {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else
  {
    for (i = 0; i < NR_CMD; i++)
    { //输出对应arg的描述
      if (strcmp(arg, cmd_table[i].name) == 0)
      {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg); //没有匹配的arg描述
  }
  return 0;
}

void sdb_set_batch_mode()
{
  is_batch_mode = true;
}

void sdb_mainloop()
{
  if (is_batch_mode) //在parse_args()处有-b则运行sdb_set_batch_mode()
  {
    printf("hsay:is_batch_mode = true\n");
    cmd_c(NULL);
    return;
  }
  else
    printf("hsay:is_batch_mode = flase\n");

  for (char *str; (str = rl_gets()) != NULL;) //接收输入字符串
  {
    char *str_end = str + strlen(str); // str表示首地址，*str_end指向str的尾地址后一位。

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");   //以空格为分界将字符串分解，返回第一个字符串得到命令cmd
    printf("hsay:cmd = %s\n", cmd); //打印cmd
    if (cmd == NULL)
    {
      continue;
    } //若无输入则重新等待输入，continue到for

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1; //*args指向cmd尾地址后两位

    if (args >= str_end) //如果args大于等于str_end说明后面没有args
    {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i++)
    {
      if (strcmp(cmd, cmd_table[i].name) == 0) //匹配命令//strcmp：相当于str1-str2，0相等，负数，正数
      {
        if (cmd_table[i].handler(args) < 0) // handler：函数指针，运行cmd_help,cmd_si...等命令
        {
          return; //表示直接结束程序，不返回任何值//cmd_q会返回-1，则退出sdb_mainloop()
        }
        break;
      }
    }

    if (i == NR_CMD) //超过CMD列表数，未找到命令
    {
      printf("Unknown command '%s'\n", cmd);
    }
  }
}

void init_sdb()
{
  /* Compile the regular expressions. */
  init_regex(); //初始化正则表达式

  /* Initialize the watchpoint pool. */
  init_wp_pool(); //初始化监测点
}
