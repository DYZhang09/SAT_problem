/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: ����DPLLģ��ͷ�ļ�                              **/
/**********************************************************/

#pragma once
#include"data_struct.h"
#include"cnfparser.h"

struct Result
{
	bool isSatisfied;
	char* res;
};


/**
@brief: DPLL�㷨������
@param formula: �����Ĺ�ʽ
@return: �������򷵻�true, ���򷵻�false
*/
bool dpll(struct Formula* formula, char *res)
{
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {
		res[abs(data_unit_clause)] = (data_unit_clause > 0 ? 1 : -1);
		removeClauseHasLiteral(formula, data_unit_clause);
		removeLiteralFromFormula(formula, -data_unit_clause);
		if (formula->num_clause == 0) return true;
		else if (hasVoidClause(formula)) return false;
	}

	struct Formula* formula_copy = copyFormula(formula);
	selected_data = selectFirstData(formula);
	res[abs(selected_data)] = 1;
	if (selected_data > 0) {
		removeClauseHasLiteral(formula, selected_data);
		removeLiteralFromFormula(formula, -selected_data);
	}
	else {
		removeClauseHasLiteral(formula, -selected_data);
		removeLiteralFromFormula(formula, selected_data);
	}
	if (dpll(formula, res)) return true;
	else {
		res[abs(selected_data)] = -1;
		if (selected_data < 0) {
			removeClauseHasLiteral(formula_copy, selected_data);
			removeLiteralFromFormula(formula_copy, -selected_data);
		}
		else {
			removeClauseHasLiteral(formula_copy, -selected_data);
			removeLiteralFromFormula(formula_copy, selected_data);
		}
		return dpll(formula_copy, res);
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
	result.res = (char*)malloc(sizeof(char) * info.num_literal + 1);
	memset(result.res, 0, sizeof(char) * info.num_literal + 1);

	result.isSatisfied = dpll(formula, result.res);
	return result;
}