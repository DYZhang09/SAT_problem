/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: 相关工具函数定义头文件                   
/**********************************************************/

#pragma once
#include"../data_struct/data_struct.h"


/**
@brief: 将文字序号转化为计数器中对应位置的下标
@param data: 被转化的文字序号
@return: 被转化文字在计数器中的位置下标
*/
int dataToIndex(int data)
{
	return data > 0 ? (2 * data) : (2 * abs(data) - 1);
}


/**
@brief: 将计数器中的下标转化为对应的文字序号
@param index: 被转化的计数器的下标
@return: 计数器下标对应的文字序号
*/
int indexToData(int index)
{
	return (index % 2) ? (-(index + 1) / 2) : (index / 2);
}


/**
@brief: 判断当前文字是否被删除
@param mask: mask
@param n: 文字在formula中对应的第一维下标
@param m: 文字在formula中对应的第二维下标
@return: 如果被删除返回true
*/
bool isLitDeleted(struct Mask mask, int n, int m)
{
	return binVecGrid(mask.lit_masks, n, m);
}


/**
@brief: 判断当前子句是否被删除
@param mask: mask
@param n: 子句在formula中对应的下标
@return: 如果被删除返回true
*/
bool isClauseDeleted(struct Mask mask, int n)
{
	return vecNth(mask.clause_masks, n);
}


/**
@brief: 删除子句中的特定文字
@param clause: 指向被删除子句的指针
@param rank: 被删除子句在formula中的下标
@param data: 被删除的文字
@param mask: 指向mask的指针
@param counter: 计数器
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
@brief: 删除当前子句
@param formula: 指向公式的指针
@param rank: 被删除子句的下标
@param mask: 指向mask的指针
@param counter: 计数器
*/
void destoryCla(struct BinVector formula, int rank, struct Mask* mask, int* counter)
{
	vecNth_re(&(mask->clause_masks), rank, 1);
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		counter[dataToIndex(binVecGrid(formula, rank, i))]--;
	}
}


/**
@brief: 判断子句是否含有特定文字
@param clause: 指向子句的指针
@param rank: 子句对应下标
@param data: 特定文字
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
@brief: 从公式中删除所有特定文字
@param formula: 指向公式的指针
@param data: 被删除的文字序号
@param mask: 指向mask的指针
@param level: 决策层数
@param counter: 计数器
@return: 被删除的文字序号
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
@brief: 删除包含特定文字的子句
@param formula: 指向公式的指针
@param data: 被删除的文字序号
@param mask: 指向mask的指针
@param level: 决策层数
@param counter: 计数器
@return: 被删除的文字序号
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
@brief: 获取子句长度
@param rank: 子句在formula中的下标
@param mask: mask
@return: 子句长度
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
@brief: 判断子句是否为单子句
@param rank: 子句在formula中的下标
@param mask: mask
@return: 为单子句返回true
*/
bool isClauseUnit(int rank, struct Mask mask)
{
	return clauseLen(rank, mask) == 1;
}


/**
@brief: 判断子句是否为空子句
@param rank: 子句在formula中的下标
@param mask: mask
@return: 为空子句返回true
*/
bool isClauseVoid(int rank, struct Mask mask)
{
	if (vecNth(mask.clause_masks, rank)) return false;
	return clauseLen(rank, mask) == 0;
}


/**
@brief: 判断公式中是否含有空子句
@param mask: mask
@return: 若含有空子句返回true
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
@brief: 判断公式是否为空
@param mask: mask
@return: 若公式为空返回true
*/
bool isFormulaVoid(struct Mask mask)
{
	for (int rank = 0; rank < vecSize(mask.clause_masks); rank++)
		if (!isClauseDeleted(mask, rank)) return false;
	return true;
}


/**
@brief: 选择单子句中的文字
@param formula: 公式
@param mask: mask
@return: 被选取的文字
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
@brief: 根据赋值结果判断当前子句是否满足
@param clause: 子句
@param rank: 子句在formula中的下标
@param var: 变量赋值数组
@param mask: mask
@return: 当前子句满足则返回true
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
@brief: 判断公式是否可满足
@param formula: 公式
@param var: 变量赋值数组
@param mask: mask
@return: 若公式可满足则返回true
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
@brief: 恢复被删除的子句
@param formula: 公式
@param rank: 子句对应的下标
@param mask: 指向mask的指针
@param counter: 计数器
*/
void recoverClause(struct BinVector formula, int rank, struct Mask* mask, int* counter)
{
	vecNth_re(&(mask->clause_masks), rank, 0);
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		counter[dataToIndex(binVecGrid(formula, rank, i))]++;
	}
}


/**
@brief: 根据决策层数恢复公式
@param formula: 公式
@param mask: 指向mask的指针
@param level: 决策层数
@param counter: 计数器
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
@brief: 找出最短的子句
@param mask: mask
@return: 最短子句对应的下标
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
@brief: 选取正负文字出现次数之和最大的文字
@param formula: 公式
@param rank: 最短子句对应的下标
@param mask: mask
@param counter: 计数器
@return: 被选取的文字
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
@brief: 根据最短子句文字频率最大策略选取文字
@param formula: 公式
@param mask: mask
@param counter: 计数器
*/
int chooseData(struct BinVector formula, struct Mask mask, int *counter)
{
	int cl = shortestClause(mask);
	return chooseMaxFreqDataInClause(formula, cl, mask, counter);
}