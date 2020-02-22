/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf求解优化模块相关头文件		                          
/**********************************************************/

#pragma once
#include"../cnfparser/cnfparser.h"
#include"../tool_function.h"


void applySelectedData(struct Formula* formula, int* res, int* counter, int selected_data)
{
	res[abs(selected_data)] = selected_data;	//赋值
	removeClauseHasLiteralOpti(formula, selected_data, counter);
	removeLiteralFromFormulaOpti(formula, -selected_data, counter);
}


bool dpllOpti(struct Formula* formula, int* res, int* counter)
{
	int selected_data = 0;

	while ((selected_data = selectDataFromUnitClauseOpti(formula, counter))) {				//选取单子句进行单子句传播
		applySelectedData(formula, res, counter, selected_data);
		if (formula->num_clause == 0) return true;		//公式全空说明公式可满足
		else if (hasVoidClause(formula)) return false;		//有空子句说明公式不可满足
	}
	if (formula->num_clause == 0) return true;

	struct Formula* formula_copy = copyFormula(formula);		//复制公式
	int* counter_copy = copyCounter(counter);
	selected_data = selectData(formula, counter);
	applySelectedData(formula_copy, res, counter_copy, selected_data);

	if (dpllOpti(formula_copy, res, counter_copy)) return true;			//分支1
	else {		//分支2
		destoryFormula(formula_copy);
		free(counter_copy);
		applySelectedData(formula, res, counter, -selected_data);
		return dpllOpti(formula, res, counter);
	}
}


struct Result DPLLOpti(struct Formula* formula, int* counter)
{
	struct Result result;
	result.isSatisfied = false;		//初始化结果
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = dpllOpti(formula, result.res, counter);			//进行求解
	return result;
}