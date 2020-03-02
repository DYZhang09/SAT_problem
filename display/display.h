/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: ����ģ�����ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"./debug.h"

#include"../binary_puzzle/play.h"
#include"../naive_dpll/solver/solver.h"
#include"../Optimized_implementation/solver/solver.h"
#include"../walksat-dpll/dpll.h"

//ģʽ״̬
//MAIN: ������(ѡ��ģʽ����), 
//CNF:CNF���ģʽ, 
//PUZZLE: �������ģʽ
//PUZZLE_PLAY: ��������ģʽ
//DEBUG: DEBUGģʽ(�����֤��Ϣ)
//ESC: �˳�����
enum
{
	MAIN, CNF, PUZZLE, PUZZLE_PLAY, DEBUG, ESC
} mode;


/**
@brief: ��ӡ˵������
*/
void printGuide()
{
	printf("/**********************************************************************************/\n");
	printf("/*                                    ��ӭ                                        */\n");
	printf("/*                     �˳�����������CNF��ʽ���������                          */\n");
	printf("/*               ����c���������CNF��ʽģʽ, ����p�������������ģʽ              */\n");
	printf("/*             ��ͨ������ESC������ģʽѡ�����, �ٴΰ���ESC���˳�����             */\n");
	printf("/**********************************************************************************/\n");
}


/**
@brief: �����cnf�Ľ��д���ļ�
@param filename: д���ļ���·��
@param result: ���
*/
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
@brief: ��ʾCNF������������ȡ�û�����
@return: ������ļ�·����
*/
char* getCnfFileName()
{
	printf("\n/*������cnf�ļ�·��:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: ��ʾPUZZLE������������ȡ�û�����
@return: ������ļ�·����
*/
char* getPuzzleName()
{
	printf("\n/*������Puzzle�����ļ�·��:\n");
	char* filename = (char*)malloc(sizeof(char) * 255);
	scanf("%s", filename);
	return filename;
}


/**
@brief: ���cnf�����ļ������·��
@param cnf_filename: cnf�����ļ�·��
@return: ���·��
*/
char* cnfOutputName(char* cnf_filename)
{
	char* suffix = strstr(cnf_filename, ".cnf");
	char c[5] = { '.', 'r', 'e', 's', '\0' };
	int suffix_len = strlen(suffix);
	for (int i = 0; i < suffix_len; i++)
		suffix[i] = c[i];
	return cnf_filename;
}


/**
@brief: ģʽѡ����
*/
void modeChange()
{
	mode = MAIN;
	printf("/*�����ģʽѡ��(����ѡ��: c - CNF���ģʽ, p - �������ģʽ, g - ��������ģʽ, d - debugģʽ, ESC�˳�): \n");
	char c = getch();
	if (c == 'c' or c == 'C') {
		mode = CNF;
		printf("/*�������CNF��ʽģʽ\n");
	}
	else if (c == 'p' or c == 'P') {
		mode = PUZZLE;
		printf("/*�����������ģʽ(��ǰ��������Ϊ%d)\n", puzzle_size);
	}
	else if (c == 'g' or c == 'G') {
		mode = PUZZLE_PLAY;
		printf("/*������������ģʽ(��ǰ��������Ϊ%d)\n", puzzle_size);
	}
	else if (c == 'd' or c == 'D') {
		mode = DEBUG;
		printf("/*����debugģʽ\n");
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


/**
@brief: ����cnf������ӿ�
*/
void callCnfSolver()
{
	char* cnf_filename = getCnfFileName();		//��ȡcnf�����ļ�·��
	struct Formula* formula = loadFile(cnf_filename);		//��ȡ�ļ��õ�CNF��ʽ

	float start = clock();		//ʱ�����
	struct Result result = DPLL(formula);		//����DPLL�������CNF��ʽ
	float finish = clock();	//ʱ���յ�
	result.time = finish - start;		//���ʱ��

	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	free(result.res);			//�ͷſռ�
	free(cnf_filename);
	destoryFormula(formula);
}


/**
@brief: ����cnf���ģ��(�����Ż��汾)
*/
void callCnfSolverOpti()
{
	char* cnf_filename = getCnfFileName();
	int* counter = initCounter();
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);

	float start = clock();		//ʱ�����
	struct Result result = DPLLOpti(formula, counter);		//����DPLL�������CNF��ʽ
	float finish = clock();	//ʱ���յ�
	result.time = finish - start;		//���ʱ��

	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	free(result.res);			//�ͷſռ�
	free(cnf_filename);
	free(counter);
	destoryFormula(formula);
}


/**
@brief: ��������������ӿ�
*/
void callPuzzleSolver()
{
	char* puzzle_filename = getPuzzleName();		//��ȡ��ʼ�����ļ�
	struct Puzzle p = loadPuzzleFromFile(puzzle_filename);		//���ļ��ж�ȡ����
	solvePuzzle(p);		//�������
	free(puzzle_filename);		//�ͷſռ�
}


/**
@brief: ���������������ӿ�
*/
void callPuzzlePlayer()
{
	struct Puzzle p = generatePuzzle();
	puzzlePlay(p);
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
@brief: debugģʽ���ýӿ�
*/
void callDebug()
{
	char* filename = getCnfFileName();
	debug(filename);
}


/**
@brief: ����ģ��������
*/
void display()
{
	printGuide();		//��ӡ˵������
	modeChange();	//ģʽѡ��
	while (mode != ESC){		//��ģʽ����ESCʱѭ��
		if (mode == CNF) {
			if(cnf_solver_version == 0) callCnfSolver();		//����CNF���ģ��
			if (cnf_solver_version == 1) callCnfSolverOpti();
		}
		if (mode == PUZZLE) callPuzzleSolver();		//�����������ģ��
		if (mode == PUZZLE_PLAY) callPuzzlePlayer();	//������������ģ��
		if (mode == DEBUG) callDebug();
		printf("/*����ESC���˻ص�ģʽѡ��, ���������������ǰģʽ.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}