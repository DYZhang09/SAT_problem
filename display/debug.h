/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: debugģ�����ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"../naive_dpll/solver/solver.h"

/**
@brief: ��ӡ��ʽ
@param formula: ָ��ʽ��ָ��
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
@brief: ��ӡ���������
@param res: �������
*/
void printArray(short* res)
{
	for (int i = 1; i <= info.num_literal; i++) {
		printf("%d ", res[i]);
		if (i % 10 == 0) printf("\n");
	}
}


//@brief: ��ӡdebugģʽ�µ���Ϣ
void printDebugInfo(struct Result result, struct Formula* formula)
{
	printf("���:\n");
	printf("%d\n", result.isSatisfied);
	printArray(result.res);
	printf("\n%f\n", result.time);
	printf("��֤:\n");
	printf("��ʽ���:%d\n", evaluateFormula(formula, result.res, true));
}
