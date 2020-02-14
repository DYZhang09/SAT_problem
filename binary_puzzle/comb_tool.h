/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/26                                                     **/
/** Description: ��������ͷ�ļ�			                          **/
/**********************************************************/

#pragma once
//#include"generate.h"
#include"../naive_implementation/data_structure/tool_functions.h"

constexpr int N = puzzle_size;
constexpr int M = (puzzle_size / 2) + 1;

/**
@brief: �������C(n, m)
@param n: ��ѡ������n
@param m: ��Ҫѡȡ������m
@return: �����C(n, m)
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

//����2ת��ʱ���������
constexpr int CNUM = C(N, M);

//�����ʱ���ò���
int cnt = 0, data_cnt = 0;
int results[puzzle_size] = { 0 };


/**
@brief: ��ʼ����ά����datas���ڴ����Ͻ��
@param n: datas��һά��С
@param m: datas�ڶ�ά��С
@return: ��ʼ�����datas����
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
@brief: �����
@param beg: ��ʼλ��־��һ��Ϊ0
@param div: ��ʼ������ã�һ��Ϊ0
@param comb_arr: ��Ҫ��ϵ�������������
@param datas: ���ڴ����Ͻ���Ķ�ά����
@param m: datas�ڶ�ά��С
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
@brief: �����е�����
@param comb_arr: ��Ҫ���е�����
@param datas: ��Ž��������
@param n: �����Ԫ�ظ���
@param m: ��Ҫѡȡ��Ԫ�ظ���
@param beg: ��ʼλ��־��Ĭ��Ϊ0
@param div: ��ʼ������ã�Ĭ��Ϊ0
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
