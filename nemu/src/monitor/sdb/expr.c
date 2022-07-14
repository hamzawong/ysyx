#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
// uint32_t isa_reg_str2val(const char *s, bool *success);
uint8_t *guest_to_host(paddr_t paddr);

enum //用于将token编码
{

  TK_NOTYPE = 256,
  TK_DEC,
  TK_HEX,
  TK_REG,

  /* TODO: Add more token types */

  TK_EQ,
  TK_NEQ,
  TK_DEREF,
  TK_NEG,
};

static struct rule
{
  const char *regex;
  int token_type;
} rules[] = {

    /* TODO: Add more rules.
     * Pay attention to the precedence level of different rules.
     */

    {" +", TK_NOTYPE},          // spaces//一个或者多个空格
    {"0x[0-9a-fA-F]+", TK_HEX}, //需要在TK_NUM之前
    {"\\$\\w{1,3}", TK_REG},
    {"[0-9]+", TK_DEC},

    {"\\+", '+'}, // plus//一个+号
    {"\\-", '-'},
    {"\\*", '*'},
    {"\\/", '/'},
    {"\\(", '('},
    {"\\)", ')'},
    {"==", TK_EQ}, // equal//=号
    {"!=", TK_NEQ},
    {"&&", '&'},
    {"||", '|'},
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex()
{
  int i;
  char error_msg[128];
  int ret; // preg, a pointer to a pattern buffer storage area

  for (i = 0; i < NR_REGEX; i++)
  {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED); // regcomp:regex compile//compile successful return 0//将正则表达式逐个编译存储到re
    if (ret != 0)
    {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token
{
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used)) = 0;

void clear_tokens(void *p_struct, unsigned int size) //清零
{
  unsigned char *p = p_struct;

  while (size > 0)
  {
    *p = 0;
    p++;
    size--;
  }
}

static bool make_token(char *e)
{
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  clear_tokens(&tokens, sizeof(tokens)); //清零

  while (e[position] != '\0') //表达式未结束
  {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i++)
    {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) // re[]存了已编译的rules
      {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        // Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
        //     i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        // nr_token++;

        // switch (rules[i].token_type)
        // {

        // case '-':
        //   if (nr_token == 0 || tokens[nr_token - 1].type == '(' ||
        //       tokens[nr_token - 1].type == '|' || tokens[nr_token - 1].type == '&' ||
        //       tokens[nr_token - 1].type == TK_EQ || tokens[nr_token - 1].type == TK_NEQ ||
        //       tokens[nr_token - 1].type == '+' || tokens[nr_token - 1].type == '-' ||
        //       tokens[nr_token - 1].type == '*' || tokens[nr_token - 1].type == '/')
        //   {
        //     printf("is TK_NEG\n");
        //     tokens[nr_token].type = TK_NEG;
        //   }
        //   else
        //   {
        //     printf("is TK_MINUS\n");
        //     tokens[nr_token].type = TK_MINUS;
        //   }

        // case '*':
        //   if (nr_token == 0 || tokens[nr_token - 1].type == '(' ||
        //       tokens[nr_token - 1].type == '|' || tokens[nr_token - 1].type == '&' ||
        //       tokens[nr_token - 1].type == TK_EQ || tokens[nr_token - 1].type == TK_NEQ ||
        //       tokens[nr_token - 1].type == '+' || tokens[nr_token - 1].type == '-' ||
        //       tokens[nr_token - 1].type == '*' || tokens[nr_token - 1].type == '/')
        //     tokens[nr_token].type = TK_MULTI;

        //   else
        //     tokens[nr_token].type = TK_DEREF;
        switch (rules[i].token_type)
        {
        case '*':
        case '-':
        case '+':
        case '/':
        case '(':
        case ')':
        case '&':
        case TK_EQ:
        case TK_NEQ:
        case TK_DEC:
        case TK_HEX:
        case TK_REG:
          if (substr_len > 31)
          {
            puts("Too long numbers not supported");
            return false;
          }
          tokens[nr_token].type = rules[i].token_type;
          strncpy(tokens[nr_token].str, substr_start, substr_len);
          tokens[nr_token].str[substr_len] = '\0';
          // nr_token++;
          break;
        case TK_NOTYPE:
          break;
        default:
          // TODO();
          tokens[nr_token].type = rules[i].token_type;
          break;
        }
        nr_token++;
        break;
      }
    }

    if (i == NR_REGEX) //未匹配到token
    {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(Token *tokens, int p, int q)
{
  printf("hsay: in check_parentheses()\n");
  int cnt_ckbk = 0;
  if (tokens[p].type == '(' && tokens[q].type == ')')
  {
    for (int i = 1; i < q - p; i++)
    {
      if (tokens[p + i].type == '(')
        cnt_ckbk++;
      if (tokens[p + i].type == ')')
        cnt_ckbk--;
      if (cnt_ckbk < 0)
        return false;
      if (cnt_ckbk >= 0)
        continue;
    }
  }
  else
    return false;
  return true;
}

int break_exp(Token *tokens, int p, int q, bool *success)
{
  printf("hsay: in break expression\n");
  int op = 0; // 主运算符位置
  for (int i = 0, cnt_bk = 0; i <= q - p; i++)
  {

    if (tokens[p + i].type == '(')
      cnt_bk++;
    if (tokens[p + i].type == ')')
      cnt_bk--;
    if (cnt_bk == 0)
    {
      if (tokens[p + i].type == '*' || tokens[p + i].type == '/')
        op = p + i;
      printf("hsay:detec */:op=%d\n", op);
    }
  }
  for (int i = 0, cnt_bk = 0; i <= q - p; i++)
  {

    if (tokens[p + i].type == '(')
      cnt_bk++;
    if (tokens[p + i].type == ')')
      cnt_bk--;
    if (cnt_bk == 0)
    {
      if (tokens[p + i].type == '+' || tokens[p + i].type == '-')
        op = p + i;
      printf("hsay:detec +-:op=%d\n", op);
    }
  }
  for (int i = 0, cnt_bk = 0; i <= q - p; i++)
  {

    if (tokens[p + i].type == '(')
      cnt_bk++;
    if (tokens[p + i].type == ')')
      cnt_bk--;
    if (cnt_bk == 0)
    {
      if (tokens[p + i].type == TK_EQ || tokens[p + i].type == TK_NEQ)
        op = p + i;
      printf("hsay:detec ==!=:op=%d\n", op);
    }
  }
  for (int i = 0, cnt_bk = 0; i <= q - p; i++)
  {

    if (tokens[p + i].type == '(')
      cnt_bk++;
    if (tokens[p + i].type == ')')
      cnt_bk--;
    if (cnt_bk == 0)
    {
      if (tokens[p + i].type == '&')
        op = p + i;
      printf("hsay:detec &:op=%d\n", op);
    }
  }
  for (int i = 0, cnt_bk = 0; i <= q - p; i++)
  {

    if (tokens[p + i].type == '(')
      cnt_bk++;
    if (tokens[p + i].type == ')')
      cnt_bk--;
    if (cnt_bk == 0)
    {
      if (tokens[p + i].type == '|')
        op = p + i;
      printf("hsay:detec |:op=%d\n", op);
    }
  }
  // *success = true;
  return op;
}

int eval(Token *tokens, int p, int q, bool *success)
{
  // printf("hsay: in eval()\n");
  if (p > q)
  {
    /* Bad expression */
    printf("Bad expression!\n");
    *success = false;
    assert(0);
  }
  else if (p == q)
  {
    /* Single token.
     * For now this token should be a number.
     * Return the value of the number.
     */
    int num = 0;
    if (tokens[p].type == TK_DEC)
    {
      printf("hsay: is TK_DEC\n");
      sscanf(tokens[p].str, "%d", &num);
      *success = true;
    }
    else if (tokens[p].type == TK_HEX)
    {
      printf("hsay: is TK_HEX\n");
      sscanf(tokens[p].str, "0x%x", &num);
      *success = true;
    }
    else if (strcmp(tokens[p].str, "$pc") == 0) // register
    {
      printf("hsay: is TK_REG;is $pc\n");
      num = cpu.pc;
      *success = true;
    }
    else
    {
      printf("hsay: is TK_REG\n");
      num = isa_reg_str2val(tokens[p].str + 1, success);
    }

    return num;
  }
  else if (q - p == 1 || check_parentheses(tokens, p + 1, q) == true) //长度为2或者是 -()型 *()型
  {
    switch (tokens[p].type)
    {
    case TK_DEREF:
      return *((uint32_t *)guest_to_host(eval(tokens, p + 1, q, success)));

    case TK_NEG: //取负
      printf("negetive\n");
      return -eval(tokens, p + 1, q, success);
    default:
      break;
    }
  }
  else if (check_parentheses(tokens, p, q) == true)
  {
    printf("hsay: have parentheses\n");
    /* The expression is surrounded by a matched pair of parentheses.
     * If that is the case, just throw away the parentheses.
     */
    return eval(tokens, p + 1, q - 1, success);
  }
  else
  {
    int op = 0;
    int val1, val2;
    if (tokens[p].type == TK_NEG && q - p == 1) //先判断是否是负表达式
    {
      return (0 - eval(tokens, p, q, success));
    }
    // else if (tokens[p].type == TK_REG) // TODO: 是否为寄存器表达式
    // {

    //   return *((uint32_t *)guest_to_host(eval(tokens, p + 1, q)));
    // }

    // else if (tokens[p].type == TK_DEREF) //TODO:是否为*指针解引用
    //   ;
    else //正表达式
    {
      op = break_exp(tokens, p, q, success);
      printf("op=%d\n", op);
      printf("p=%d\n", p);
      val1 = eval(tokens, p, op - 1, success);
      printf("val1=%d\n", val1);
      printf("op=%d\n", op);
      val2 = eval(tokens, op + 1, q, success);
      printf("val2=%d\n", val2);

      // int op_type = tokens[op].type;
      // printf("op_type=%d\n", op_type);
    }
    int op_type = tokens[op].type;
    printf("op_type=%d\n", op_type);

    switch (op_type)
    {
    case '+':
      return val1 + val2;
    case '-':
      return val1 - val2; /* ... */
    case '*':
      return val1 * val2; /* ... */
    case '/':
      if (val2 != 0)
        return val1 / val2; /* ... */
      else
      {
        printf("div by zero!");
        return 0;
      }

    case TK_EQ:
      return val1 == val2;
    case TK_NEQ:
      return val1 != val2;
    case '&':
      return val1 && val2;
    case '|':
      return val1 || val2;
      // case TK_DEREF:
      // return 0;
      // case TK_NEG:
      // return
    default:
      assert(0);
    }
  }
  return 0;
}

word_t expr(char *e, bool *success)
{
  if (!make_token(e)) //此处提出了所有tokens
  {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  // TODO();

  printf("nr_token=%d\n", nr_token);

  word_t result_e = eval(tokens, 0, nr_token - 1, success);
  printf("result_e = %lu\n", result_e);

  return result_e;
}
