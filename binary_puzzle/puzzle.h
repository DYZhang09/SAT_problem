/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: �����ṹ���ͷ�ļ�		                          **/
/**********************************************************/

#pragma once

#include"../config/config.h"

//��ʾ���̵Ľṹ��
struct Puzzle
{
	int puzzle[puzzle_size][puzzle_size];		//��������
	int mask[puzzle_size][puzzle_size];		//��Ĥ����, ������ʾĳ��λ���Ƿ�ֻ��
	int level;		//�Ѷ�ϵ��, ��ʾ���ɵ�������ʼ���ֵĸ���
};