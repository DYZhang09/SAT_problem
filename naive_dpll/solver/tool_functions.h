/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/21                                                     
//* Description: ������cnf�������Ĺ��ߺ���                       
/**********************************************************/

#pragma once
#include"../data_structure/data_struct.h"


/**
@brief: �ж��Ӿ�������ָ��������
@param clause: ָ����Ҫ�жϵ��Ӿ��ָ��
@param data: ָ�����ֵ����
@return: �ҵ�����true, ����false
*/
bool hasData(struct Clause* clause, short data)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {		//�����ж�
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
	while (!curr->isLastClause) {		//�����ж�
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
short removeLiteralFromFormula(struct Formula* formula, short data)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//����ɾ��
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
short removeClauseHasLiteral(struct Formula* formula, short data)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//����ɾ��
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			target = curr;
			curr = curr->nextClause;
			//free(target);
			destroyClause(target);
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
short selectFirstData(struct Formula* formula)
{
	return	formula->head->nextClause->head->nextLiteral->data;
}


/**
@brief: �ӹ�ʽ��ѡȡһ�����Ӿ䲢�������������, �������ڵ��Ӿ��򷵻�0
@param formula: ָ��ʽ��ָ��
@calls: isUnitClause()
@return: ���Ӿ���������, �������ڵ��Ӿ䷵��0
*/
short selectDataFromUnitClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//������ֱ���ҵ�һ�����Ӿ�
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
bool evaluateClause(struct Clause* clause, short* var)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {		//���������������������ֵͬ��ʱ����Ϊ��
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
bool evaluateFormula(struct Formula* formula, short* var, bool verbose = false)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//������ֻҪ���Ӿ䲻������������ʽ��������
		bool flag = evaluateClause(curr, var);
		if (verbose) printf("%d\n", flag);
		if (!flag) return false;
		curr = curr->nextClause;
	}
	return true;
}


/**
@brief: ������ѡ�����ָ�ֵ�����д���
@param formula: ָ��ʽ��ָ��
@param res: ����������ָ�ֵ������
@param data: ��ѡ���������
*/
void applySelData(struct Formula* formula, short* res, short data)
{
	res[abs(data)] = data;
	removeClauseHasLiteral(formula, data);
	removeLiteralFromFormula(formula, -data);
}