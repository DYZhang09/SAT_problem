#pragma once
#include"parser.h"
#include"data_struct.h"
#include"tools.h"
#include"../naive_implementation/data_structure/data_struct.h"



void simplify(struct BinVector* formula, int*res, int data, struct Mask* mask, int level)
{
	res[abs(data)] = data;
	rmClausesHasLit(formula, data, mask, level);
	rmLitFromFormula(formula, -data, mask, level);
}


bool walksat_dpll(struct BinVector* formula, struct Mask* mask, int* res, int level)
{
	int selected_data = 0;
	while ((selected_data = chooseUnitData(*formula, *mask))) {
		simplify(formula, res, selected_data, mask, level);
		if (isFormulaVoid(*mask)) return true;
		if (isFormulaHasVoid(*mask)) {
			recoverFormula(*formula, mask, level);
			return false;
		}
	}
	if (isFormulaVoid(*mask)) return true;
	if (isFormulaHasVoid(*mask)) return false;
	
	selected_data = chooseFIrstData(*formula, *mask);
	simplify(formula, res, selected_data, mask, level + 1);
	if (walksat_dpll(formula, mask, res, level+1)) return true;
	else {
		simplify(formula, res, -selected_data, mask, level + 1);
		if(walksat_dpll(formula, mask, res, level+1)) return true;
		else {
			recoverFormula(*formula, mask, level);
			return false;
		}
	}
}


struct Result WALKSAT_DPLL(struct BinVector* formula, struct Mask* mask)
{
	struct Result result;
	result.isSatisfied = false;		//初始化结果
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = walksat_dpll(formula, mask, result.res, 0);			//进行求解
	return result;
}