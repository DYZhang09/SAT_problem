/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独求解相关头文件		                          **/
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
	transformRule2(formula);
	printf("transform ok\n");
	printFormula(formula);
}

void test_comb()
{
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = 12 + i;
	comb(0, 0);
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 3; j++)
			printf("%d ", datas[i][j]);
		printf("\n");
	}
}