/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf求解优化模块相关头文件		                          
/**********************************************************/

#pragma once
#include"./tool_functions.h"
#include"../cnfparser/cnfparser.h"
#include"../../display/debug.h"


bool dpllOpti(struct Formula*, short*, int*);


/**
@brief: 根据所选的文字赋值并进行传播
@param formula: 指向公式的指针
@param res: 存放所有文字赋值的数组
@param counter: 计数器
@param selected_data: 所选的文字序号
*/
void applySelectedData(struct Formula* formula, short* res, int* counter, short selected_data)
{
	res[abs(selected_data)] = selected_data;	//赋值
	removeClauseHasLiteralOpti(formula, selected_data, counter);
	removeLiteralFromFormulaOpti(formula, -selected_data, counter);
}


/**
@brief: dpll分支函数
@param formula: 指向公式的指针
@param res: 存放变量赋值的数组
@param counter: 计数器
@param data: 根据分支策略选取的文字
@return: 若当前分支公式可满足则返回true
*/
bool branchOpti(struct Formula* formula, short* res, int* counter, short data)
{
	applySelectedData(formula, res, counter, data);
	return dpllOpti(formula, res, counter);
}


/**
@brief: dpll算法主程序
@param formula: 指向公式的指针
@param res: 所有文字赋值的数组
@param counter: 计数器
@return: 若公式可满足则返回true
*/
bool dpllOpti(struct Formula* formula, short* res, int* counter)
{
	short selected_data = 0;
	while ((selected_data = selectDataFromUnitClauseOpti(formula, counter))) {				//选取单子句进行单子句传播
		applySelectedData(formula, res, counter, selected_data);
		if (formula->num_clause == 0) return true;		//公式全空说明公式可满足
		else if (hasVoidClause(formula)) return false;//有空子句说明公式不可满足
	}
	if (formula->num_clause == 0) return true;
	struct Formula* formula_copy = copyFormula(formula);		//复制公式
	int* counter_copy = copyCounter(counter);
	selected_data = selectData(formula, counter);
	if (branchOpti(formula_copy, res, counter_copy, selected_data)) return true;			//分支1
	else {		
		destoryFormula(formula_copy);
		free(counter_copy);
		return branchOpti(formula, res, counter, -selected_data);		//分支2
	}
}


/**
@brief: 调用dpll算法的接口
@param formula: 指向公式的指针
@param counter: 计数器
@return: 表示结果的Result结构体
*/
struct Result DPLLOpti(struct Formula* formula, int* counter)
{
	struct Result result = initResult();
	float time_start = clock();
	result.isSatisfied = dpllOpti(formula, result.res, counter);			//进行求解
	result.time = clock() - time_start;
	return result;
}