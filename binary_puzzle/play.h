/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/29                                                     
//* Description: ������Ϸ��غ���ͷ�ļ�                        
/**********************************************************/

#pragma once
#include"./tools/tool_function.h"
#include"./solver/solve_puzzle.h"

/**
@brief: ��������ģ��������
@param p: ����
*/
void puzzlePlay(struct Puzzle p)
{
	draw(p);

	info.num_literal = puzzle_size * puzzle_size;
	int flag = false;
	struct Puzzle p_ori = p;
	struct Formula* formula = transform(p);

	while (hasVacant(p)) {
		int* input = getPlayInput();
		if (input[2] != -1) {
			if(applyInput(&p, input))
				draw(p);
		}
		else {
			flag = true;
			break;
		}
	}
	if (!flag) {
		if (checkValid(p, formula)) printf("/*�����ȷ\n");
		else {
			printf("/*������\n");
		}
	}
	else solvePuzzle(p_ori, false);
}