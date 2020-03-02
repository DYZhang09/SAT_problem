/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: debug模块相关头文件		                          
/**********************************************************/

#pragma once
#include"../naive_dpll/solver/solver.h"

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


