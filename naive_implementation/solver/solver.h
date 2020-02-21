/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: ����DPLLģ��ͷ�ļ�                              **/
/**********************************************************/

#pragma once
#include"../data_structure/tool_functions.h"
#include"../cnfparser/cnfparser.h"


/**
@brief: DPLL�㷨������
@param formula: �����Ĺ�ʽ
@return: �������򷵻�true, ���򷵻�false
*/
bool dpll(struct Formula* formula, int *res)
{
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {				//ѡȡ���Ӿ���е��Ӿ䴫��
		res[abs(data_unit_clause)] = (data_unit_clause > 0 ? abs(data_unit_clause) : -abs(data_unit_clause));		//��ֵ
		/*removeClauseHasLiteral(formula, data_unit_clause);
		removeLiteralFromFormula(formula, -data_unit_clause);*/
		unitClauseRule(formula, data_unit_clause);
		if (formula->num_clause == 0) return true;		//��ʽȫ��˵����ʽ������
		else if (hasVoidClause(formula)) return false;		//�п��Ӿ�˵����ʽ��������
	}
	if (formula->num_clause == 0) return true;

	struct Formula* formula_copy = copyFormula(formula);		//���ƹ�ʽ
	selected_data = selectFirstData(formula);			//ѡȡ��һ��Ԫ��
	int num = formula->num_clause;
	res[abs(selected_data)] = abs(selected_data);			
	removeClauseHasLiteral(formula_copy, abs(selected_data));
	removeLiteralFromFormula(formula_copy, -abs(selected_data));
	if (dpll(formula_copy, res)) return true;			//��֧1
	else {		//��֧2
		destoryFormula(formula_copy);
		res[abs(selected_data)] = -abs(selected_data);
		removeClauseHasLiteral(formula, -abs(selected_data));
		removeLiteralFromFormula(formula, abs(selected_data));
		return dpll(formula, res);
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
