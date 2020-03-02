/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: 主控模块相关头文件		                          
/**********************************************************/

#pragma once
#include"./debug.h"

#include"../binary_puzzle/play.h"
#include"../naive_dpll/solver/solver.h"
#include"../Optimized_implementation/solver/solver.h"
#include"../walksat-dpll/dpll.h"

//模式状态
//MAIN: 主界面(选择模式界面), 
//CNF:CNF求解模式, 
//PUZZLE: 求解数独模式
//PUZZLE_PLAY: 游玩数独模式
//DEBUG: DEBUG模式(输出验证信息)
//ESC: 退出程序
enum
{
	MAIN, CNF, PUZZLE, PUZZLE_PLAY, DEBUG, ESC
} mode;


/**
@brief: 打印说明界面
*/
void printGuide()
{
	printf("/**********************************************************************************/\n");
	printf("/*                                    欢迎                                        */\n");
	printf("/*                     此程序可用于求解CNF范式和求解数独                          */\n");
	printf("/*               按下c键进入求解CNF范式模式, 按下p键进入求解数独模式              */\n");
	printf("/*             可通过按下ESC键返回模式选择界面, 再次按下ESC键退出程序             */\n");
	printf("/**********************************************************************************/\n");
}


/**
@brief: 将求解cnf的解答写入文件
@param filename: 写入文件的路径
@param result: 解答
*/
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
@brief: 显示CNF交互环境，获取用户输入
@return: 输入的文件路径名
*/
char* getCnfFileName()
{
	printf("\n/*请输入cnf文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: 显示PUZZLE交互环境，获取用户输入
@return: 输入的文件路径名
*/
char* getPuzzleName()
{
	printf("\n/*请输入Puzzle棋盘文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: 获得cnf求解后文件的输出路径
@param cnf_filename: cnf输入文件路径
@return: 输出路径
*/
char* cnfOutputName(char* cnf_filename)
{
	char* suffix = strstr(cnf_filename, ".cnf");
	char c[5] = { '.', 'r', 'e', 's', '\0' };
	int suffix_len = strlen(suffix);
	for (int i = 0; i < suffix_len; i++)
		suffix[i] = c[i];
	return cnf_filename;
}


/**
@brief: 模式选择函数
*/
void modeChange()
{
	mode = MAIN;
	printf("/*请进行模式选择(按键选择: c - CNF求解模式, p - 数独求解模式, g - 数独游玩模式, d - debug模式, ESC退出): \n");
	char c = getch();
	if (c == 'c' or c == 'C') {
		mode = CNF;
		printf("/*进入求解CNF范式模式\n");
	}
	else if (c == 'p' or c == 'P') {
		mode = PUZZLE;
		printf("/*进入求解数独模式(当前数独阶数为%d)\n", puzzle_size);
	}
	else if (c == 'g' or c == 'G') {
		mode = PUZZLE_PLAY;
		printf("/*进入游玩数独模式(当前数独阶数为%d)\n", puzzle_size);
	}
	else if (c == 'd' or c == 'D') {
		mode = DEBUG;
		printf("/*进入debug模式\n");
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


/**
@brief: 调用cnf求解程序接口
*/
void callCnfSolver()
{
	char* cnf_filename = getCnfFileName();		//获取cnf输入文件路径
	struct Formula* formula = loadFile(cnf_filename);		//读取文件得到CNF公式

	float start = clock();		//时间起点
	struct Result result = DPLL(formula);		//调用DPLL函数求解CNF公式
	float finish = clock();	//时间终点
	result.time = finish - start;		//求解时间

	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	free(result.res);			//释放空间
	free(cnf_filename);
	destoryFormula(formula);
}


/**
@brief: 调用cnf求解模块(决策优化版本)
*/
void callCnfSolverOpti()
{
	char* cnf_filename = getCnfFileName();
	int* counter = initCounter();
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);

	float start = clock();		//时间起点
	struct Result result = DPLLOpti(formula, counter);		//调用DPLL函数求解CNF公式
	float finish = clock();	//时间终点
	result.time = finish - start;		//求解时间

	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	free(result.res);			//释放空间
	free(cnf_filename);
	free(counter);
	destoryFormula(formula);
}


/**
@brief: 调用数独求解程序接口
*/
void callPuzzleSolver()
{
	char* puzzle_filename = getPuzzleName();		//获取初始棋盘文件
	struct Puzzle p = loadPuzzleFromFile(puzzle_filename);		//从文件中读取棋盘
	solvePuzzle(p);		//求解数独
	free(puzzle_filename);		//释放空间
}


/**
@brief: 调用数独游玩程序接口
*/
void callPuzzlePlayer()
{
	struct Puzzle p = generatePuzzle();
	puzzlePlay(p);
}


/**
@brief: debug时使用函数, 负责将结果输出到屏幕
@param filename: cnf文件路径
@calls: loadFile(), copyFormula(), DPLL(), printArray()
*/
void debug(char* filename)
{
	float start = clock();
	struct Formula* formula = loadFile(filename);
	printFormula(formula);
	struct Formula* formula_copy = copyFormula(formula);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	printf("结果:\n");
	printf("%d\n", result.isSatisfied);
	printArray(result.res);
	printf("\n%f\n", finish - start);

	printf("验证:\n");
	struct Clause* test = formula->head->nextClause;
	while (!test->isLastClause) {
		printf("%d\n", evaluateClause(test, result.res));
		test = test->nextClause;
	}
	printf("公式结果:%d\n", evaluateFormula(formula, result.res));
	free(result.res);
	destoryFormula(formula);
}


/**
@brief: debug模式调用接口
*/
void callDebug()
{
	char* filename = getCnfFileName();
	debug(filename);
}


/**
@brief: 主控模块主程序
*/
void display()
{
	printGuide();		//打印说明界面
	modeChange();	//模式选择
	while (mode != ESC){		//当模式不是ESC时循环
		if (mode == CNF) {
			if(cnf_solver_version == 0) callCnfSolver();		//调用CNF求解模块
			if (cnf_solver_version == 1) callCnfSolverOpti();
		}
		if (mode == PUZZLE) callPuzzleSolver();		//调用数独求解模块
		if (mode == PUZZLE_PLAY) callPuzzlePlayer();	//调用数独游玩模块
		if (mode == DEBUG) callDebug();
		printf("/*按下ESC键退回到模式选择, 其他按键则继续当前模式.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}