/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: ����������ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"transform.h"
#include"load_puzzle.h"
#include"print.h"
#include"../naive_implementation/solver/solver.h"



/**
@brief: ����������
@param p: ���������
@param verbose: �Ƿ��ӡ�����Ϣ�ı�־
@return: ���������
*/
struct Puzzle  solvePuzzle(struct Puzzle p, bool verbose = true)
{
	if (verbose) {
		printf("���������:\n");
		draw(p);
	}

	info.num_literal = puzzle_size * puzzle_size;		//��ʼ��������
	struct Formula* formula = transform(p);				//��������ת��ΪCNF��ʽ
	printCnfIntoFile(formula);
	struct Result result = DPLL(formula);		//����DPLLģ��������
	for (int i = 0; i < puzzle_size; i++) {			//���̸�ֵ
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + j + 1;
			p.puzzle[i][j] = (result.res[data] > 0) ? 1 : 0;
		}
	}
	printf("���:\n");
	draw(p);		//��ӡ����
	destoryFormula(formula);		//�ͷſռ�
	free(result.res);
	return p;
}