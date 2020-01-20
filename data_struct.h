/***********************************************************/
/** author: Zhang DY                                                     **/
/** date: 2020/01/20                                                     **/
/** description: CNF��ʽ�洢���ݽṹ����غ���ʵ��    **/
/**********************************************************/

#pragma once
#include<stdio.h>
#include<stdlib.h>
#define DEBUG

//------------------------data structure---------------------//
struct Literal
{
	int data = 0;
	struct Literal* nextLiteral = NULL;
	struct Literal* beforeLiteral = NULL;
	bool isHead = false;
	bool isTail = false;
};

struct Clause
{
	struct Literal* head;
	struct Literal* tail;
	struct Clause* nextClause = NULL;
	struct Clause* beforeClause = NULL;

	bool isFirstClause = false;
	bool isLastClause = false;
	int len = 0;
};

struct Formula
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};

//-------------------------some functions---------------------------//
struct Clause* initClause()
{
	/**
	@brief: ��ʼ��һ���Ӿ�
	@return: ָ���ʼ���Ӿ��ָ��
	*/
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
			clause->tail->beforeLiteral = clause->head;
			clause->tail->isHead = false;
			clause->tail->isTail = true;
			clause->len = 0;
			return clause;
		}
	}
}

struct Formula* initFormula()
{
	/**
	@brief: ��ʼ��һ����ʽ
	@calls: initClause()
	@return: ָ���ʼ����ʽ��ָ��
	*/
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
		formula->tail->beforeClause = formula->head;
		formula->tail->isFirstClause = false;
		formula->tail->isLastClause = true;
		formula->num_clause = 0;
		return formula;
	}
}

void addLiteral(struct Clause* clause, int data)
{
	/**
	@brief: ��һ���Ӿ�β������һ������
	@param clause: ��Ҫ������ֵ�ָ���Ӿ��ָ��
	@param data: ������ֵ����
	*/
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

void deleteLiteral(struct Clause* clause, int data)
{
	/**
	@brief: ɾ��ָ���Ӿ��е�ָ������
	@param clause: ָ��ָ���Ӿ��ָ��
	@param data: ��Ҫɾ�������ֵ����
	*/
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

struct Clause* addClause(struct Formula* formula)
{
	/**
	@brief: ��ָ����ʽβ�����һ����ʽ
	@param formula: ָ���Ĺ�ʽ
	@return: ָ������ӹ�ʽ��ָ��
	*/
	struct Clause* tl = formula->tail;
	struct Clause* temp = initClause();
	if(temp){
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

void destroyClause(struct Clause* clause)
{
	/**
	@brief: ɾ��һ���Ӿ�
	@param clause: ָ��ɾ���Ӿ��ָ��
	*/
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

bool hasData(struct Clause* clause, int data)
{
	/**
	@brief: �ж��Ӿ�������ָ��������
	@param clause: ָ����Ҫ�жϵ��Ӿ��ָ��
	@param data: ָ�����ֵ����
	@return: �ҵ�����true, ����false
	*/
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if (curr->data == data) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}

int removeLiteralFromFormula(struct Formula* formula, int data)
{
	/**
	@brief: ɾ����ʽ�������Ӿ����������ض�����
	@param formula: ָ����Ҫɾ���Ӿ����ֵĹ�ʽ��ָ��
	@param data: ��Ҫɾ�������ֵ����
	@calls: deleteLiteral()
	@return: ��ɾ�������ֵ����
	*/
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		deleteLiteral(curr, data);
		curr = curr->nextClause;
	}
	return data;
}

int removeClauseHasLiteral(struct Formula* formula, int data)
{
	/**
	@brief: ɾ����ʽ�����а���ָ�����ֵ��Ӿ�
	@param formula: ָ����Ҫɾ���Ӿ�Ĺ�ʽ��ָ��
	@param data: ��Ҫɾ�������ֵ����
	@calls: hasData()
	@return: ��ɾ�������ֵ����
	*/
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

bool isUnitClause(struct Clause* clause)
{
	/**
	@brief: �ж�һ���Ӿ��Ƿ�Ϊ���Ӿ�
	@param clause: ָ����Ҫ�жϵ��Ӿ��ָ��
	@return: Ϊ���Ӿ���Ϊtrue, ����Ϊfalse
	*/
	return (clause->len == 1) ? true : false;
}

bool evaluateClause(struct Clause* clause, int* var)
{
	/**
	@brief: �����Ӿ����
	@param clause: ָ����Ҫ�����Ӿ��ָ��
	@param var: ��Ԫֵ����
	@return: �Ӿ�Ϊ����Ϊtrue, ����Ϊfalse
	*/
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if (curr->data * var[abs(curr->data)] > 0) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}

bool evaluateFormula(struct Formula* formula, int* var)
{
	/**
	@brief: ������ʽ�����
	@param formula: ָ����Ҫ������ʽ��ָ��
	@param var: ��Ԫֵ����
	@calls: evaluateClause()
	@return: ��ʽΪ����Ϊtrue, ����Ϊfalse
	*/
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {
		if (!evaluateClause(curr, var)) return false;
		else curr = curr->nextClause;
	}
	return true;
}

//-------------------------functions for debug-----------------------------------//
#ifdef DEBUG
void printFormula(struct Formula* formula)
{
	int i = 1;
	struct Clause* currClause = formula->head->nextClause;
	while (!currClause->isLastClause) {
		struct Literal* currLit = currClause->head->nextLiteral;
		printf("Formula %d:\n", i++);
		while (!currLit->isTail) {
			printf("%d ", currLit->data);
			currLit = currLit->nextLiteral;
		}
		std::cout << currClause->len<<'\n';
		currClause = currClause->nextClause;
	}
}
#endif // DEBUG

