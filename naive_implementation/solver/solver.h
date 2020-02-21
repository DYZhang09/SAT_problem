/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: 核心DPLL模块头文件                              **/
/**********************************************************/

#pragma once
#include"../data_structure/tool_functions.h"
#include"../cnfparser/cnfparser.h"


/**
@brief: DPLL算法主程序
@param formula: 待求解的公式
@return: 可满足则返回true, 否则返回false
*/
bool dpll(struct Formula* formula, int *res)
{
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {				//选取单子句进行单子句传播
		res[abs(data_unit_clause)] = (data_unit_clause > 0 ? abs(data_unit_clause) : -abs(data_unit_clause));		//赋值
		/*removeClauseHasLiteral(formula, data_unit_clause);
		removeLiteralFromFormula(formula, -data_unit_clause);*/
		unitClauseRule(formula, data_unit_clause);
		if (formula->num_clause == 0) return true;		//公式全空说明公式可满足
		else if (hasVoidClause(formula)) return false;		//有空子句说明公式不可满足
	}
	if (formula->num_clause == 0) return true;

	struct Formula* formula_copy = copyFormula(formula);		//复制公式
	selected_data = selectFirstData(formula);			//选取第一个元素
	int num = formula->num_clause;
	res[abs(selected_data)] = abs(selected_data);			
	removeClauseHasLiteral(formula_copy, abs(selected_data));
	removeLiteralFromFormula(formula_copy, -abs(selected_data));
	if (dpll(formula_copy, res)) return true;			//分支1
	else {		//分支2
		destoryFormula(formula_copy);
		res[abs(selected_data)] = -abs(selected_data);
		removeClauseHasLiteral(formula, -abs(selected_data));
		removeLiteralFromFormula(formula, abs(selected_data));
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
	result.isSatisfied = false;		//初始化结果
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = dpll(formula, result.res);			//进行求解
	return result;
}
