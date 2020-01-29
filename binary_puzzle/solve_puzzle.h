/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ����������ͷ�ļ�		                          **/
/**********************************************************/

#pragma once
#include"transform.h"
#include"load_puzzle.h"
#include"print.h"
#include"../naive_implementation/solver/solver.h"


/**
@brief: ����������
@param p: ���������
@return: ���������
*/
struct Puzzle  solvePuzzle(struct Puzzle p)
{
	info.num_literal = puzzle_size * puzzle_size;
	struct Formula* formula = transform(p);
	struct Result result = DPLL(formula);
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + j + 1;
			p.puzzle[i][j] = (result.res[data] > 0) ? 1 : 0;
		}
	}
	drawTerminal(p);
	destoryFormula(formula);
	free(result.res);
	return p;
}