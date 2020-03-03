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
	short data = 0;
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
	short len = 0;
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
	short num_literal;
	int num_clause;
}info;


struct Result
{
	bool isSatisfied;
	short* res;
	int* counter;
	float time;
};


//----------------------------------------functions---------------------------------------------------//
/**
@brief: ��ʼ��һ������
@param ... : Literal����Ա��ʼֵ
@return: ��ʼ�����ָ�����ֵ�ָ��
*/
struct Literal* initLiteral(short data = 0, struct Literal* nextLiteral = NULL, struct Literal* beforeLiteral = NULL,
										bool isHead = false, bool isTail = false)
{
	struct Literal* new_lit = (struct Literal*)malloc(sizeof(struct Literal));
	new_lit->data = data;
	new_lit->nextLiteral = nextLiteral;
	new_lit->beforeLiteral = beforeLiteral;
	new_lit->isHead = isHead;
	new_lit->isTail = isTail;
	return new_lit;
}


/**
@brief: ��ʼ��һ���Ӿ�
@param ... : Clause�ĳ�Ա��ʼֵ
@return: ָ���ʼ���Ӿ��ָ��
*/
struct Clause* initClause(struct Clause* nextClause = NULL,struct Clause* beforeClause = NULL, 
										bool isFirstCla = false, bool isLastCla = false)
{
	struct Clause* clause = (struct Clause*) malloc(sizeof(struct Clause));			//��̬�����Ӿ�ռ�
	clause->head = initLiteral(0, NULL, NULL, true, false);
	clause->tail = initLiteral(0, NULL, clause->head, false, true);
	clause->head->nextLiteral = clause->tail;
	clause->nextClause = nextClause;
	clause->beforeClause = beforeClause;
	clause->isFirstClause = isFirstCla;
	clause->isLastClause = isLastCla;
	clause->len = 0;
	return clause;
}


/**
@brief: ��ʼ��һ����ʽ
@calls: initClause()
@return: ָ���ʼ����ʽ��ָ��
*/
struct Formula* initFormula()
{
	struct Formula* formula = (struct Formula*)malloc(sizeof(struct Formula));		//��̬���乫ʽ�ռ�
	formula->head = initClause(NULL, NULL, true, false);
	formula->tail = initClause(NULL, formula->head, false, true);
	formula->head->nextClause = formula->tail;
	formula->num_clause = 0;
	return formula;
}


/*
@brief: ��ʼ��һ���𰸽ṹ��
@return: ��ʼ����Ĵ𰸽ṹ��
*/
struct Result initResult()
{
	struct Result result;
	result.isSatisfied = false;		//��ʼ�����
	result.res = (short*)malloc(sizeof(short) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(short) * (info.num_literal + 1));
	return result;
}


/**
@brief: ��һ���Ӿ�β������һ������
@param clause: ��Ҫ������ֵ�ָ���Ӿ��ָ��
@param data: ������ֵ����
*/
void addLiteral(struct Clause* clause, short data)
{
	struct Literal* tl = clause->tail;
	struct Literal* temp = initLiteral(data, tl, tl->beforeLiteral);
	tl->beforeLiteral->nextLiteral = temp;		//�������
	tl->beforeLiteral = temp;
	clause->len++;

}


/**
@brief: ɾ��ָ���Ӿ��е�ָ������
@param clause: ָ��ָ���Ӿ��ָ��
@param data: ��Ҫɾ�������ֵ����
*/
void deleteLiteral(struct Clause* clause, short data)
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
	struct Clause* temp = initClause(tl, tl->beforeClause);
	tl->beforeClause->nextClause = temp;
	tl->beforeClause = temp;
	formula->num_clause++;
	return temp;
}


/**
@brief: ����ʽĩβ���һ���Ӿ�
@param formula: ָ��ʽ��ָ��
@param clause: ָ������Ӿ��ָ��
*/
void addClause(struct Formula* formula, struct Clause* clause)
{
	struct Clause* tl = formula->tail;
	clause->isFirstClause = clause->isLastClause = false;		//����Ӿ�
	tl->beforeClause->nextClause = clause;
	clause->beforeClause = tl->beforeClause;
	clause->nextClause = tl;
	tl->beforeClause = clause;
	formula->num_clause++;
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