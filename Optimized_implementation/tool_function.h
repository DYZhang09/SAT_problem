/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf工具函数优化相关头文件		                          
/**********************************************************/

#pragma once
#include"../naive_implementation/data_structure/data_struct.h"


int* initCounter()
{
	int* counter = (int*)malloc(sizeof(int) * (info.num_literal + 1));
	memset(counter, 0, sizeof(int) * (info.num_literal + 1));
	return counter;
}


int* copyCounter(int* counter)
{
	int* copy_counter = initCounter();
	for (int i = 0; i < info.num_literal + 1; i++)
		copy_counter[i] = counter[i];
	return copy_counter;
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
			counter[abs(data)] --;
		}
		else curr = curr->nextLiteral;
	}
}


void destroyClauseOpti(struct Clause* clause, int* counter)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {		//遍历删除文字
		free(curr);
		if (!curr->isTail) counter[abs(curr->data)] --;
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
		if (counter[abs(curr->data)] > max_freq) {
			selected_data = curr->data;
			max_freq = counter[abs(curr->data)];
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


int randomSelectData(int* counter)
{
	int data = 0;
	do {
		data = rand() % (info.num_literal + 1);
		data = rand() % 2 ? data : -data;
	} while (data == 0 or counter[abs(data)] == 0);
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