/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/26                                                     **/
/** Description: 辅助函数头文件			                          **/
/**********************************************************/

#pragma once
#include"generate.h"
#include"../naive_implementation/data_structure/tool_functions.h"

constexpr int N = puzzle_size;
constexpr int M = (puzzle_size / 2) + 1;

constexpr int C(int n, int m)
{
    int a = 1, b = 1;
    for (int i = n; i > n - m; i--)
        a *= i;
    for (int j = 2; j <= m; j++)
        b *= j;
    return a / b;
}

constexpr int CNUM = C(N, M);

int comb_arr[puzzle_size];
int cnt = 0, data_cnt = 0;
int results[puzzle_size] = { 0 };
int datas[CNUM][M] = { 0 };

void comb(int beg, int div)
{
    if (div == M)
    {
        if (cnt != M)
            return;
        for (int i = 0; i < M; ++i)
        {
           // printf("%d", results[i]);
            datas[data_cnt][i] = results[i];
        }
        data_cnt++;
        return;
    }
    for (int i = beg; i < N; ++i)
    {
        results[cnt++] = comb_arr[i];
        comb(i + 1, div + 1);
        --cnt;
    }
    return;
}
