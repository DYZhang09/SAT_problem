#pragma once
#include"display.h"
#include"../opti_memo_dpll/walksat.h"



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
		free(result.res);			//ÊÍ·Å¿Õ¼ä
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