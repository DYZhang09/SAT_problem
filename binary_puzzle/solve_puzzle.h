/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ����������ͷ�ļ�		                          **/
/**********************************************************/

#pragma once
#include"transform.h"

void test_puzzle()
{
	struct Formula* formula = initFormula();
	struct Puzzle p = generatePuzzle();
	draw(p);
	transformUnitClause(formula, p);
	transformRule1(formula);
	printf("transform ok\n");
	printFormula(formula);
}