/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf���ߺ����Ż����ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"../../naive_dpll/data_structure/data_struct.h"


/**
@brief: ��ʼ��һ��������
@return: ��ʼ����ļ�����
*/
int* initCounter()
{
	int* counter = (int*)malloc(sizeof(int) * ( 2 * info.num_literal + 1));
	memset(counter, 0, sizeof(int) * ( 2 * info.num_literal + 1));
	return counter;
}


/**
@brief: ���Ƽ�����
@param counter: �����Ƶļ�����
@return: ���ƺ�ļ�����
*/
int* copyCounter(int* counter)
{
	int* copy_counter = initCounter();
	for (int i = 0; i < 2 * info.num_literal + 1; i++)
		copy_counter[i] = counter[i];
	return copy_counter;
}


/**
@brief: ���������ת��Ϊ�������ж�Ӧλ�õ��±�
@param data: ��ת�����������
@return: ��ת�������ڼ������е�λ���±�
*/
short counterIndex(short data)
{
	return data > 0 ? (2 * data) : (2 * abs(data) -1);
}


/**
@brief: ���������е��±�ת��Ϊ��Ӧ���������
@param index: ��ת���ļ��������±�
@return: �������±��Ӧ���������
*/
short counterData(int index)
{
	return (index % 2) ? (-(index + 1) / 2) : (index / 2);
}


/**
@brief: ɾ���Ӿ��е��ض�����
@param clause: ָ���Ӿ��ָ��
@param data: ��Ҫ��ɾ�������ֵ����
@param counter: ������
*/
void deleteLiteralOpti(struct Clause* clause, short data, int* counter)
{
	struct Literal* curr = clause->head->nextLiteral;
	struct Literal* target = curr;
	while (!curr->isTail) {
		if (curr->data == data) {		//��λ��Ҫɾ��������
			target = curr;
			target->beforeLiteral->nextLiteral = target->nextLiteral;		//ɾ����Ӧ����
			target->nextLiteral->beforeLiteral = target->beforeLiteral;
			curr = curr->nextLiteral;
			free(target);
			clause->len--;
			counter[counterIndex(data)] --;		//�������ж�Ӧ��������-1
		}
		else curr = curr->nextLiteral;
	}
}


/**
@brief: ɾ��һ���Ӿ�
@param clause: ��ɾ�����Ӿ��ָ��
@param counter: ������
*/
void destroyClauseOpti(struct Clause* clause, int* counter)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {		//����ɾ������
		if (!curr->isTail) counter[counterIndex(curr->data)] --;		//��Ӧ��������-1
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
}


/**
@brief: ɾ����ʽ
@param formula: ָ��ɾ���Ĺ�ʽ��ָ��
@param counter: ������
*/
void destoryFormulaOpti(struct Formula* formula, int* counter)
{
	struct Clause* curr = formula->tail;
	struct Clause* prev = curr->beforeClause;
	while (prev != NULL) {		//����ɾ���Ӿ伯
		destroyClauseOpti(curr, counter);
		curr = prev;
		prev = prev->beforeClause;
	}
	formula->num_clause = 0;
	destroyClauseOpti(formula->head, counter);
}


/**
@brief: �ӹ�ʽ��ɾ���������ض�������ȵ�����
@param formula: ָ��ʽ��ָ��
@param data: ��ɾ�����������
@param counter: ������
@return: ��ɾ�����������
*/
int removeLiteralFromFormulaOpti(struct Formula* formula, short data, int* counter)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//����ɾ��
		deleteLiteralOpti(curr, data, counter);
		curr = curr->nextClause;
	}
	return data;
}


/**
@brief: �ӹ�ʽ��ɾ�����а����ض����ֵ��Ӿ�
@param formula: ָ��ʽ��ָ��
@param data: ��ɾ�����ض�����
@param counter: ������
@return: ��ɾ�����������
*/
int removeClauseHasLiteralOpti(struct Formula* formula, short data, int* counter)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//����ɾ��
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
@brief: �ӵ��Ӿ���ѡ������(ѡ�����Ӿ����������ֳ��ִ���֮����������)
@param formula: ָ��ʽ��ָ��
@param counter: ������
@return: ��ѡ�������ֵ����
*/
int selectDataFromUnitClauseOpti(struct Formula* formula, int* counter)
{
	int max_freq = 0, selected_data =0, freq = 0;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//������ֱ���ҵ�һ�����Ӿ�
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
@brief: ѡȡ��̵��Ӿ�
@param formula: ָ��ʽ��ָ��
@return: ָ������Ӿ��ָ��
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
@brief: ѡȡ�Ӿ����������ֳ��ִ���֮����������
@param clause: ָ���Ӿ��ָ��
@param counter: ������
@return: ��ѡȡ���������
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
@brief: ��������Ӿ�Ƶ��������ѡȡ����
@param formula: ָ��ʽ��ָ��
@param counter: ������
@return: ��ѡȡ���������
*/
int selectData(struct Formula* formula, int* counter)
{
	struct Clause* shortest_clause = selectShortestClause(formula);
	return selectMaxFreqLiteralData(shortest_clause, counter);
}