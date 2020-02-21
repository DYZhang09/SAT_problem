/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: ���ء���������ʾģ��                           **/
/**********************************************************/

#pragma once
#include"../solver/solver.h"
#include"../data_structure/data_struct.h"
#include"../cnfparser/cnfparser.h"



//-------------------------functions for debug-----------------------------------//
/**
@brief: ��ӡ��ʽ
@param formula: ָ��ʽ��ָ��
*/
void printFormula(struct Formula* formula)
{
	int i = 1;
	struct Clause* currClause = formula->head->nextClause;
	while (!currClause->isLastClause) {
		struct Literal* currLit = currClause->head->nextLiteral;
		printf("Formula %d:\n", i++);
		while (!currLit->isTail) {
			printf("%d ", currLit->data);
			currLit = currLit->nextLiteral;
		}
		std::cout << currClause->len << '\n';
		currClause = currClause->nextClause;
	}
}


/**
@brief: ��ӡ���������
@param res: �������
*/
void printArray(int* res)
{
	for (int i = 1; i <= info.num_literal; i++) {
		printf("%d ", res[i]);
		if (i % 10 == 0) printf("\n");
	}
}


//------------------------------------------------------------------------------------------
/**
@brief: ��ӡ�����Ϣ
*/
void printHelp()
{
	printf("/**************************************************************/\n");
	printf("/****************��ӭ����CNF��ʽ������***********************/\n");
	printf("/*********�𰸽�д���������ļ�ͬ�ļ����µ�res�ļ���************/\n");
	printf("/**************************************************************/\n");
}


/**
@brief: ��ʾ������������ȡ�û�����
@return: ������ļ�·����
*/
char* getFileName()
{
	printf("\n/*������cnf�ļ�·��:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: �����д���ļ�
@param filename: Ŀ���ļ���·��
@param result: ���
*/
void filePrint(char* filename, struct Result result)
{
	FILE* fp = fopen(filename, "w");
	fprintf(fp, "%s %d\n", "s", result.isSatisfied);
	fprintf(fp, "%s ", "v");
	for (int i = 1; i <= info.num_literal; i++) {
		fprintf(fp, "%d\t", result.res[i]);
		if (i % 15 == 0) fprintf(fp, "%c", '\n');
	}
	fprintf(fp, "%c", '\n');
	fprintf(fp, "%s %f", "t", result.time);
	printf("�����д���ļ�:%s\n", filename);
	fclose(fp);
}


/**
@brief: debugʱʹ�ú���, ���𽫽���������Ļ
@param filename: cnf�ļ�·��
@calls: loadFile(), copyFormula(), DPLL(), printArray()
*/
void debug(char* filename)
{
	float start = clock();
	struct Formula* formula = loadFile(filename);
	printFormula(formula);
	struct Formula* formula_copy = copyFormula(formula);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	printf("���:\n");
	printf("%d\n", result.isSatisfied);
	printArray(result.res);
	printf("\n%f\n", finish - start);

	printf("��֤:\n");
	struct Clause* test = formula->head->nextClause;
	while (!test->isLastClause) {
		printf("%d\n", evaluateClause(test, result.res));
		test = test->nextClause;
	}
	printf("��ʽ���:%d\n", evaluateFormula(formula, result.res));
	free(result.res);
	destoryFormula(formula);
}


/**
@brief: ͨ��DPLL�㷨���CNF��ʽ
@param filename: cnf�ļ�·��
@calls: loadFile(), copyFormula(), DPLL(), filePrint()
*/
void calculate(char* filename)
{
	float start = clock();
	struct Formula* formula = loadFile(filename);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	result.time = finish - start;

	char* path = strtok(filename, ".cnf");
	char* w_filename = strcat(path, ".res");
	filePrint(w_filename, result);
	free(result.res);
    destoryFormula(formula);
}

/**
@brief: ������
*/
void solve()
{
	char c;
//	struct Formula* formula;

	printHelp();
	do {
		char* filename = getFileName();
#ifdef DEBUG
		debug(filename);
#endif // DEBUG

#ifndef DEBUG
		calculate(filename);
#endif // !DEBUG
		free(filename);
		
		printf("��ESC�˳�, ����������\n");
		c = getchar();
	} while ((c = getch()) != 27);
	printf("���˳�\n");
}
