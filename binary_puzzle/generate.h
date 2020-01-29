/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: �����������ͷ�ļ�		                          **/
/**********************************************************/
#pragma once
#include"../config/config.h"

//��ʾ���̵Ľṹ��
struct Puzzle
{
	int puzzle[puzzle_size][puzzle_size];
};


/**
@brief: ��ʼ��һ������
@return: ��ʼ�����һ������
*/
struct Puzzle initPuzzle()
{
	struct Puzzle p;
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			p.puzzle[i][j] = -1;
	return p;
}


/**
@brief: ����һ������
@return: ���ɺ������
*/
struct Puzzle generatePuzzle()
{
	struct Puzzle p;
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			p.puzzle[i][j] = rand() % 2;
	return p;
}