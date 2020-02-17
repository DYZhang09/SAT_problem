#pragma once
#include"../naive_implementation/data_structure/data_struct.h"


//�������ṹ��
struct Counter
{
	int positive;
	int negative;
};


/**
@brief: ��ʼ��һ��������
@return: ��ʼ����ļ���������
*/
struct Counter* initCounter()
{
	struct Counter* counter = (struct Counter*)malloc(sizeof(struct Counter) * (info.num_literal + 1));
	memset(counter, 0, sizeof(struct Counter) * (info.num_literal + 1));
	return counter;
}


/**
@brief: �Ӽ�������ѡȡ���ִ��������������
@param counter: ����������
@return: ���ִ��������������
*/
int argmaxFromCounter(struct Counter* counter)
{
	int i = 1, max_count = -1, argmax = 0;
	for (; i <= info.num_literal; i++) {
		if (counter[i].positive > max_count) {
			max_count = counter[i].positive;
			argmax = i;
		}
		if (counter[i].negative > max_count) {
			max_count = counter[i].negative;
			argmax = -i;
		}
	}
	counter[abs(argmax)].positive = 0;
	counter[abs(argmax)].negative = 0;
	return argmax;
}


struct Counter* copyCounter(struct Counter* counter)
{
	struct Counter* counter_copy = initCounter();
	for (int i = 0; i <= info.num_literal; i++)
		counter_copy[i] = counter[i];
	return counter_copy;
}

