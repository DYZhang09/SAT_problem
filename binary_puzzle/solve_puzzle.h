/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独求解相关头文件		                          **/
/**********************************************************/

#pragma once
#include"transform.h"
#include"load_puzzle.h"
#include"../naive_implementation/solver/solver.h"


struct Puzzle  solvePuzzle()
{
	info.num_literal = puzzle_size * puzzle_size;
	const char* filename = "E:\\SAT_Problem_\\SAT_problem\\test.res";
	struct Puzzle p = loadPuzzleFromFile(filename);
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