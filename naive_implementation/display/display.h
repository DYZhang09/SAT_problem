/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: 主控、交互与显示模块                           **/
/**********************************************************/

#pragma once
#include"../solver/solver.h"
#include"../data_structure/data_struct.h"
#include"../cnfparser/cnfparser.h"



//-------------------------functions for debug-----------------------------------//
/**
@brief: 打印公式
@param formula: 指向公式的指针
*/
void printFormula(struct Formula* formula)
{
	int i = 1;
	struct Clause* currClause = formula->head->nextClause;
	while (!currClause->isLastClause) {
		struct Literal* currLit = currClause->head->nextLiteral;
		printf("Formula %d:\n", i++);
		while (!currLit->isTail) {
			printf("%d ", currLit->data);
			currLit = currLit->nextLiteral;
		}
		std::cout << currClause->len << '\n';
		currClause = currClause->nextClause;
	}
}


/**
@brief: 打印结果的数组
@param res: 结果数组
*/
void printArray(int* res)
{
	for (int i = 1; i <= info.num_literal; i++) {
		printf("%d ", res[i]);
		if (i % 10 == 0) printf("\n");
	}
}


//------------------------------------------------------------------------------------------
/**
@brief: 打印相关信息
*/
void printHelp()
{
	printf("/**************************************************************/\n");
	printf("/****************欢迎来到CNF公式求解程序***********************/\n");
	printf("/*********答案将写入与输入文件同文件夹下的res文件中************/\n");
	printf("/**************************************************************/\n");
}


/**
@brief: 显示交互环境，获取用户输入
@return: 输入的文件路径名
*/
char* getFileName()
{
	printf("\n/*请输入cnf文件路径:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
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
	fclose(fp);
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
@brief: 通过DPLL算法求解CNF范式
@param filename: cnf文件路径
@calls: loadFile(), copyFormula(), DPLL(), filePrint()
*/
void calculate(char* filename)
{
	float start = clock();
	struct Formula* formula = loadFile(filename);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	result.time = finish - start;

	char* path = strtok(filename, ".cnf");
	char* w_filename = strcat(path, ".res");
	filePrint(w_filename, result);
	free(result.res);
    destoryFormula(formula);
}

/**
@brief: 主程序
*/
void solve()
{
	char c;
//	struct Formula* formula;

	printHelp();
	do {
		char* filename = getFileName();
#ifdef DEBUG
		debug(filename);
#endif // DEBUG

#ifndef DEBUG
		calculate(filename);
#endif // !DEBUG
		free(filename);
		
		printf("按ESC退出, 其他键继续\n");
		c = getchar();
	} while ((c = getch()) != 27);
	printf("已退出\n");
}
