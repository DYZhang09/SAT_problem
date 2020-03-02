/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/20                                                     
//* Description: 核心DPLL模块头文件                              
/**********************************************************/

#pragma once
#include"./tool_functions.h"
#include "../cnfparser/cnfparser.h"


bool dpll(struct Formula*, int*);

/**
@brief: dpll分支函数
@param formula: 指向公式的指针
@param res: 存放变量赋值的数组
@param data: 根据策略选择的文字序号
@return: 当前分支是否可使公式满足
*/
bool branch(struct Formula* formula, int* res, int data)
{
	applySelData(formula, res, data);
	return dpll(formula, res);
}


/**
@brief: DPLL算法主程序
@param formula: 待求解的公式
@return: 可满足则返回true, 否则返回false
*/
bool dpll(struct Formula* formula, int *res)
{
	if (formula->num_clause == 0) return true;
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {				//选取单子句进行单子句传播
		applySelData(formula, res, data_unit_clause);
		if (formula->num_clause == 0) return true;		//公式全空说明公式可满足
		else if (hasVoidClause(formula)) return false;		//有空子句说明公式不可满足
	}
	struct Formula* formula_copy = copyFormula(formula);		//复制公式
	selected_data = selectFirstData(formula);			//选取第一个元素
	if (branch(formula_copy, res, selected_data)) return true;			//分支1
	else {		//分支2
		destoryFormula(formula_copy);
		return branch(formula, res, -selected_data);		//分支2
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
