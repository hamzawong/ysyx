#include <dlfcn.h>

#include <isa.h>
#include <cpu/cpu.h>
#include <memory/paddr.h>
#include <utils.h>
#include <difftest-def.h>

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL; //函数指针
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;

#ifdef CONFIG_DIFFTEST

static bool is_skip_ref = false;
static int skip_dut_nr_inst = 0;

// this is used to let ref skip instructions which
// can not produce consistent behavior with NEMU
void difftest_skip_ref() //让ref跳过不能匹配NEMU行为的指令
{
  is_skip_ref = true;
  // If such an instruction is one of the instruction packing in QEMU
  // (see below), we end the process of catching up with QEMU's pc to
  // keep the consistent behavior in our best.
  // Note that this is still not perfect: if the packed instructions
  // already write some memory, and the incoming instruction in NEMU
  // will load that memory, we will encounter false negative. But such
  // situation is infrequent.
  skip_dut_nr_inst = 0;
}

// this is used to deal with instruction packing in QEMU.
// Sometimes letting QEMU step once will execute multiple instructions.
// We should skip checking until NEMU's pc catches up with QEMU's pc.
// The semantic is
//   Let REF run `nr_ref` instructions first.
//   We expect that DUT will catch up with REF within `nr_dut` instructions.
void difftest_skip_dut(int nr_ref, int nr_dut) //
{
  skip_dut_nr_inst += nr_dut;

  while (nr_ref-- > 0)
  {
    ref_difftest_exec(1); //
  }
}

void init_difftest(char *ref_so_file, long img_size, int port)
{
  assert(ref_so_file != NULL); //不能为空指针

  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);
  assert(handle); // handle不能为0

  ref_difftest_memcpy = dlsym(handle, "difftest_memcpy");
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");
  assert(ref_difftest_regcpy);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in menuconfig.",
      ref_so_file);

  ref_difftest_init(port);                                                                   //对REF的DIffTest功能进行初始化, 具体行为因REF而异.
  ref_difftest_memcpy(RESET_VECTOR, guest_to_host(RESET_VECTOR), img_size, DIFFTEST_TO_REF); //将DUT的guest memory拷贝到REF中.
  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);                                                //将DUT的寄存器状态拷贝到REF中.
}

static void checkregs(CPU_state *ref, vaddr_t pc)
{
  if (!isa_difftest_checkregs(ref, pc)) // TODO: 比较ref和DUT的gpr//目前函数固定返回0，所以直接中断
  {
    nemu_state.state = NEMU_ABORT;
    nemu_state.halt_pc = pc;
    isa_reg_display();
  }
}

void difftest_step(vaddr_t pc, vaddr_t npc) // pc为dut已执行的指令地址，npc为dut将要跳转的pc地址
{
  CPU_state ref_r; //内有gpr和pc

  if (skip_dut_nr_inst > 0) //跳过dut，去追ref
  {
    ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT); //直接把ref的寄存器复制给dut
    if (ref_r.pc == npc)                          // dut追上ref
    {
      skip_dut_nr_inst = 0;
      checkregs(&ref_r, npc);
      return;
    }
    skip_dut_nr_inst--;
    if (skip_dut_nr_inst == 0) // dut追不上ref
      panic("can not catch up with ref.pc = " FMT_WORD " at pc = " FMT_WORD, ref_r.pc, pc);
    return;
  }

  if (is_skip_ref) //跳过ref，直接复制reg给ref
  {
    // to skip the checking of an instruction, just copy the reg state to reference design
    ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF); //直接把dut的寄存器复制给ref
    is_skip_ref = false;
    return;
  }

  ref_difftest_exec(1);                         //执行ref
  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT); // TODO: 不懂

  checkregs(&ref_r, npc); //比较ref刚执行的gpr和dut已执行pc的gpr
}
#else
void init_difftest(char *ref_so_file, long img_size, int port)
{
}
#endif
