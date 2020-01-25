/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ����ת��ΪCNF���ͷ�ļ�		              **/
/**********************************************************/

#pragma once
#include"generate.h"
#include"../naive_implementation/data_structure/tool_functions.h"


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
@brief: Ϊ����1�������������Ӿ伯������빫ʽ
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
@brief: Ϊ����1�������������Ӿ伯������빫ʽ
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
@param data: ��һ������ӵ����ֵ����
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