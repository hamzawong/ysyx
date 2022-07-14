/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-06-22 10:48:43
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-13 15:09:37
 * @FilePath: /ysyx-workbench/nemu/src/monitor/sdb/watchpoint.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint
{
  int NO;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */
  char expr_wp[64];
  word_t val_expr;

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool()
{
  int i;
  for (i = 0; i < NR_WP; i++)
  {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool; // pool的首地址给free_
}

/* TODO: Implement the functionality of watchpoint */

WP *new_wp(char *expression)
{
  assert(free_);                    //没有空闲wp，直接停止
  WP *new = free_;                  //空闲wp地址给new
  free_ = free_->next;              //用next更新free_
  strcpy(new->expr_wp, expression); //记录新wp的表达式
  return new;
}

void add_wp(char *expression, bool *success)
{
  WP *new = new_wp(expression);
  new->next = head; //前一个wp地址给新的wp的next
  new->val_expr = expr(expression, success);
  head = new; //新建的wp地址存到head
}

void free_wp(WP *wp)
{
  wp->next = free_; // wp->next指向闲
  free_ = wp;       //把wp置闲
}

bool delete_wp(int NO)
{
  WP *wp_NO = head;    // head是刚新建的wp地址
  if (wp_NO->NO == NO) //如果要删除的是刚建立的监视点
  {
    head = wp_NO->next; // head回退一个
    free_wp(wp_NO);     //把wp_NO置闲
    return true;
  }
  else
  {
    while (wp_NO->next && wp_NO->next->NO != NO) //只要没有找到对应的监视点，或者已经遍历所有监视点
    {
      wp_NO = wp_NO->next; //往前找到需要删除的wp
    }
    if (!wp_NO->next) // wp_NO为第一个监视点
      return false;   //说明没有寻找到对应的监视点序号
    else              //找到对应监视点的后一个监视点
    {
      WP *temp = wp_NO->next;   //把要删除的监视点地址暂存起来
      wp_NO->next = temp->next; //让要删除的监视点的后一个监视点的next指向前一个监视点的地址//即跳过要删除的监视点
      free_wp(temp);            //清空temp//删除的监视点变成闲置
      return true;
    }
  }
}

void watchpoint_display()
{
  printf("hsay:in watchpoint_display()\n");
  if (!head)
  {
    puts("NO watchpoint yet!");
    return;
  }
  printf("%-20s%-20s\n", "NUM", "What");
  WP *temp = head;
  while (temp) //从后往前打印wp信息
  {
    printf("%-20d%-20s\n", temp->NO, temp->expr_wp);
    temp = temp->next;
  }
}

// bool wp_update_display_changed()
// {
//   WP *temp = head;
//   bool flag = false;
//   bool success; // not used
//   while (temp != NULL)
//   {
//     word_t new_val = expr(temp->expr_wp, &success);
//     if (temp->val_expr != new_val)
//     {
//       if (!flag)
//       {
//         flag = true;
//         printf("Watchpoint value changed:\n");
//       }
//       printf("Watchpoint %2d: %-16s new:%-10lu      old:%-10lu\n", temp->NO, temp->expr_wp, new_val, temp->val_expr);
//       temp->val_expr = new_val;
//     }
//     temp = temp->next;
//   }
//   if (flag)
//     return true;
//   else
//     return false;
// }