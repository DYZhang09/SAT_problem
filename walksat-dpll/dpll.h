#pragma once
#include"parser.h"
#include"data_struct.h"
#include"tools.h"
#include"../naive_implementation/data_structure/data_struct.h"



void simplify(struct BinVector* formula, int*res, int data, struct Mask* mask, int level, int* counter)
{
	res[abs(data)] = data;
	rmClausesHasLit(formula, data, mask, level, counter);
	rmLitFromFormula(formula, -data, mask, level, counter);
}


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


struct Result WALKSAT_DPLL(struct BinVector* formula, struct Mask* mask, int *counter)
{
	struct Result result;
	result.isSatisfied = false;		//初始化结果
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = walksat_dpll(formula, mask, result.res, 0, counter);			//进行求解
	return result;
}