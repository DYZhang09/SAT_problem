/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf工具函数优化相关头文件		                          
/**********************************************************/

#pragma once
#include"../../naive_dpll/data_structure/data_struct.h"


/**
@brief: 初始化一个计数器
@return: 初始化后的计数器
*/
int* initCounter()
{
	int* counter = (int*)malloc(sizeof(int) * ( 2 * info.num_literal + 1));
	memset(counter, 0, sizeof(int) * ( 2 * info.num_literal + 1));
	return counter;
}


/**
@brief: 复制计数器
@param counter: 被复制的计数器
@return: 复制后的计数器
*/
int* copyCounter(int* counter)
{
	int* copy_counter = initCounter();
	for (int i = 0; i < 2 * info.num_literal + 1; i++)
		copy_counter[i] = counter[i];
	return copy_counter;
}


/**
@brief: 将文字序号转化为计数器中对应位置的下标
@param data: 被转化的文字序号
@return: 被转化文字在计数器中的位置下标
*/
short counterIndex(short data)
{
	return data > 0 ? (2 * data) : (2 * abs(data) -1);
}


/**
@brief: 将计数器中的下标转化为对应的文字序号
@param index: 被转化的计数器的下标
@return: 计数器下标对应的文字序号
*/
short counterData(int index)
{
	return (index % 2) ? (-(index + 1) / 2) : (index / 2);
}


/**
@brief: 删除子句中的特定文字
@param clause: 指向子句的指针
@param data: 需要被删除的文字的序号
@param counter: 计数器
*/
void deleteLiteralOpti(struct Clause* clause, short data, int* counter)
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
			counter[counterIndex(data)] --;		//计数器中对应文字数量-1
		}
		else curr = curr->nextLiteral;
	}
}


/**
@brief: 删除一个子句
@param clause: 被删除的子句的指针
@param counter: 计数器
*/
void destroyClauseOpti(struct Clause* clause, int* counter)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {		//遍历删除文字
		if (!curr->isTail) counter[counterIndex(curr->data)] --;		//对应文字数量-1
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
}


/**
@brief: 删除公式
@param formula: 指向被删除的公式的指针
@param counter: 计数器
*/
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


/**
@brief: 从公式中删除所有与特定文字相等的文字
@param formula: 指向公式的指针
@param data: 被删除的文字序号
@param counter: 计数器
@return: 被删除的文字序号
*/
int removeLiteralFromFormulaOpti(struct Formula* formula, short data, int* counter)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		deleteLiteralOpti(curr, data, counter);
		curr = curr->nextClause;
	}
	return data;
}


/**
@brief: 从公式中删除所有包含特定文字的子句
@param formula: 指向公式的指针
@param data: 被删除的特定文字
@param counter: 计数器
@return: 被删除的文字序号
*/
int removeClauseHasLiteralOpti(struct Formula* formula, short data, int* counter)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			target = curr;
			curr = curr->nextClause;
			destroyClauseOpti(target, counter);
			formula->num_clause--;
		}
		else curr = curr->nextClause;
	}
	return data;
}


/**
@brief: 从单子句中选出文字(选出单子句中正负文字出现次数之和最大的文字)
@param formula: 指向公式的指针
@param counter: 计数器
@return: 被选出的文字的序号
*/
int selectDataFromUnitClauseOpti(struct Formula* formula, int* counter)
{
	int max_freq = 0, selected_data =0, freq = 0;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历，直到找到一个单子句
		if (isUnitClause(curr)) {
			if ((freq = counter[counterIndex(curr->head->nextLiteral->data)] + 
				counter[counterIndex(-(curr->head->nextLiteral->data ))])> max_freq) {
				selected_data = curr->head->nextLiteral->data;
				max_freq = freq;
			}
			return selected_data;
		}
		curr = curr->nextClause;
	}
	return selected_data;
}


/**
@brief: 选取最短的子句
@param formula: 指向公式的指针
@return: 指向最短子句的指针
*/
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


/**
@brief: 选取子句中正负文字出现次数之和最大的文字
@param clause: 指向子句的指针
@param counter: 计数器
@return: 被选取的文字序号
*/
int selectMaxFreqLiteralData(struct Clause* clause, int* counter)
{
	int max_freq = 0, selected_data = 0, freq = 0;
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {
		if ((freq = counter[counterIndex(curr->data)] + counter[counterIndex(-(curr->data))]) > max_freq) {
			selected_data = curr->data;
			max_freq = freq;
		}
		curr = curr->nextLiteral;
	}
	return selected_data;
}


/**
@brief: 根据最短子句频率最大策略选取文字
@param formula: 指向公式的指针
@param counter: 计数器
@return: 被选取的文字序号
*/
int selectData(struct Formula* formula, int* counter)
{
	struct Clause* shortest_clause = selectShortestClause(formula);
	return selectMaxFreqLiteralData(shortest_clause, counter);
}