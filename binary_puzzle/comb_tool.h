/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/26                                                     **/
/** Description: 辅助函数头文件			                          **/
/**********************************************************/

#pragma once
//#include"generate.h"
#include"../naive_implementation/data_structure/tool_functions.h"

constexpr int N = puzzle_size;
constexpr int M = (puzzle_size / 2) + 1;

/**
@brief: 求组合数C(n, m)
@param n: 待选总体数n
@param m: 需要选取个体数m
@return: 组合数C(n, m)
*/
constexpr int C(int n, int m)
{
    int a = 1, b = 1;
    for (int i = n; i > n - m; i--)
        a *= i;
    for (int j = 2; j <= m; j++)
        b *= j;
    return a / b;
}

//规则2转换时所用组合数
constexpr int CNUM = C(N, M);

//求组合时所用参数
int cnt = 0, data_cnt = 0;
int results[puzzle_size] = { 0 };


/**
@brief: 初始化二维数组datas用于存放组合结果
@param n: datas第一维大小
@param m: datas第二维大小
@return: 初始化后的datas数组
*/
int** setDataArr(size_t n, size_t m)
{
    data_cnt = 0;
    int** datas = (int**)malloc(sizeof(int*) * n);
    for (int i = 0; i < n; i++) {
       datas[i] = (int*)malloc(sizeof(int) * m);
       memset(datas[i], 0, sizeof(int) * m);
    }
    return datas;
}


/**
@brief: 求组合
@param beg: 开始位标志，一般为0
@param div: 起始深度设置，一般为0
@param comb_arr: 需要组合的样本总体数组
@param datas: 用于存放组合结果的二维数组
@param m: datas第二维大小
*/
void comb(int beg, int div, int* comb_arr, int** datas, int m)
{
    if (div == m)
    {
        if (cnt != m)
            return;
        for (int i = 0; i < m; ++i)
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
        comb(i + 1, div + 1, comb_arr, datas, m);
        --cnt;
    }
    return;
}


/**
@brief: 求所有的排列
@param comb_arr: 需要排列的总体
@param datas: 存放结果的数组
@param n: 总体的元素个数
@param m: 需要选取的元素个数
@param beg: 开始位标志，默认为0
@param div: 起始深度设置，默认为0
*/
void combine(int* comb_arr, int** datas, int n, int m, int beg = 0, int div = 0)
{
    if (div == m)
    {
        if (cnt != m)
            return;
        for (int i = 0; i < m; ++i)
        {
            // printf("%d", results[i]);
            datas[data_cnt][i] = results[i];
        }
        data_cnt++;
        return;
    }
    for (int i = beg; i < n; ++i)
    {
        results[cnt++] = comb_arr[i];
        combine(comb_arr, datas, n, m, i + 1, div + 1);
        --cnt;
    }
    return;
}
