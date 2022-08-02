/*
 * @Author: 2022041439-Huang Wenhan huangwenhan@126.com
 * @Date: 2022-07-14 16:51:30
 * @LastEditors: 2022041439-Huang Wenhan huangwenhan@126.com
 * @LastEditTime: 2022-07-14 16:55:09
 * @FilePath: /ysyx-workbench/nemu/tools/gen-expr/test_expression.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <common.h>

void test_expression()
{
    FILE *fp = fopen("./tools/gen-expr/input", "r");
    char eg_test[64];
    int ok_num = 0;
    int bad_num = 0;
    int zero_num = 0;
    while (fgets(eg_test, sizeof(eg_test), fp) != NULL)
    {
        printf("eg_test is %s\n", eg_test);
        char *result_test = strtok(eg_test, " ");
        printf("result_test is %s\n", result_test);
        char *expr_test = strtok(NULL, " ");
        printf("expr_test is %s\n", expr_test);
        printf("strlen(expr_test)=%ld\n", strlen(expr_test));
        char *tmp;
        tmp = expr_test;
        if ((tmp = strstr(expr_test, "\n")))
            *tmp = '\0';
        // strncpy(cp_expr, expr_test, sizeof(expr_test) - 1);
        // printf("cp_expr is %s\n", cp_expr);
        bool success = false;
        u_int32_t result = expr(expr_test, &success);
        if (result == (u_int32_t)atoi(result_test) || result == 0)
        {
            printf("hsay: ok_num=%d\n", ok_num++);
            printf("hsay: bad_num=%d\n", bad_num);
            printf("hsay: divi by zero_num=%d\n", zero_num);
            if (result == 0)
            {
                printf("hsay: divi by zero_num=%d\n", zero_num++);
            }
        }

        else
        {
            printf("expr_test is %s\n", expr_test);
            printf("result_test=%s != result=%u\n", result_test, result);
            printf("hsay: NOT EQUAL,ok_num=%d\n", ok_num);
            printf("hsay: NOT EQUAL,bad_num=%d\n", bad_num++);
            printf("hsay: divi by zero_num=%d\n", zero_num);
            assert(0);
        }

        eg_test[0] = '\0';
        // cp_expr[0] = '\0';
    }
    fclose(fp);
}
