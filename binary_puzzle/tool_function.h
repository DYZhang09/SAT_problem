

#pragma once
#include"puzzle.h"

/**
@brief: 打印输入指南
*/
void printPlayGuide()
{
	printf("/*请按以下格式输入解答: 行(空格) 列(空格) 0/1/-1(放弃解答, 直接放出结果)\n");
}


/**
@brief: 检查输入合法性
@param input: 保存了输入的数组
@return: 合法则返回true
*/
bool checkInput(int* input)
{
	if (input[0] > 0 and input[0] <= puzzle_size
		and input[1] > 0 and input[1] <= puzzle_size
		and (input[2] == 0 or input[2] == 1 or input[2] == -1)) {
		return true;
	}
	return false;
}


/**
@brief: 检查是否棋盘结果是否正确
@param p: 棋盘
@return: 正确则返回true
*/
bool checkValid(struct Puzzle p, struct Formula* formula)
{
	int result[puzzle_size * puzzle_size + 1];
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + j + 1;
			if (p.puzzle[i][j] == -1) 
				result[data] = 0;
			else 
				result[data] = (p.puzzle[i][j] > 0) ? 1 : -1;
		}
	}
	bool res = evaluateFormula(formula, result);
	return res;
}


/**
@brief: 检查棋盘是否还有空位
@param p: 棋盘
@return: 仍有空位则返回true
*/
bool hasVacant(struct Puzzle p)
{
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			if (1 - p.puzzle[i][j] > 1) return true;
	return false;
}


/**
@brief: 获取用户输入并判断合法性
@return: 合法的用户输入
*/
int* getPlayInput()
{
	int input[3] = { -1 };
	printPlayGuide();
	scanf("%d %d %d", input, input + 1, input + 2);
	if (!checkInput(input)) {
		printf("请检查输入是否符合\n");
		return getPlayInput();
	}
	return input;
}


/**
@brief: 将用户输入应用到棋盘
@param p: 指向棋盘的指针
@param input: 用户的输入
*/
void applyInput(struct Puzzle* p, int* input)
{
	int row = input[0], col = input[1];
	p->puzzle[row - 1][col - 1] = input[2];
}

