#include <cpu/cpu.h>
#include <cpu/decode.h>
#include <cpu/difftest.h>
#include <locale.h>

/* The assembly code of instructions executed is only output to the screen
 * when the number of instructions executed is less than this value.
 * This is useful when you use the `si' command.
 * You can modify this value as you want.
 */
#define MAX_INST_TO_PRINT 10

CPU_state cpu = {};
uint64_t g_nr_guest_inst = 0;
static uint64_t g_timer = 0; // unit: us
static bool g_print_step = false;

static char iringbuf[16][128];

void device_update();

static void trace_and_difftest(Decode *_this, vaddr_t dnpc)
{
#ifdef CONFIG_ITRACE_COND // CONFIG_ITRACE_COND=true
  if (ITRACE_COND)        // true
  {
    // printf("hsay:ITRACE_COND=true\n"); // hamza
    log_write("%s\n", _this->logbuf); //把_this->logbuf写到log_fp里

    strcpy(iringbuf[g_nr_guest_inst % 16], _this->logbuf); //记录ringbuf
  }
#endif
  if (g_print_step) // 运行次数n<10的话为true,
  {
    // printf("hsay:g_print_step=true\n");        // hamza
    IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); // CONFIG_ITRACE=true//puts()把字符串输入到stdout
  }
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc)); // 这个函数是difftest，this->pc是已经执行完的pc，dnpc是DUT（真实）的下条pc
}

static void exec_once(Decode *s, vaddr_t pc)
{
  // printf("hsay:in exec_once()\n"); // hamza
  s->pc = pc;        // s是指针，所以用->。等同于(*s).pc
  s->snpc = pc;      //静态指针赋值，在isa_exec_once()函数中更新为下一指令
  isa_exec_once(s);  //运行一条指令，snpc指向pc+4，dnpc指向下一条指令（可能是任意跳转的位置）
  cpu.pc = s->dnpc;  //把dnpc指令给pc，此时pc为（真实）下条指令位置
#ifdef CONFIG_ITRACE //#define CONFIG_ITRACE 1
  char *p = s->logbuf;
  p += snprintf(p, sizeof(s->logbuf), FMT_WORD ":", s->pc); // snprintf()把s->pc写到p，返回值为欲写入长度
  int ilen = s->snpc - s->pc;
  int i;
  uint8_t *inst = (uint8_t *)&s->isa.inst.val;
  for (i = ilen - 1; i >= 0; i--)
  {
    p += snprintf(p, 4, " %02x", inst[i]);
  }
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0)
    space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;
  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, s->logbuf + sizeof(s->logbuf) - p,
              MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
#endif
}

static void execute(uint64_t n)
{
  // printf("hsay:in execute()\n"); // hamza
  Decode s; // PC相关的结构体
  for (; n > 0; n--)
  {
    // printf("hsay: before exec_once cpu.pc = %lx\n", cpu.pc);
    exec_once(&s, cpu.pc); //执行一次指令,把cpu.pc存入s.pc
    // printf("hsay: after exec_once cpu.pc = %lx\n", cpu.pc);
    g_nr_guest_inst++;              //执行指令数，在statistic()统计函数中需要用到
    trace_and_difftest(&s, cpu.pc); // s.pc存的还是已执行的pc，cpu.pu已经是（真实）的下一条要执行的指令了

    if (nemu_state.state != NEMU_RUNNING) //状态不为running时退出for循环
      break;
    IFDEF(CONFIG_DEVICE, device_update()); //更新时间和状态(quit)
  }
}

static void statistic()
{
  IFNDEF(CONFIG_TARGET_AM, setlocale(LC_NUMERIC, ""));
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%ld", "%'ld")
  Log("host time spent = " NUMBERIC_FMT " us", g_timer);
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
  if (g_timer > 0)
    Log("simulation frequency = " NUMBERIC_FMT " inst/s", g_nr_guest_inst * 1000000 / g_timer);
  else
    Log("Finish running in less than 1 us and can not calculate the simulation frequency");
}

void assert_fail_msg()
{
  isa_reg_display();
  statistic();
}

/* Simulate how the CPU works. */
void cpu_exec(uint64_t n)
{
  // printf("hsay:in cpu_exec()\n");         // hamza
  // printf("hsay:n=%ld\n", n);              // hamza
  g_print_step = (n < MAX_INST_TO_PRINT); // trace_and_difftest()函数内if的判断条件
  switch (nemu_state.state)               //开始没有设定值，所以走的是default
  {
  case NEMU_END:
    // printf("hsay:case NEMU_END\n"); // hamza
    // break;
  case NEMU_ABORT:
    printf("Program execution has ended. To restart the program, exit NEMU and run again.\n");
    return;
  default:
  {
    // printf("hsay:neme_state.state default=NEMU_RUNNING\n"); // hamza
    nemu_state.state = NEMU_RUNNING;
  }
  }

  uint64_t timer_start = get_time(); //记录开始时间
  // printf("hsay:timer_start=%lu\n", timer_start); // hamza

  execute(n); //传入-1时do nothing

  uint64_t timer_end = get_time(); //记录结束时间
  // printf("hsay:timer_end=%lu\n", timer_end); // hamza

  g_timer += timer_end - timer_start; //运行execute的时间
  // printf("hsay:g_timer=%lu\n", g_timer); // hamza

  switch (nemu_state.state)
  {
  case NEMU_RUNNING:
    nemu_state.state = NEMU_STOP;
    break;

  case NEMU_END:
  case NEMU_ABORT:
    Log("nemu: %s at pc = " FMT_WORD, //#define FMT_WORD MUXDEF(CONFIG_ISA64, "0x%016lx", "0x%08x")
        (nemu_state.state == NEMU_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) : (nemu_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) : ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
        nemu_state.halt_pc); //打印运行结果（状态和PC）
    // fall through
    for (int i = 0; i < 16; i++) //输出ringbuf
    {
      if (i == g_nr_guest_inst % 16)
        printf("--> %s\n", iringbuf[i]);
      else
        printf("    %s\n", iringbuf[i]);
    }
  case NEMU_QUIT:
    statistic(); //统计运行速度
  }
}
