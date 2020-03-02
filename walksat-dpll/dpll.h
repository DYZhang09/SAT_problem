/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: dpll�㷨��������	                          
/**********************************************************/

#pragma once
#include"parser.h"
#include"data_struct.h"
#include"tools.h"
#include"../naive_dpll/data_structure/data_struct.h"


/**
@brief: ����ѡȡ���ֽ��и�ֵ������ʽ
@param formula: ָ��ʽ��ָ��
@param res: �������ֵĸ�ֵ����
@param data: ѡȡ������
@param mask: ָ��mask��ָ��
@param level: �������ڲ���
@param counter: ������
*/
void simplify(struct BinVector* formula, int*res, int data, struct Mask* mask, int level, int* counter)
{
	res[abs(data)] = data;
	rmClausesHasLit(formula, data, mask, level, counter);
	rmLitFromFormula(formula, -data, mask, level, counter);
}


/**
@brief: �㷨������
@param formula: ָ��ʽ��ָ��
@param mask: ָ��mask��ָ��
@param res: ����������ָ�ֵ������
@param level: ���߲���
@param counter: ������
@return: ��ʽ�����㷵��true
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
@brief: �����㷨�ӿ�
@param formula: ָ��ʽ��ָ��
@param mask: ָ��mask��ָ��
@param counter: ������
*/
struct Result WALKSAT_DPLL(struct BinVector* formula, struct Mask* mask, int *counter)
{
	struct Result result;
	result.isSatisfied = false;		//��ʼ�����
	result.res = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(int) * (info.num_literal + 1));

	result.isSatisfied = walksat_dpll(formula, mask, result.res, 0, counter);			//�������
	return result;
}