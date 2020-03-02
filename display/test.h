#pragma once
#include"display.h"
#include"../walksat-dpll/walksat.h"

void testCnfSolverOpti()
{
	char* cnf_filename = getCnfFileName();
	int* counter = initCounter();
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);

	float start = clock();		//时间起点
	struct Result result = DPLLOpti(formula, counter);		//调用DPLL函数求解CNF公式
	float finish = clock();	//时间终点
	result.time = finish - start;		//求解时间

	char* suffix = strstr(cnf_filename, ".cnf");
	int suffix_len = strlen(suffix);
	for (int i = 0; i < suffix_len; i++)
		if (i == 0) suffix[i] = '.';
		else if (i == 1) suffix[i] = 'r';
		else if (i == 2) suffix[i] = 'e';
		else if (i == 3) suffix[i] = 's';
		else suffix[i] = '\0';
	cnfResultPrint(cnf_filename, result);
	free(result.res);			//释放空间
	free(cnf_filename);
	free(counter);
	destoryFormula(formula);
}


void test_WALKSAT_DPLL()
{
	char* filename = getCnfFileName();
	struct BinVector formula = binVecInit();
	struct Mask mask = maskInit();
	int* counter;
	if (!loadFile(filename, &formula, &mask, &counter)) return;
	else {
		float time_start = clock();
		struct Result result= WALKSAT_DPLL(&formula, &mask, counter);
		float time_end = clock();
		result.time = time_end - time_start;

		char* suffix = strstr(filename, ".cnf");
		int suffix_len = strlen(suffix);
		for (int i = 0; i < suffix_len; i++)
			if (i == 0) suffix[i] = '.';
			else if (i == 1) suffix[i] = 'r';
			else if (i == 2) suffix[i] = 'e';
			else if (i == 3) suffix[i] = 's';
			else suffix[i] = '\0';
		cnfResultPrint(filename, result);
		free(result.res);			//释放空间
		free(filename);
		freeBinVec(&formula);
	}
}


void test_WALKSAT()
{
	char* filename = getCnfFileName();
	if (!load_formula(filename, &formula, &vars)) return;
	else {
		float time_start = clock();
		bool result = WALKSAT();
		float time_end = clock();
		printf("%d\n", result);
		vecPrint(vars);
		printf("time: %.2f\n", time_end - time_start);
		free(filename);
		freeBinVec(&formula);
		freeVec(&vars);
	}
}