/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: ���ء���������ʾģ��                           **/
/**********************************************************/

#pragma once
#include"config.h"
#include"solver.h"
#include"data_struct.h"
#include"cnfparser.h"

/**
@brief: ��ʾ������������ȡ�û�����, ����DEFAULT��ʾʹ��Ĭ���ļ�
@return: ������ļ�·����, ������DEFAULT,�򷵻�Ĭ���ļ�·��
*/
char* getFileName()
{
	char* filename = (char*)malloc(sizeof(char) * 255);
	printf("/**************************************************************/\n");
	printf("/****************��ӭ����CNF��ʽ������***********************/\n");
	printf("/*********�𰸽�д���������ļ�ͬ�ļ����µ�res�ļ���************/\n");
	printf("/**************************************************************/\n");
	printf("\n/*������cnf�ļ�·��(б������˫б�ܱ�ʾ):\n");
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
	struct Formula* formula_copy = copyFormula(formula);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	printf("���:\n");
	printf("%d\n", result.isSatisfied);
	printArray(result.res);
	printf("\n%f", finish - start);
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
	struct Formula* formula_copy = copyFormula(formula);
	std::cout << std::endl;

	struct Result result = DPLL(formula);
	float finish = clock();
	result.time = finish - start;

	char* path = strtok(filename, ".cnf");
	char* w_filename = strcat(path, ".res");
	filePrint(w_filename, result);
}