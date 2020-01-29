/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 主控模块相关头文件		                          **/
/**********************************************************/

#pragma once
#include"../binary_puzzle/solve_puzzle.h"


enum
{
	MAIN, CNF, PUZZLE, ESC
} mode;


void printGuide()
{
	printf("/**************************************************************/\n");
	printf("/*                          欢迎                              */\n");
	printf("/*           此程序可用于求解CNF范式和求解数独                */\n");
	printf("/*     按下c键进入求解CNF范式模式, 按下p键进入求解数独模式    */\n");
	printf("/*   可通过按下ESC键返回模式选择界面, 再次按下ESC键退出程序   */\n");
	printf("/**************************************************************/\n");
}


void cnfResultPrint(char* filename, struct Result result)
{
	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s %d\n", "s", result.isSatisfied);
	fprintf(fp, "%s ", "v");
	for (int i = 1; i <= info.num_literal; i++) {
		fprintf(fp, "%d\t", result.res[i]);
		if (i % 15 == 0) fprintf(fp, "%c", '\n');
	}
	fprintf(fp, "%c", '\n');
	fprintf(fp, "%s %f", "t", result.time);
	printf("/*解答已写入文件:%s\n", filename);
	fclose(fp);
}


/**
@brief: 显示交互环境，获取用户输入
@return: 输入的文件路径名
*/
char* getCnfFileName()
{
	printf("\n/*请输入cnf文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


char* getPuzzleName()
{
	printf("\n/*请输入Puzzle棋盘文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


void modeChange()
{
	mode = MAIN;
	printf("/*请进行模式选择(按下c键进入求解CNF范式模式, 按下p键进入求解数独模式, esc退出程序): \n");
	char c = getch();
	if (c == 'c' or c == 'C') {
		mode = CNF;
		printf("/*进入求解CNF范式模式\n");
	}
	else if (c == 'p' or c == 'P') {
		mode = PUZZLE;
		printf("/*进入求解数独模式(当前数独阶数为%d)\n", puzzle_size);
	}
	else if (c == 27) {
			mode = ESC;
			printf("已退出程序\n");
	}
	else {
		printf("/*请检查按键是否错误!\n");
		modeChange();
	}
}


void callCnfSolver()
{
	char* cnf_filename = getCnfFileName();
	struct Formula* formula = loadFile(cnf_filename);

	float start = clock();
	struct Result result = DPLL(formula);
	float finish = clock();
	result.time = finish - start;

	char* path = strtok(cnf_filename, ".cnf");
	char* w_filename = strcat(path, ".res");
	cnfResultPrint(w_filename, result);
	free(result.res);
	free(cnf_filename);
	destoryFormula(formula);
}


void callPuzzleSolver()
{
	char* puzzle_filename = getPuzzleName();
	struct Puzzle p = loadPuzzleFromFile(puzzle_filename);
	solvePuzzle(p);
}


void display()
{
	printGuide();
	modeChange();
	while (mode != ESC){
		if (mode == CNF) callCnfSolver();
		if (mode == PUZZLE) callPuzzleSolver();
		printf("/*按下ESC键退回到模式选择, 其他按键则继续当前模式.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}