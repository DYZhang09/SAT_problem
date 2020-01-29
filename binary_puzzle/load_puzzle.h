/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/29                                                     **/
/** Description: 数独读取相关函数头文件                        **/
/**********************************************************/

#pragma once
#include"generate.h"

/**
@brief: 从命令行中读取棋盘
@return: 读取数据后的棋盘
*/
struct Puzzle loadPuzzleFromTerm()
{
	struct Puzzle p = initPuzzle();
	int c = -1;
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			scanf("%d", &c);
			if (c != -1) p.puzzle[i][j] = c;
		}
	}
	return p;
}


/**
@brief: 从文件中读取棋盘
@param filename: 目标文件路径名
@return: 读取数据后的棋盘
*/
struct Puzzle loadPuzzleFromFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("无法打开文件: %s\n", filename);
		exit(-1);
	}
	else {
		struct Puzzle p = initPuzzle();
		int c = -1;
		for (int i = 0; i < puzzle_size; i++) {
			for (int j = 0; j < puzzle_size; j++) {
				fscanf(fp, "%d", &c);
				if (c != -1) p.puzzle[i][j] = c;
			}
		}
		return p;
	}
}