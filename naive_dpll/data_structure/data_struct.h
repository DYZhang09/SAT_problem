/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/20                                                     
//* Description: CNF��ʽ�洢���ݽṹ����					  
/**********************************************************/

#pragma once
#include"../../config/config.h"

//------------------------naive data structure---------------------//
struct Literal			//���ֽṹ�壬data��Ӧ�������
{
	int data = 0;
	struct Literal* nextLiteral = NULL;
	struct Literal* beforeLiteral = NULL;
	bool isHead = false;
	bool isTail = false;
};

struct Clause		//�Ӿ�ṹ��
{
	struct Literal* head;
	struct Literal* tail;
	struct Clause* nextClause = NULL;
	struct Clause* beforeClause = NULL;

	bool isFirstClause = false;
	bool isLastClause = false;
	int len = 0;
};

struct Formula		//��ʽ�ṹ��
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};


//CNF������Ϣ�ṹ��
struct ProblemInfo
{
	int num_clause;
	int num_literal;
}info;


struct Result
{
	bool isSatisfied;
	int* res;
	int* counter;
	float time;
};


//----------------------------------------functions---------------------------------------------------//
/**
@brief: ��ʼ��һ���Ӿ�
@return: ָ���ʼ���Ӿ��ָ��
*/
struct Clause* initClause()
{
	struct Clause* clause = (struct Clause*) malloc(sizeof(struct Clause));			//��̬�����Ӿ�ռ�
	if (!clause) {
		printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
		return NULL;
	}
	else {
		clause->head = (struct Literal*)malloc(sizeof(struct Literal));			//��̬�����Ӿ���β�ռ�
		clause->tail = (struct Literal*)malloc(sizeof(struct Literal));
		if (!clause->head or !clause->tail) {
			printf("malloc error.\nFileName:data_struct\nFunc:initClause\n");
			return NULL;
		}
		else {
			clause->head->nextLiteral = clause->tail;			//��ʼ���Ӿ�
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
	struct Formula* formula = (struct Formula*)malloc(sizeof(struct Formula));		//��̬���乫ʽ�ռ�
	if (!formula) {
		printf("malloc error.\nFileName:data_struct\nFunc:initFormula\n");
		return NULL;
	}
	else {
		formula->head = initClause();			//��ʼ����ʽ
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
	struct Literal* temp = (struct Literal*)malloc(sizeof(struct Literal));			//��̬�������ֿռ�
	if (temp) {
		temp->data = data;			//��ʼ������
		temp->isTail = false;
		temp->isHead = false;
		tl->beforeLiteral->nextLiteral = temp;		//�������
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
		if (curr->data == data) {		//��λ��Ҫɾ��������
			target = curr;
			target->beforeLiteral->nextLiteral = target->nextLiteral;		//ɾ����Ӧ����
			target->nextLiteral->beforeLiteral = target->beforeLiteral;
			curr = curr->nextLiteral;
			free(target);
			clause->len--;
		}
		else curr = curr->nextLiteral;
	}
}


/**
@brief: ��ָ����ʽβ������һ���Ӿ�
@param formula: ָ���Ĺ�ʽ
@return: ָ��������Ӿ��ָ��
*/
struct Clause* createClause(struct Formula* formula)
{
	struct Clause* tl = formula->tail;
	struct Clause* temp = initClause();			//��ʼ���Ӿ�
	if (temp) {
		temp->isFirstClause = temp->isLastClause = false;		//����Ӿ�
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
@brief: ����ʽĩβ���һ���Ӿ�
@param formula: ָ��ʽ��ָ��
@param clause: ָ������Ӿ��ָ��
*/
void addClause(struct Formula* formula, struct Clause* clause)
{
	struct Clause* tl = formula->tail;
	struct Clause* temp = clause;
	if (temp) {
		temp->isFirstClause = temp->isLastClause = false;		//����Ӿ�
		tl->beforeClause->nextClause = temp;
		temp->beforeClause = tl->beforeClause;
		temp->nextClause = tl;
		tl->beforeClause = temp;
		formula->num_clause++;
	}
	else {
		printf("malloc error.\n FileName:data_struct.h\nFunc:addClause\n");
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
	struct Formula* formula_copy = initFormula();		//��ʼ����ʽ�ռ�
	struct Clause* curr_clause = formula->head->nextClause;
	struct Clause* curr_clause_copy = formula_copy->head;
	while (!curr_clause->isLastClause) {			//�������ƹ�ʽ
		curr_clause_copy = createClause(formula_copy);
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
	while (prev != NULL) {		//����ɾ������
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
	free(clause);
}


/**
@brief: ɾ����ʽ, �黹��̬�����ڴ�
@param formula: ָ��ɾ����ʽ��ָ��
@calls: destroyClause()
*/
void destoryFormula(struct Formula* formula)
{
	struct Clause* curr = formula->tail;
	struct Clause* prev = curr->beforeClause;
	while (prev != NULL) {		//����ɾ���Ӿ伯
		destroyClause(curr);
		curr = prev;
		prev = prev->beforeClause;
	}
	formula->num_clause = 0;
	destroyClause(formula->head);
	free(formula);
}