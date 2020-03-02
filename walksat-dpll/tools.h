#pragma once
#include"data_struct.h"


int dataToIndex(int data)
{
	return data > 0 ? (2 * data) : (2 * abs(data) - 1);
}


int indexToData(int index)
{
	return (index % 2) ? (-(index + 1) / 2) : (index / 2);
}


bool isLitDeleted(struct Mask mask, int n, int m)
{
	return binVecGrid(mask.lit_masks, n, m);
}


bool isClauseDeleted(struct Mask mask, int n)
{
	return vecNth(mask.clause_masks, n);
}


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


void destoryCla(struct BinVector formula, int rank, struct Mask* mask, int* counter)
{
	vecNth_re(&(mask->clause_masks), rank, 1);
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		counter[dataToIndex(binVecGrid(formula, rank, i))]--;
	}
}


bool isClauseHasData(struct Vector* clause, int rank, int data, struct Mask mask)
{
	if (isClauseDeleted(mask, rank)) return false;
	for (int i = 0; i < vecSize(*clause); i++)
		if (!isLitDeleted(mask, rank, i) and vecNth(*clause, i) == data) return true;
	return false;
}


int rmLitFromFormula(struct BinVector* formula, int data, struct Mask* mask, int level, int* counter)
{
	for (int rank = 0; rank < binVecSize(*formula); rank++) {
			deleteLit(&(binVecNth(*formula, rank)), rank, data, mask, counter);
	}
	mask->del_lit_log[abs(data)] = level;
	return data;
}


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


int clauseLen(int rank, struct Mask mask)
{
	if (vecNth(mask.clause_masks, rank)) return 0;
	int del_cnt = 0, clause_size = mask.lit_masks._elem[rank]._size;
	for (int i = 0; i < clause_size; i++) {
		del_cnt += binVecGrid(mask.lit_masks, rank, i);
	}
	return clause_size - del_cnt;
}


bool isClauseUnit(int rank, struct Mask mask)
{
	return clauseLen(rank, mask) == 1;
}


bool isClauseVoid(int rank, struct Mask mask)
{
	if (vecNth(mask.clause_masks, rank)) return false;
	int cnt = 0, clause_size = mask.lit_masks._elem[rank]._size;
	for (int i = 0; i < clause_size; i++) {
		cnt += binVecGrid(mask.lit_masks, rank, i);
	}
	return (cnt == clause_size) ? true : false;
}


bool isFormulaHasVoid(struct Mask mask)
{
	for (int rank = 0; rank < vecSize(mask.clause_masks); rank++)
		if (isClauseVoid(rank, mask)) {
			return true;
		}
	return false;
}


bool isFormulaVoid(struct Mask mask)
{
	for (int rank = 0; rank < vecSize(mask.clause_masks); rank++)
		if (!isClauseDeleted(mask, rank)) return false;
	return true;
}


int chooseFIrstData(struct BinVector formula, struct Mask mask)
{
	int rank = 0, i = 0;
	while (isClauseDeleted(mask, rank)) rank++;
	while (isLitDeleted(mask, rank, i)) i++;
	return binVecGrid(formula, rank, i);
}


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


bool evalFormula(struct BinVector formula, int* var, struct Mask mask)
{
	for (int rank = 0; rank < binVecSize(formula); rank++) {
		if (!isClauseDeleted(mask, rank)) {
			if (!evalClause(binVecNth(formula, rank), rank, var, mask)) return false;
		}
	}
	return true;
}


void arrPrint(int* arr, int size = info.num_clause)
{
	for (int i = 0; i < size; i++) {
		printf("%d ", arr[i]);
		if ((i+1) % 10 == 0) printf("\n");
	}
	printf("\n");
}


void recoverClause(struct BinVector formula, int rank, struct Mask* mask, int* counter)
{
	vecNth_re(&(mask->clause_masks), rank, 0);
	for (int i = 0; i < vecSize(binVecNth(formula, rank)); i++) {
		counter[dataToIndex(binVecGrid(formula, rank, i))]++;
	}
}

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
	freeVec(&del_cla);
	freeVec(&del_lit);
}


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


int chooseData(struct BinVector formula, struct Mask mask, int *counter)
{
	int cl = shortestClause(mask);
	return chooseMaxFreqDataInClause(formula, cl, mask, counter);
}