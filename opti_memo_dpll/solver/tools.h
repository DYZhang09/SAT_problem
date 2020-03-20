/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: ��ع��ߺ�������ͷ�ļ�                   
/**********************************************************/

#pragma once
#include"../data_struct/data_struct.h"


/**
@brief: ���������ת��Ϊ�������ж�Ӧλ�õ��±�
@param data: ��ת�����������
@return: ��ת�������ڼ������е�λ���±�
*/
int dataToIndex(int data)
{
	return data > 0 ? (2 * data) : (2 * abs(data) - 1);
}


/**
@brief: ���������е��±�ת��Ϊ��Ӧ���������
@param index: ��ת���ļ��������±�
@return: �������±��Ӧ���������
*/
int indexToData(int index)
{
	return (index % 2) ? (-(index + 1) / 2) : (index / 2);
}


/**
@brief: �жϵ�ǰ�����Ƿ�ɾ��
@param mask: mask
@param n: ������formula�ж�Ӧ�ĵ�һά�±�
@param m: ������formula�ж�Ӧ�ĵڶ�ά�±�
@return: �����ɾ������true
*/
bool isLitDeleted(struct Mask mask, int n, int m)
{
	return binVecGrid(mask.lit_masks, n, m);
}


/**
@brief: �жϵ�ǰ�Ӿ��Ƿ�ɾ��
@param mask: mask
@param n: �Ӿ���formula�ж�Ӧ���±�
@return: �����ɾ������true
*/
bool isClauseDeleted(struct Mask mask, int n)
{
	return vecNth(mask.clause_masks, n);
}


/**
@brief: ɾ���Ӿ��е��ض�����
@param clause: ָ��ɾ���Ӿ��ָ��
@param rank: ��ɾ���Ӿ���formula�е��±�
@param data: ��ɾ��������
@param mask: ָ��mask��ָ��
@param counter: ������
*/
void deleteLit(struct Vector* clause, int rank, int data, struct Mask *mask, int* counter)
{
	if (isClauseDeleted(*mask, rank)) return;
	for (int i = 0; i < vecSize(*clause); i++) {
		if (!isLitDeleted(*mask, rank, i) and vecNth(*clause, i) == data) {
			binVecGrid_re(&(mask->lit_masks), rank, i, 1);
			counter[dataToIndex(data)]--;
		}
	}
}


/**
@brief: ɾ����ǰ�Ӿ�
@param formula: ָ��ʽ��ָ��
@param rank: ��ɾ���Ӿ���±�
@param mask: ָ��mask��ָ��
@param counter: ������
*/
void destoryCla(struct BinVector formula, int rank, struct Mask* mask, int* counter)
{
	vecNth_re(&(mask->clause_masks), rank, 1);
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		counter[dataToIndex(binVecGrid(formula, rank, i))]--;
	}
}


/**
@brief: �ж��Ӿ��Ƿ����ض�����
@param clause: ָ���Ӿ��ָ��
@param rank: �Ӿ��Ӧ�±�
@param data: �ض�����
@param mask: mask
*/
bool isClauseHasData(struct Vector* clause, int rank, int data, struct Mask mask)
{
	if (isClauseDeleted(mask, rank)) return false;
	for (int i = 0; i < vecSize(*clause); i++)
		if (!isLitDeleted(mask, rank, i) and vecNth(*clause, i) == data) return true;
	return false;
}


/**
@brief: �ӹ�ʽ��ɾ�������ض�����
@param formula: ָ��ʽ��ָ��
@param data: ��ɾ�����������
@param mask: ָ��mask��ָ��
@param level: ���߲���
@param counter: ������
@return: ��ɾ�����������
*/
int rmLitFromFormula(struct BinVector* formula, int data, struct Mask* mask, int level, int* counter)
{
	for (int rank = 0; rank < binVecSize(*formula); rank++) {
			deleteLit(&(binVecNth(*formula, rank)), rank, data, mask, counter);
	}
	mask->del_lit_log[abs(data)] = level;
	return data;
}


/**
@brief: ɾ�������ض����ֵ��Ӿ�
@param formula: ָ��ʽ��ָ��
@param data: ��ɾ�����������
@param mask: ָ��mask��ָ��
@param level: ���߲���
@param counter: ������
@return: ��ɾ�����������
*/
int rmClausesHasLit(struct BinVector* formula, int data, struct Mask *mask, int level, int* counter)
{
	for (int rank = 0; rank < binVecSize(*formula); rank++) {
		if (!isClauseDeleted(*mask, rank) and isClauseHasData(&(binVecNth(*formula, rank)), rank, data, *mask)) {
			destoryCla(*formula, rank, mask, counter);
			mask->del_clause_log[rank] = level;
		}
	}
	return data;
}


/**
@brief: ��ȡ�Ӿ䳤��
@param rank: �Ӿ���formula�е��±�
@param mask: mask
@return: �Ӿ䳤��
*/
int clauseLen(int rank, struct Mask mask)
{
	if (vecNth(mask.clause_masks, rank)) return 0;
	int del_cnt = 0, clause_size = mask.lit_masks._elem[rank]._size;
	for (int i = 0; i < clause_size; i++) {
		del_cnt += binVecGrid(mask.lit_masks, rank, i);
	}
	return clause_size - del_cnt;
}


/**
@brief: �ж��Ӿ��Ƿ�Ϊ���Ӿ�
@param rank: �Ӿ���formula�е��±�
@param mask: mask
@return: Ϊ���Ӿ䷵��true
*/
bool isClauseUnit(int rank, struct Mask mask)
{
	return clauseLen(rank, mask) == 1;
}


/**
@brief: �ж��Ӿ��Ƿ�Ϊ���Ӿ�
@param rank: �Ӿ���formula�е��±�
@param mask: mask
@return: Ϊ���Ӿ䷵��true
*/
bool isClauseVoid(int rank, struct Mask mask)
{
	if (vecNth(mask.clause_masks, rank)) return false;
	return clauseLen(rank, mask) == 0;
}


/**
@brief: �жϹ�ʽ���Ƿ��п��Ӿ�
@param mask: mask
@return: �����п��Ӿ䷵��true
*/
bool isFormulaHasVoid(struct Mask mask)
{
	for (int rank = 0; rank < vecSize(mask.clause_masks); rank++)
		if (isClauseVoid(rank, mask)) {
			return true;
		}
	return false;
}


/**
@brief: �жϹ�ʽ�Ƿ�Ϊ��
@param mask: mask
@return: ����ʽΪ�շ���true
*/
bool isFormulaVoid(struct Mask mask)
{
	for (int rank = 0; rank < vecSize(mask.clause_masks); rank++)
		if (!isClauseDeleted(mask, rank)) return false;
	return true;
}


/**
@brief: ѡ���Ӿ��е�����
@param formula: ��ʽ
@param mask: mask
@return: ��ѡȡ������
*/
int chooseUnitData(struct BinVector formula, struct Mask mask)
{
	int rank = 0, i = 0;
	while (rank < binVecSize(formula) and !isClauseUnit(rank, mask)) rank++;
	if (rank > binVecSize(formula) - 1) return 0;
	else {
		while (isLitDeleted(mask, rank, i)) i++;
		return binVecGrid(formula, rank, i);
	}
}


/**
@brief: ���ݸ�ֵ����жϵ�ǰ�Ӿ��Ƿ�����
@param clause: �Ӿ�
@param rank: �Ӿ���formula�е��±�
@param var: ������ֵ����
@param mask: mask
@return: ��ǰ�Ӿ������򷵻�true
*/
bool evalClause(struct Vector clause, int rank,  int* var, struct Mask mask)
{
	for (int i = 0; i < vecSize(clause); i++) {
		if (!isLitDeleted(mask, rank, i)) {
			int data = vecNth(clause, i);
			if (data * var[abs(data)] >= 0) return true;
		}
	}
	return false;
}


/**
@brief: �жϹ�ʽ�Ƿ������
@param formula: ��ʽ
@param var: ������ֵ����
@param mask: mask
@return: ����ʽ�������򷵻�true
*/
bool evalFormula(struct BinVector formula, int* var, struct Mask mask)
{
	for (int rank = 0; rank < binVecSize(formula); rank++) {
		if (!isClauseDeleted(mask, rank)) {
			if (!evalClause(binVecNth(formula, rank), rank, var, mask)) return false;
		}
	}
	return true;
}


/**
@brief: �ָ���ɾ�����Ӿ�
@param formula: ��ʽ
@param rank: �Ӿ��Ӧ���±�
@param mask: ָ��mask��ָ��
@param counter: ������
*/
void recoverClause(struct BinVector formula, int rank, struct Mask* mask, int* counter)
{
	vecNth_re(&(mask->clause_masks), rank, 0);
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		counter[dataToIndex(binVecGrid(formula, rank, i))]++;
	}
}


/**
@brief: ���ݾ��߲����ָ���ʽ
@param formula: ��ʽ
@param mask: ָ��mask��ָ��
@param level: ���߲���
@param counter: ������
*/
void recoverFormula(struct BinVector formula, struct Mask* mask, int level, int* counter)
{
	struct Vector del_lit = vecInit();
	struct Vector del_cla = vecInit();
	for (int rank = 0; rank < info.num_clause; rank++) {
		if (mask->del_clause_log[rank] == level) {
			recoverClause(formula, rank, mask, counter);
			vec_push_back(&del_cla, rank);
		}
		if (!isClauseDeleted(*mask, rank)) {
			for (int i = 0; i < vecSize(binVecNth(mask->lit_masks, rank)); i++) {
				if (mask->del_lit_log[abs(binVecGrid(formula, rank, i))] == level) {
					binVecGrid_re(&(mask->lit_masks), rank, i, 0);
					counter[dataToIndex(binVecGrid(formula, rank, i))]++;
					vec_push_back(&del_lit, abs(binVecGrid(formula, rank, i)));
				}
			}
		}
	}
	for (int i = 0; i < vecSize(del_lit); i++) {
		mask->del_lit_log[vecNth(del_lit, i)] = 0;
	}
	for (int i = 0; i < vecSize(del_cla); i++)
		mask->del_clause_log[vecNth(del_cla, i)] = 0;
	/*freeVec(&del_cla);
	freeVec(&del_lit);*/
}


/**
@brief: �ҳ���̵��Ӿ�
@param mask: mask
@return: ����Ӿ��Ӧ���±�
*/
int shortestClause(struct Mask mask)
{
	int min_len = INT_MAX, s_rank = 0, len = 0;
	for (int rank = 0; rank < binVecSize(mask.lit_masks); rank++) {
		if ((len = clauseLen(rank, mask)) > 0 and len < min_len) {
			min_len = len;
			s_rank = rank;
		}
	}
	return s_rank;
}


/**
@brief: ѡȡ�������ֳ��ִ���֮����������
@param formula: ��ʽ
@param rank: ����Ӿ��Ӧ���±�
@param mask: mask
@param counter: ������
@return: ��ѡȡ������
*/
int chooseMaxFreqDataInClause(struct BinVector formula, int rank, struct Mask mask, int* counter)
{
	int max_freq = 0, selected_data = 0, freq = 0;
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		if (!isLitDeleted(mask, rank, i)) {
			if ((freq = counter[dataToIndex(binVecGrid(formula, rank, i))] +
				counter[dataToIndex(-binVecGrid(formula, rank, i))]) > max_freq) {
				max_freq = freq;
				selected_data = binVecGrid(formula, rank, i);
			}
		}
	}
	return selected_data;
}


/**
@brief: ��������Ӿ�����Ƶ��������ѡȡ����
@param formula: ��ʽ
@param mask: mask
@param counter: ������
*/
int chooseData(struct BinVector formula, struct Mask mask, int *counter)
{
	int cl = shortestClause(mask);
	return chooseMaxFreqDataInClause(formula, cl, mask, counter);
}