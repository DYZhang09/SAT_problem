/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: 核心DPLL模块头文件                              **/
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
@brief: DPLL算法主程序
@param formula: 待求解的公式
@return: 可满足则返回true, 否则返回false
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

	selected_data = selectFirstData(formula);
	res[abs(selected_data)] = 1;
	if (dpll(formula, res)) return true;
	else {
		res[abs(selected_data)] = -1;
		return dpll(formula, res);
	}
}


/**
@brief: 调用DPLL算法程序入口
@param formula: 指向公式的指针
@calls: dpll()
@return: 结构体Result
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