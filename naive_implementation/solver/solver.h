/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: ����DPLLģ��ͷ�ļ�                              **/
/**********************************************************/

#pragma once
#include"naive_implementation/data_structure/tool_functions.h"
#include"naive_implementation/cnfparser/cnfparser.h"

struct Result
{
	bool isSatisfied;
	int* res;
	float time;
};


/**
@brief: DPLL�㷨������
@param formula: �����Ĺ�ʽ
@return: �������򷵻�true, ���򷵻�false
*/
bool dpll(struct Formula* formula, int *res)
{
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {
		res[abs(data_unit_clause)] = (data_unit_clause > 0 ? abs(data_unit_clause) : -abs(data_unit_clause));
		removeClauseHasLiteral(formula, data_unit_clause);
		removeLiteralFromFormula(formula, -data_unit_clause);
		if (formula->num_clause == 0) return true;
		else if (hasVoidClause(formula)) return false;
	}
	if (formula->num_clause == 0) return true;

	struct Formula* formula_copy = copyFormula(formula);
	selected_data = selectFirstData(formula);
	//selected_data = selectData(formula);
	int num = formula->num_clause;
	res[abs(selected_data)] = abs(selected_data);
	removeClauseHasLiteral(formula_copy, abs(selected_data));
	removeLiteralFromFormula(formula_copy, -abs(selected_data));
	if (dpll(formula_copy, res)) return true;
	else {
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
	result.isSatisfied = false;
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = dpll(formula, result.res);
	return result;
}
