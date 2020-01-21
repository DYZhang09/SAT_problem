/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: 主控、交互与显示模块                           **/
/**********************************************************/

#pragma once
#include"config.h"
#include"solver.h"
#include"data_struct.h"
#include"cnfparser.h"

/**
@brief: 显示交互环境，获取用户输入, 输入DEFAULT表示使用默认文件
@return: 输入的文件路径名, 若输入DEFAULT,则返回默认文件路径
*/
char* getFileName()
{
	char* filename = (char*)malloc(sizeof(char) * 255);
	printf("/**************************************************************/\n");
	printf("/****************欢迎来到CNF公式求解程序***********************/\n");
	printf("/*********答案将写入与输入文件同文件夹下的res文件中************/\n");
	printf("/**************************************************************/\n");
	printf("\n/*请输入cnf文件路径(斜杠请用双斜杠表示):\n");
	scanf("%s", filename);
	return filename;
}


/**
@brief: 将结果写入文件
@param filename: 目标文件的路径
@param result: 结果
*/
void filePrint(char* filename, struct Result result)
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
	printf("解答已写入文件:%s\n", filename);
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
	struct Formula* formula_copy = copyFormula(formula);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	printf("结果:\n");
	printf("%d\n", result.isSatisfied);
	printArray(result.res);
	printf("\n%f", finish - start);
}


/**
@brief: 通过DPLL算法求解CNF范式
@param filename: cnf文件路径
@calls: loadFile(), copyFormula(), DPLL(), filePrint()
*/
void calculate(char* filename)
{
	float start = clock();
	struct Formula* formula = loadFile(filename);
	struct Formula* formula_copy = copyFormula(formula);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	result.time = finish - start;

	char* path = strtok(filename, ".cnf");
	char* w_filename = strcat(path, ".res");
	filePrint(w_filename, result);
}