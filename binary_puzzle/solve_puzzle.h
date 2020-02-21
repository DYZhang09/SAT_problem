/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: 数独求解相关头文件		                          
/**********************************************************/

#pragma once
#include"transform.h"
#include"load_puzzle.h"
#include"print.h"
#include"../naive_implementation/solver/solver.h"



/**
@brief: 解数独函数
@param p: 待求解数独
@param verbose: 是否打印相关信息的标志
@return: 求解后的数独
*/
struct Puzzle  solvePuzzle(struct Puzzle p, bool verbose = true)
{
	if (verbose) {
		printf("待解答棋盘:\n");
		draw(p);
	}

	info.num_literal = puzzle_size * puzzle_size;		//初始化变量数
	struct Formula* formula = transform(p);				//根据棋盘转换为CNF公式
	printCnfIntoFile(formula);
	struct Result result = DPLL(formula);		//调用DPLL模块进行求解
	for (int i = 0; i < puzzle_size; i++) {			//棋盘赋值
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + j + 1;
			p.puzzle[i][j] = (result.res[data] > 0) ? 1 : 0;
		}
	}
	printf("解答:\n");
	draw(p);		//打印棋盘
	destoryFormula(formula);		//释放空间
	free(result.res);
	return p;
}