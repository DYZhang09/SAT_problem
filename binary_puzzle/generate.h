/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独生成相关头文件		                          **/
/**********************************************************/
#pragma once
#include"../config/config.h"

//表示棋盘的结构体
struct Puzzle
{
	int puzzle[puzzle_size][puzzle_size];
};


/**
@brief: 初始化一个棋盘
@return: 初始化后的一个棋盘
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
@brief: 生成一个棋盘
@return: 生成后的棋盘
*/
struct Puzzle generatePuzzle()
{
	struct Puzzle p;
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			p.puzzle[i][j] = rand() % 2;
	return p;
}