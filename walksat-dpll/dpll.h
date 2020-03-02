/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: dpll算法函数定义	                          
/**********************************************************/

#pragma once
#include"parser.h"
#include"data_struct.h"
#include"tools.h"
#include"../naive_dpll/data_structure/data_struct.h"


/**
@brief: 根据选取文字进行赋值并化简公式
@param formula: 指向公式的指针
@param res: 所有文字的赋值数组
@param data: 选取的文字
@param mask: 指向mask的指针
@param level: 决策所在层数
@param counter: 计数器
*/
void simplify(struct BinVector* formula, int*res, int data, struct Mask* mask, int level, int* counter)
{
	res[abs(data)] = data;
	rmClausesHasLit(formula, data, mask, level, counter);
	rmLitFromFormula(formula, -data, mask, level, counter);
}


/**
@brief: 算法主程序
@param formula: 指向公式的指针
@param mask: 指向mask的指针
@param res: 存放所有文字赋值的数组
@param level: 决策层数
@param counter: 计数器
@return: 公式可满足返回true
*/
bool walksat_dpll(struct BinVector* formula, struct Mask* mask, int* res, int level, int* counter)
{
	int selected_data = 0;
	while ((selected_data = chooseUnitData(*formula, *mask))) {
		simplify(formula, res, selected_data, mask, level, counter);
		if (isFormulaVoid(*mask)) return true;
		if (isFormulaHasVoid(*mask)) {
			recoverFormula(*formula, mask, level, counter);
			return false;
		}
	}
	if (isFormulaVoid(*mask)) return true;
	if (isFormulaHasVoid(*mask)) return false;
	
	//selected_data = chooseFIrstData(*formula, *mask);
	selected_data = chooseData(*formula, *mask, counter);
	simplify(formula, res, selected_data, mask, level + 1, counter);
	if (walksat_dpll(formula, mask, res, level+1, counter)) return true;
	else {
		simplify(formula, res, -selected_data, mask, level + 1, counter);
		if(walksat_dpll(formula, mask, res, level+1, counter)) return true;
		else {
			recoverFormula(*formula, mask, level, counter);
			return false;
		}
	}
}


/**
@brief: 调用算法接口
@param formula: 指向公式的指针
@param mask: 指向mask的指针
@param counter: 计数器
*/
struct Result WALKSAT_DPLL(struct BinVector* formula, struct Mask* mask, int *counter)
{
	struct Result result;
	result.isSatisfied = false;		//初始化结果
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = walksat_dpll(formula, mask, result.res, 0, counter);			//进行求解
	return result;
}