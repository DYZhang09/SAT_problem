/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独转换为CNF相关头文件		              **/
/**********************************************************/

#pragma once
#include"tool.h"


//---------------------------------Rule 0----------------------------------------//
/**
@brief: 读取初始化的棋盘, 生成单子句集并添加入公式中
@param formula: 指向公式的指针
@param p: 初始化后的棋盘
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
@brief: 为规则1的行限制的子句添加文字
@param clause_pos: 指向正子句的指针
@param clause_neg: 指向负子句的指针
@param data: 第一个被添加文字的序号
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
@brief: 为规则1的列限制的子句添加文字
@param clause_pos: 指向正子句的指针
@param clause_neg: 指向负子句的指针
@param data: 第一个被添加文字的序号
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
@brief: 为规则1的行限制生成子句并添加入公式
@param formula: 指向公式的指针
@param data: 第一个被添加的文字的序号
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
@brief: 为规则1的列限制生成子句并添加入公式
@param formula: 指向公式的指针
@param data: 第一个被添加的文字的序号
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
@brief: 为规则1生成子句集并添加入公式
@param formula: 指向公式的指针
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
@brief: 为规则2的生成单行对应的子句集并添加入公式
@param formula: 指向公式的指针
@param data: 行首文字序号
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
@brief: 为规则2的生成单列对应的子句集并添加入公式
@param formula: 指向公式的指针
@param data: 列首文字序号
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
@brief: 为规则2的行限制生成子句集并添加入公式
@param formula: 指向公式的指针
*/
void addClauseForRow_Rule2(struct Formula* formula)
{
	for (int i = 0; i < puzzle_size; i++) {
		int data = i * puzzle_size + 1;
		addClauseForEachRow_Rule2(formula, data);
	}
}


/**
@brief: 为规则2的列限制生成子句集并添加入公式
@param formula: 指向公式的指针
*/
void addClauseForCol_Rule2(struct Formula* formula)
{
	for (int j = 0; j < puzzle_size; j++) {
		int data = j + 1;
		addClauseForEachCol_Rule2(formula, data);
	}
}


/**
@brief: 为规则2生成子句集并添加入公式
@param formula: 指向公式的指针
*/
void transformRule2(struct Formula* formula)
{
	addClauseForRow_Rule2(formula);
	addClauseForCol_Rule2(formula);
}


//---------------------------------------Rule 3---------------------------------------------------//
/**
@brief: 给子句批量添加文字, 并将子句添加入公式中(仅用于规则3相关函数实现)
@param formula: 指向公式的指针
@param datas: 待添加文字的序号的数组
@param len: datas长度
*/
void addClauseWithArr(struct Formula* formula, int* datas, int len)
{
	struct Clause* clause = initClause();
	for (int i = 0; i < len; i++)
		addLiteral(clause, datas[i]);
	addClause(formula, clause);
}


/**
@brief: 重置all数组(仅用于规则3相关函数实现)
@param all: 需要重置的数组
*/
void recoverAllArr(int* all)
{
	for (int i = 0; i < 2 * puzzle_size; i++)
		all[i] = abs(all[i]);
}


/**
@brief: 根据all数组里面元素的排列组合添加子句集(仅用于规则3相关函数实现)
@param formula: 指向公式的指针
@param all: all数组
@param comb_arr: 需要排列组合的序号数组
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
@brief: 为规则3任意两行限制添加子句集
@param formula: 指向公式的指针
@param data1: 第一行的行首文字序号
@param data2: 第二行的行首文字序号
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
@brief: 为规则3任意两列限制添加子句集
@param formula: 指向公式的指针
@param data1: 第一行的列首文字序号
@param data2: 第二行的列首文字序号
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
@brief: 为规则3的行限制添加子句集
@param formula: 指向公式的指针
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
@brief: 为规则3的列限制添加子句集
@param formula: 指向公式的指针
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
@brief: 为规则3添加子句集
@param formula: 指向公式的指针
*/
void transformRule3(struct Formula* formula)
{
	addClauseForRow_Rule3(formula);
	addClauseForCol_Rule3(formula);
}


//-------------------------------------------------------------------------------------
/**
@brief: 将数独转换为CNF公式
@param p: 需要转换的数独
@return: 转换后的数独
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