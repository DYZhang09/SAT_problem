/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ����ģ�����ͷ�ļ�		                          **/
/**********************************************************/

#pragma once
#include"../binary_puzzle/solve_puzzle.h"


enum
{
	MAIN, CNF, PUZZLE, ESC
} mode;


void printGuide()
{
	printf("/**************************************************************/\n");
	printf("/*                          ��ӭ                              */\n");
	printf("/*           �˳�����������CNF��ʽ���������                */\n");
	printf("/*     ����c���������CNF��ʽģʽ, ����p�������������ģʽ    */\n");
	printf("/*   ��ͨ������ESC������ģʽѡ�����, �ٴΰ���ESC���˳�����   */\n");
	printf("/**************************************************************/\n");
}


void cnfResultPrint(char* filename, struct Result result)
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
	printf("/*�����д���ļ�:%s\n", filename);
	fclose(fp);
}


/**
@brief: ��ʾ������������ȡ�û�����
@return: ������ļ�·����
*/
char* getCnfFileName()
{
	printf("\n/*������cnf�ļ�·��:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


char* getPuzzleName()
{
	printf("\n/*������Puzzle�����ļ�·��:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


void modeChange()
{
	mode = MAIN;
	printf("/*�����ģʽѡ��(����c���������CNF��ʽģʽ, ����p�������������ģʽ, esc�˳�����): \n");
	char c = getch();
	if (c == 'c' or c == 'C') {
		mode = CNF;
		printf("/*�������CNF��ʽģʽ\n");
	}
	else if (c == 'p' or c == 'P') {
		mode = PUZZLE;
		printf("/*�����������ģʽ(��ǰ��������Ϊ%d)\n", puzzle_size);
	}
	else if (c == 27) {
			mode = ESC;
			printf("���˳�����\n");
	}
	else {
		printf("/*���鰴���Ƿ����!\n");
		modeChange();
	}
}


void callCnfSolver()
{
	char* cnf_filename = getCnfFileName();
	struct Formula* formula = loadFile(cnf_filename);

	float start = clock();
	struct Result result = DPLL(formula);
	float finish = clock();
	result.time = finish - start;

	char* path = strtok(cnf_filename, ".cnf");
	char* w_filename = strcat(path, ".res");
	cnfResultPrint(w_filename, result);
	free(result.res);
	free(cnf_filename);
	destoryFormula(formula);
}


void callPuzzleSolver()
{
	char* puzzle_filename = getPuzzleName();
	struct Puzzle p = loadPuzzleFromFile(puzzle_filename);
	solvePuzzle(p);
}


void display()
{
	printGuide();
	modeChange();
	while (mode != ESC){
		if (mode == CNF) callCnfSolver();
		if (mode == PUZZLE) callPuzzleSolver();
		printf("/*����ESC���˻ص�ģʽѡ��, ���������������ǰģʽ.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}