/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf工具函数优化相关头文件		                          
/**********************************************************/

#pragma once
#include"../naive_implementation/data_structure/data_struct.h"


int* initCounter()
{
	int* counter = (int*)malloc(sizeof(int) * ( 2 * info.num_literal + 1));
	memset(counter, 0, sizeof(int) * ( 2 * info.num_literal + 1));
	return counter;
}


int* copyCounter(int* counter)
{
	int* copy_counter = initCounter();
	for (int i = 0; i < 2 * info.num_literal + 1; i++)
		copy_counter[i] = counter[i];
	return copy_counter;
}


int counterIndex(int data)
{
	return data > 0 ? (2 * data) : (2 * abs(data) -1);
}


int counterData(int index)
{
	return (index % 2) ? (-(index + 1) / 2) : (index / 2);
}


void deleteLiteralOpti(struct Clause* clause, int data, int* counter)
{
	struct Literal* curr = clause->head->nextLiteral;
	struct Literal* target = curr;
	while (!curr->isTail) {
		if (curr->data == data) {		//定位需要删除的文字
			target = curr;
			target->beforeLiteral->nextLiteral = target->nextLiteral;		//删除对应文字
			target->nextLiteral->beforeLiteral = target->beforeLiteral;
			curr = curr->nextLiteral;
			free(target);
			clause->len--;
			counter[counterIndex(data)] --;
		}
		else curr = curr->nextLiteral;
	}
}


void destroyClauseOpti(struct Clause* clause, int* counter)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {		//遍历删除文字
		if (!curr->isTail) counter[counterIndex(curr->data)] --;
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
}


void destoryFormulaOpti(struct Formula* formula, int* counter)
{
	struct Clause* curr = formula->tail;
	struct Clause* prev = curr->beforeClause;
	while (prev != NULL) {		//遍历删除子句集
		destroyClauseOpti(curr, counter);
		curr = prev;
		prev = prev->beforeClause;
	}
	formula->num_clause = 0;
	destroyClauseOpti(formula->head, counter);
}


int removeLiteralFromFormulaOpti(struct Formula* formula, int data, int* counter)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		deleteLiteralOpti(curr, data, counter);
		curr = curr->nextClause;
	}
	return data;
}


int removeClauseHasLiteralOpti(struct Formula* formula, int data, int* counter)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			target = curr;
			curr = curr->nextClause;
			//free(target);
			destroyClauseOpti(target, counter);
			formula->num_clause--;
		}
		else curr = curr->nextClause;
	}
	return data;
}


int selectDataFromUnitClauseOpti(struct Formula* formula, int* counter)
{
	int max_freq = 0, selected_data =0;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历，直到找到一个单子句
		if (isUnitClause(curr)) {
			if (counter[counterIndex(curr->head->nextLiteral->data)] + 
				counter[counterIndex(-(curr->head->nextLiteral->data ))]> max_freq) {
				selected_data = curr->head->nextLiteral->data;
				max_freq = counter[counterIndex(curr->head->nextLiteral->data)] +
									counter[counterIndex(-(curr->head->nextLiteral->data))];
			}
			return selected_data;
		}
		curr = curr->nextClause;
	}
	return selected_data;
}


int selectPureData(int* counter)
{
	for (int i = 1; i < info.num_literal + 1; i++) {
		if (counter[counterIndex(i)] == 0 and counter[counterIndex(-i)] > 0) return -i;
		if (counter[counterIndex(i)] > 0 and counter[counterIndex(-i)] == 0) return i;
	}
	return 0;
}



struct Clause* selectShortestClause(struct Formula* formula)
{
	static struct Clause* shortest = NULL;
	struct Clause* curr = formula->head->nextClause;
	int min_len = INT_MAX;
	while (!curr->isLastClause) {
		if (curr->len < min_len) {
			shortest = curr;
			min_len = shortest->len;
		}
		curr = curr->nextClause;
	}
	return shortest;
}


int selectMaxFreqLiteralData(struct Clause* clause, int* counter)
{
	struct Literal* curr = clause->head->nextLiteral;
	int max_freq = 0, selected_data = 0;
	while (!curr->isTail) {
		if (counter[counterIndex(curr->data)] + counter[counterIndex(-(curr->data))] > max_freq) {
			selected_data = curr->data;
			max_freq = counter[counterIndex(curr->data)] + counter[counterIndex(-(curr->data))];
		}
		curr = curr->nextLiteral;
	}
	return selected_data;
}


int selectData(struct Formula* formula, int* counter)
{
	struct Clause* shortest_clause = selectShortestClause(formula);
	return selectMaxFreqLiteralData(shortest_clause, counter);
}


int selectData(int* counter)
{
	int max = 0, selected = 0;
	for (int i = 1; i < 2 * info.num_literal + 1; i++) {
		if (counter[i] > max) {
			max = counter[i];
			selected = i;
		}
	}
	return counterData(selected);
}


int randomSelectData(int* counter)
{
	int data = 0;
	do {
		data = rand() % (info.num_literal + 1);
		data = rand() % 2 ? data : -data;
	} while (data == 0 or counter[counterIndex(data)] == 0);
	printf("random\n");
	return data;
}


void unitClauseRuleOpti(struct Formula* formula, int data, int* counter)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {			//一次遍历完成单子句传播规则
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			target = curr;
			curr = curr->nextClause;
			//free(target);
			destroyClauseOpti(target, counter);
			formula->num_clause--;
		}
		else {
			deleteLiteralOpti(curr, -data, counter);
			curr = curr->nextClause;
		}
	}
}