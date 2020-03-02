/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/20                                                     
//* Description: ����DPLLģ��ͷ�ļ�                              
/**********************************************************/

#pragma once
#include"./tool_functions.h"
#include "../cnfparser/cnfparser.h"


bool dpll(struct Formula*, int*);

/**
@brief: dpll��֧����
@param formula: ָ��ʽ��ָ��
@param res: ��ű�����ֵ������
@param data: ���ݲ���ѡ����������
@return: ��ǰ��֧�Ƿ��ʹ��ʽ����
*/
bool branch(struct Formula* formula, int* res, int data)
{
	applySelData(formula, res, data);
	return dpll(formula, res);
}


/**
@brief: DPLL�㷨������
@param formula: �����Ĺ�ʽ
@return: �������򷵻�true, ���򷵻�false
*/
bool dpll(struct Formula* formula, int *res)
{
	if (formula->num_clause == 0) return true;
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {				//ѡȡ���Ӿ���е��Ӿ䴫��
		applySelData(formula, res, data_unit_clause);
		if (formula->num_clause == 0) return true;		//��ʽȫ��˵����ʽ������
		else if (hasVoidClause(formula)) return false;		//�п��Ӿ�˵����ʽ��������
	}
	struct Formula* formula_copy = copyFormula(formula);		//���ƹ�ʽ
	selected_data = selectFirstData(formula);			//ѡȡ��һ��Ԫ��
	if (branch(formula_copy, res, selected_data)) return true;			//��֧1
	else {		//��֧2
		destoryFormula(formula_copy);
		return branch(formula, res, -selected_data);		//��֧2
	}
}


/**
@brief: ����DPLL�㷨�������
@param formula: ָ��ʽ��ָ��
@calls: dpll()
@return: �ṹ��Result
*/
struct Result DPLL(struct Formula* formula)
{
	struct Result result;
	result.isSatisfied = false;		//��ʼ�����
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = dpll(formula, result.res);			//�������
	return result;
}
