#pragma once
#include"../cnfparser/cnfparser.h"
#include"../tool_function.h"


bool dpllOpti(struct Formula* formula, int* res, int* counter)
{
	int data_unit_clause = 0, selected_data = 0;
	while ((data_unit_clause = selectDataFromUnitClause(formula))) {				//ѡȡ���Ӿ���е��Ӿ䴫��
		res[abs(data_unit_clause)] = (data_unit_clause > 0 ? abs(data_unit_clause) : -abs(data_unit_clause));		//��ֵ
		/*removeClauseHasLiteral(formula, data_unit_clause);
		removeLiteralFromFormula(formula, -data_unit_clause);*/
		unitClauseRule(formula, data_unit_clause);
		if (formula->num_clause == 0) return true;		//��ʽȫ��˵����ʽ������
		else if (hasVoidClause(formula)) return false;		//�п��Ӿ�˵����ʽ��������
	}
	if (formula->num_clause == 0) return true;

	struct Formula* formula_copy = copyFormula(formula);		//���ƹ�ʽ
	int* counter_copy = copyCounter(counter);
	selected_data = selectData(formula, counter);
	int num = formula->num_clause;
	res[abs(selected_data)] = abs(selected_data);
	removeClauseHasLiteralOpti(formula_copy, abs(selected_data), counter_copy);
	removeLiteralFromFormulaOpti(formula_copy, -abs(selected_data), counter_copy);
	counter_copy[abs(selected_data)] = 0;
	if (dpllOpti(formula_copy, res, counter_copy)) return true;			//��֧1
	else {		//��֧2
		destoryFormula(formula_copy);
		free(counter_copy);
		res[abs(selected_data)] = -abs(selected_data);
		removeClauseHasLiteralOpti(formula, -abs(selected_data), counter);
		removeLiteralFromFormulaOpti(formula, abs(selected_data), counter);
		return dpllOpti(formula, res, counter);
	}
}


struct Result DPLLOpti(struct Formula* formula, int* counter)
{
	struct Result result;
	result.isSatisfied = false;		//��ʼ�����
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = dpllOpti(formula, result.res, counter);			//�������
	return result;
}