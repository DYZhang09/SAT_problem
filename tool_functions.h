/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: ���ߺ�����������ͷ�ļ�                        **/
/**********************************************************/

#pragma once
#include"data_struct.h"


/**
@brief: ��ʼ��һ���Ӿ�
@return: ָ���ʼ���Ӿ��ָ��
*/
struct Clause* initClause()
{
	struct Clause* clause = (struct Clause*) malloc(sizeof(struct Clause));
	if (!clause) {
		printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
		return NULL;
	}
	else {
		clause->head = (struct Literal*)malloc(sizeof(struct Literal));
		clause->tail = (struct Literal*)malloc(sizeof(struct Literal));
		if (!clause->head or !clause->tail) {
			printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
			return NULL;
		}
		else {
			clause->head->nextLiteral = clause->tail;
			clause->head->isHead = true;
			clause->head->isTail = false;
			clause->head->beforeLiteral = NULL;
			clause->tail->beforeLiteral = clause->head;
			clause->tail->isHead = false;
			clause->tail->isTail = true;
			clause->tail->nextLiteral = NULL;
			clause->len = 0;
			return clause;
		}
	}
}


/**
@brief: ��ʼ��һ����ʽ
@calls: initClause()
@return: ָ���ʼ����ʽ��ָ��
*/
struct Formula* initFormula()
{
	struct Formula* formula = (struct Formula*)malloc(sizeof(struct Formula));
	if (!formula) {
		printf("malloc error.\nFileName:data_struct\nFunc:initFormula\n");
		return NULL;
	}
	else {
		formula->head = initClause();
		formula->tail = initClause();
		formula->head->nextClause = formula->tail;
		formula->head->isFirstClause = true;
		formula->head->isLastClause = false;
		formula->head->beforeClause = NULL;
		formula->tail->beforeClause = formula->head;
		formula->tail->isFirstClause = false;
		formula->tail->isLastClause = true;
		formula->tail->nextClause = NULL;
		formula->num_clause = 0;
		return formula;
	}
}


/**
@brief: ��һ���Ӿ�β������һ������
@param clause: ��Ҫ������ֵ�ָ���Ӿ��ָ��
@param data: ������ֵ����
*/
void addLiteral(struct Clause* clause, int data)
{
	struct Literal* tl = clause->tail;
	struct Literal* temp = (struct Literal*)malloc(sizeof(struct Literal));
	if (temp) {
		temp->data = data;
		temp->isTail = false;
		temp->isHead = false;
		tl->beforeLiteral->nextLiteral = temp;
		temp->beforeLiteral = tl->beforeLiteral;
		temp->nextLiteral = tl;
		tl->beforeLiteral = temp;
		clause->len++;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addLiteral\n");
	}
}


/**
@brief: ɾ��ָ���Ӿ��е�ָ������
@param clause: ָ��ָ���Ӿ��ָ��
@param data: ��Ҫɾ�������ֵ����
*/
void deleteLiteral(struct Clause* clause, int data)
{
	struct Literal* curr = clause->head->nextLiteral;
	struct Literal* target = curr;
	while (!curr->isTail) {
		if (curr->data == data) {
			target = curr;
			target->beforeLiteral->nextLiteral = target->nextLiteral;
			target->nextLiteral->beforeLiteral = target->beforeLiteral;
			curr = curr->nextLiteral;
			free(target);
			clause->len--;
		}
		else curr = curr->nextLiteral;
	}
}


/**
@brief: ��ָ����ʽβ�����һ����ʽ
@param formula: ָ���Ĺ�ʽ
@return: ָ������ӹ�ʽ��ָ��
*/
struct Clause* addClause(struct Formula* formula)
{
	struct Clause* tl = formula->tail;
	struct Clause* temp = initClause();
	if (temp) {
		temp->isFirstClause = temp->isLastClause = false;
		tl->beforeClause->nextClause = temp;
		temp->beforeClause = tl->beforeClause;
		temp->nextClause = tl;
		tl->beforeClause = temp;
		formula->num_clause++;
		return temp;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addClause\n");
		return NULL;
	}
}


/**
@brief: ���ƹ�ʽ
@param formula: ָ�򱻸��ƹ�ʽ��ָ��
@calls: initFormula(), addClause(), addLiteral()
@return: ָ���ƺ�ʽ��ָ��
*/
struct Formula* copyFormula(struct Formula* formula)
{
	struct Formula* formula_copy = initFormula();
	struct Clause* curr_clause = formula->head->nextClause;
	struct Clause* curr_clause_copy = formula_copy->head;
	while (!curr_clause->isLastClause) {
		curr_clause_copy = addClause(formula_copy);
		struct Literal* curr_literal = curr_clause->head->nextLiteral;
		while (!curr_literal->isTail) {
			addLiteral(curr_clause_copy, curr_literal->data);
			curr_literal = curr_literal->nextLiteral;
		}
		curr_clause = curr_clause->nextClause;
	}
	return formula_copy;
}


/**
@brief: ɾ��һ���Ӿ�
@param clause: ָ��ɾ���Ӿ��ָ��
*/
void destroyClause(struct Clause* clause)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
}


/**
@brief: ɾ����ʽ, �黹��̬�����ڴ�
@param formula: ָ��ɾ����ʽ��ָ��
@calls: destroyFormula()
*/
void destoryFormula(struct Formula* formula)
{
	struct Clause* curr = formula->tail;
	struct Clause* prev = curr->beforeClause;
	while (prev != NULL) {
		destroyClause(curr);
		curr = prev;
		prev = prev->beforeClause;
	}
	formula->num_clause = 0;
	destroyClause(formula->head);
}


/**
@brief: �ж��Ӿ�������ָ��������
@param clause: ָ����Ҫ�жϵ��Ӿ��ָ��
@param data: ָ�����ֵ����
@return: �ҵ�����true, ����false
*/
bool hasData(struct Clause* clause, int data)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if (curr->data == data) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}


/**
@brief: �жϹ�ʽ���Ƿ��п��Ӿ�
@param formula: ָ��ʽ��ָ��
@return: �п��Ӿ䷵��true, ���򷵻�false
*/
bool hasVoidClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		if (curr->len == 0) return true;
		else curr = curr->nextClause;
	}
	return false;
}


/**
@brief: ɾ����ʽ�������Ӿ����������ض�����
@param formula: ָ����Ҫɾ���Ӿ����ֵĹ�ʽ��ָ��
@param data: ��Ҫɾ�������ֵ����
@calls: deleteLiteral()
@return: ��ɾ�������ֵ����
*/
int removeLiteralFromFormula(struct Formula* formula, int data)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		deleteLiteral(curr, data);
		curr = curr->nextClause;
	}
	return data;
}


/**
@brief: ɾ����ʽ�����а���ָ�����ֵ��Ӿ�
@param formula: ָ����Ҫɾ���Ӿ�Ĺ�ʽ��ָ��
@param data: ��Ҫɾ�������ֵ����
@calls: hasData()
@return: ��ɾ�������ֵ����
*/
int removeClauseHasLiteral(struct Formula* formula, int data)
{
	struct Clause* traget;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			traget = curr;
			curr = curr->nextClause;
			free(traget);
			formula->num_clause--;
		}
		else curr = curr->nextClause;
	}
	return data;
}


/**
@brief: �ж�һ���Ӿ��Ƿ�Ϊ���Ӿ�
@param clause: ָ����Ҫ�жϵ��Ӿ��ָ��
@return: Ϊ���Ӿ���Ϊtrue, ����Ϊfalse
*/
bool isUnitClause(struct Clause* clause)
{
	return (clause->len == 1) ? true : false;
}


/**
@brief: ѡȡ��ʽ�ĵ�һ���Ӿ�ĵ�һ������
@param formula: ָ��ʽ��ָ��
@return: ѡȡ�����ֵ����
*/
int selectFirstData(struct Formula* formula)
{
	return formula->head->nextClause->head->nextLiteral->data;
}


/**
@brief: �ӹ�ʽ��ѡȡһ�����Ӿ䲢�������������, �������ڵ��Ӿ��򷵻�0
@param formula: ָ��ʽ��ָ��
@calls: isUnitClause()
@return: ���Ӿ���������, �������ڵ��Ӿ䷵��0
*/
int selectDataFromUnitClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		if (isUnitClause(curr)) return curr->head->nextLiteral->data;
		else curr = curr->nextClause;
	}
	return 0;
}


/**
@brief: �����Ӿ����
@param clause: ָ����Ҫ�����Ӿ��ָ��
@param var: ��Ԫֵ����
@return: �Ӿ�Ϊ����Ϊtrue, ����Ϊfalse
*/
bool evaluateClause(struct Clause* clause, int* var)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if (curr->data * var[abs(curr->data)] >= 0) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}


/**
@brief: ������ʽ�����
@param formula: ָ����Ҫ������ʽ��ָ��
@param var: ��Ԫֵ����
@calls: evaluateClause()
@return: ��ʽΪ����Ϊtrue, ����Ϊfalse
*/
bool evaluateFormula(struct Formula* formula, int* var)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		bool flag = evaluateClause(curr, var);
		if (!flag) return false;
		else {
			curr = curr->nextClause;
		}
	}
	return true;
}