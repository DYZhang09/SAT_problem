/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独转换为CNF相关头文件		              **/
/**********************************************************/

#pragma once
#include"tool.h"


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


/**
@brief: 为规则2的生成单行对应的子句集并添加入公式
@param formula: 指向公式的指针
@param data: 行首文字序号
*/
void addClauseForEachRow_Rule2(struct Formula* formula, int data)
{
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = data + i;
	comb(0, 0);
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
	for (int i = 0; i < puzzle_size; i++)
		comb_arr[i] = data + i * puzzle_size;
	comb(0, 0);
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