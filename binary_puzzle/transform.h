/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ����ת��ΪCNF���ͷ�ļ�		              **/
/**********************************************************/

#pragma once
#include"tool.h"


//---------------------------------Rule 0----------------------------------------//
/**
@brief: ��ȡ��ʼ��������, ���ɵ��Ӿ伯������빫ʽ��
@param formula: ָ��ʽ��ָ��
@param p: ��ʼ���������
*/
void transformUnitClause(struct Formula* formula, struct Puzzle p)
{
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + (j + 1);
			if (p.puzzle[i][j] > -1) {
				struct Clause* clause = initClause();
				if (p.puzzle[i][j] == 0) {
					addLiteral(clause, -data);
					addClause(formula, clause);
				}
				else {
					addLiteral(clause, data);
					addClause(formula, clause);
				}
			}
		}
	}
}


//----------------------------------------Rule 1----------------------------------------------//
/**
@brief: Ϊ����1�������Ƶ��Ӿ��������
@param clause_pos: ָ�����Ӿ��ָ��
@param clause_neg: ָ���Ӿ��ָ��
@param data: ��һ����������ֵ����
*/
void addLiteralForRow_Rule1(struct Clause* clause_pos, struct Clause* clause_neg, int data)
{
	addLiteral(clause_pos, data);
	addLiteral(clause_pos, data + 1);
	addLiteral(clause_pos, data + 2);
	addLiteral(clause_neg, -data);
	addLiteral(clause_neg, -(data + 1));
	addLiteral(clause_neg, -(data + 2));
}


/**
@brief: Ϊ����1�������Ƶ��Ӿ��������
@param clause_pos: ָ�����Ӿ��ָ��
@param clause_neg: ָ���Ӿ��ָ��
@param data: ��һ����������ֵ����
*/
void addLiteralForCol_Rule1(struct Clause* clause_pos, struct Clause* clause_neg, int data)
{
	addLiteral(clause_pos, data);
	addLiteral(clause_pos, data + puzzle_size);
	addLiteral(clause_pos, data + puzzle_size * 2);
	addLiteral(clause_neg, -data);
	addLiteral(clause_neg, -(data + puzzle_size));
	addLiteral(clause_neg, -(data + puzzle_size * 2));
}


/**
@brief: Ϊ����1�������������Ӿ䲢����빫ʽ
@param formula: ָ��ʽ��ָ��
@param data: ��һ������ӵ����ֵ����
*/
void addClauseForRow_Rule1(struct Formula* formula, int data)
{
	struct Clause* clause_pos = initClause();
	struct Clause* clause_neg = initClause();
	addLiteralForRow_Rule1(clause_pos, clause_neg, data);
	addClause(formula, clause_pos);
	addClause(formula, clause_neg);
}


/**
@brief: Ϊ����1�������������Ӿ䲢����빫ʽ
@param formula: ָ��ʽ��ָ��
@param data: ��һ������ӵ����ֵ����
*/
void addClauseForCol_Rule1(struct Formula* formula, int data)
{
	struct Clause* clause_pos = initClause();
	struct Clause* clause_neg = initClause();
	addLiteralForCol_Rule1(clause_pos, clause_neg, data);
	addClause(formula, clause_pos);
	addClause(formula, clause_neg);
}


/**
@brief: Ϊ����1�����Ӿ伯������빫ʽ
@param formula: ָ��ʽ��ָ��
*/
void transformRule1(struct Formula* formula)
{
	for (int i = 0; i < puzzle_size ; i++)
		for (int j = 0; j < puzzle_size - 2; j++) {
			int data = i * puzzle_size + (j + 1);
			addClauseForRow_Rule1(formula, data);
		}
	for (int i = 0; i < puzzle_size - 2; i++)
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + (j + 1);
			addClauseForCol_Rule1(formula, data);
		}
}


//-----------------------------------------Rule 2--------------------------------------------//
/**
@brief: Ϊ����2�����ɵ��ж�Ӧ���Ӿ伯������빫ʽ
@param formula: ָ��ʽ��ָ��
@param data: �����������
*/
void addClauseForEachRow_Rule2(struct Formula* formula, int data)
{
	int comb_arr[puzzle_size];
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = data + i;
	int** datas = setDataArr(C(N, M), M);
	comb(0, 0, comb_arr, datas, M);
	for (int i = 0; i < CNUM; i++) {
		struct Clause* clause_pos = initClause();
		struct Clause* clause_neg = initClause();
		for (int j = 0; j < M; j++) {
			addLiteral(clause_pos, datas[i][j]);
			addLiteral(clause_neg, -datas[i][j]);
		}
		addClause(formula, clause_pos);
		addClause(formula, clause_neg);
	}
	data_cnt = 0;
}


/**
@brief: Ϊ����2�����ɵ��ж�Ӧ���Ӿ伯������빫ʽ
@param formula: ָ��ʽ��ָ��
@param data: �����������
*/
void addClauseForEachCol_Rule2(struct Formula* formula, int data)
{
	int comb_arr[puzzle_size];
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = data + i * puzzle_size;
	int** datas = setDataArr(C(N, M), M);
	comb(0, 0, comb_arr, datas, M);
	for (int i = 0; i < CNUM; i++) {
		struct Clause* clause_pos = initClause();
		struct Clause* clause_neg = initClause();
		for (int j = 0; j < M; j++) {
			addLiteral(clause_pos, datas[i][j]);
			addLiteral(clause_neg, -datas[i][j]);
		}
		addClause(formula, clause_pos);
		addClause(formula, clause_neg);
	}
	data_cnt = 0;
}


/**
@brief: Ϊ����2�������������Ӿ伯������빫ʽ
@param formula: ָ��ʽ��ָ��
*/
void addClauseForRow_Rule2(struct Formula* formula)
{
	for (int i = 0; i < puzzle_size; i++) {
		int data = i * puzzle_size + 1;
		addClauseForEachRow_Rule2(formula, data);
	}
}


/**
@brief: Ϊ����2�������������Ӿ伯������빫ʽ
@param formula: ָ��ʽ��ָ��
*/
void addClauseForCol_Rule2(struct Formula* formula)
{
	for (int j = 0; j < puzzle_size; j++) {
		int data = j + 1;
		addClauseForEachCol_Rule2(formula, data);
	}
}


/**
@brief: Ϊ����2�����Ӿ伯������빫ʽ
@param formula: ָ��ʽ��ָ��
*/
void transformRule2(struct Formula* formula)
{
	addClauseForRow_Rule2(formula);
	addClauseForCol_Rule2(formula);
}


//---------------------------------------Rule 3---------------------------------------------------//
/**
@brief: ���Ӿ������������, �����Ӿ�����빫ʽ��(�����ڹ���3��غ���ʵ��)
@param formula: ָ��ʽ��ָ��
@param datas: ��������ֵ���ŵ�����
@param len: datas����
*/
void addClauseWithArr(struct Formula* formula, int* datas, int len)
{
	struct Clause* clause = initClause();
	for (int i = 0; i < len; i++)
		addLiteral(clause, datas[i]);
	addClause(formula, clause);
}


/**
@brief: ����all����(�����ڹ���3��غ���ʵ��)
@param all: ��Ҫ���õ�����
*/
void recoverAllArr(int* all)
{
	for (int i = 0; i < 2 * puzzle_size; i++)
		all[i] = abs(all[i]);
}


/**
@brief: ����all��������Ԫ�ص������������Ӿ伯(�����ڹ���3��غ���ʵ��)
@param formula: ָ��ʽ��ָ��
@param all: all����
@param comb_arr: ��Ҫ������ϵ��������
*/
void addClauseForRule3(struct Formula* formula, int* all, int* comb_arr)
{
	addClauseWithArr(formula, all, 2 * puzzle_size);
	for (int m = 1; m <= puzzle_size; m++) {
		int** datas = setDataArr(C(N, m), m);
		comb(0, 0, comb_arr, datas, m);
		data_cnt = 0;
		for (int row = 0; row < C(N, m); row++) {
			for (int col = 0; col < m; col++) {
				int index = datas[row][col] - 1;
				all[index * 2] *= -1;
				all[index * 2 + 1] *= -1;
			}
			addClauseWithArr(formula, all, 2 * puzzle_size);
			recoverAllArr(all);
		}
	}
}


/**
@brief: Ϊ����3����������������Ӿ伯
@param formula: ָ��ʽ��ָ��
@param data1: ��һ�е������������
@param data2: �ڶ��е������������
*/
void addClauseForTwoRows_Rule3(struct Formula* formula, int data1, int data2)
{
	int all[2 * puzzle_size];
	int comb_arr[puzzle_size];
	for (int i = 0; i < puzzle_size; i++) {
		comb_arr[i] = i + 1;
		all[2 * i] = data1 + i;
		all[2 * i + 1] = data2 + i;
	}
	addClauseForRule3(formula, all, comb_arr);
}


/**
@brief: Ϊ����3����������������Ӿ伯
@param formula: ָ��ʽ��ָ��
@param data1: ��һ�е������������
@param data2: �ڶ��е������������
*/
void addClauseForTwoCols_Rule3(struct Formula* formula, int data1, int data2)
{
	int all[2 * puzzle_size];
	int comb_arr[puzzle_size];
	for (int i = 0; i < puzzle_size; i++) {
		comb_arr[i] = i + 1;
		all[2 * i] = data1 + i * puzzle_size;
		all[2 * i + 1] = data2 + i * puzzle_size;
	}
	addClauseForRule3(formula, all, comb_arr);
}


/**
@brief: Ϊ����3������������Ӿ伯
@param formula: ָ��ʽ��ָ��
*/
void addClauseForRow_Rule3(struct Formula* formula)
{
	int comb_arr[puzzle_size];
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = i * puzzle_size + 1;
	int** datas = setDataArr(C(N, 2), 2);
	comb(0, 0, comb_arr, datas, 2);
	data_cnt = 0;
	for (int i = 0; i < C(N, 2); i++) {
		addClauseForTwoRows_Rule3(formula, datas[i][0], datas[i][1]);
	}
}


/**
@brief: Ϊ����3������������Ӿ伯
@param formula: ָ��ʽ��ָ��
*/
void addClauseForCol_Rule3(struct Formula* formula)
{
	int comb_arr[puzzle_size];
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = i + 1;
	int** datas = setDataArr(C(N, 2), 2);
	comb(0, 0, comb_arr, datas, 2);
	data_cnt = 0;
	for (int i = 0; i < C(N, 2); i++) {
		addClauseForTwoCols_Rule3(formula, datas[i][0], datas[i][1]);
	}
}


/**
@brief: Ϊ����3����Ӿ伯
@param formula: ָ��ʽ��ָ��
*/
void transformRule3(struct Formula* formula)
{
	addClauseForRow_Rule3(formula);
	addClauseForCol_Rule3(formula);
}


//-------------------------------------------------------------------------------------
/**
@brief: ������ת��ΪCNF��ʽ
@param p: ��Ҫת��������
@return: ת���������
*/
struct Formula* transform(struct Puzzle p)
{
	struct Formula* formula = initFormula();
	transformUnitClause(formula, p);
	transformRule1(formula);
	transformRule2(formula);
	transformRule3(formula);
	return formula;
}