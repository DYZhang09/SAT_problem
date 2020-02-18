#pragma once
#include"display.h"

void testCnfSolverOpti()
{
	char* cnf_filename = getCnfFileName();
	int* counter = initCounter();
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);

	float start = clock();		//ʱ�����
	struct Result result = DPLLOpti(formula, counter);		//����DPLL�������CNF��ʽ
	float finish = clock();	//ʱ���յ�
	result.time = finish - start;		//���ʱ��

	char* suffix = strstr(cnf_filename, ".cnf");
	int suffix_len = strlen(suffix);
	for (int i = 0; i < suffix_len; i++)
		if (i == 0) suffix[i] = '.';
		else if (i == 1) suffix[i] = 'r';
		else if (i == 2) suffix[i] = 'e';
		else if (i == 3) suffix[i] = 's';
		else suffix[i] = '\0';
	cnfResultPrint(cnf_filename, result);
	free(result.res);			//�ͷſռ�
	free(cnf_filename);
	free(counter);
	destoryFormula(formula);
}

void testLoadFileOpti()
{
	char* cnf_filename = getCnfFileName();
	int* counter = NULL;
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);

	for (int i = 0; i < info.num_literal+1 ; i++)
		printf("%d ", counter[i]);
}