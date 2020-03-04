/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: ����ģ�����ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"./debug.h"

#include"../binary_puzzle/play.h"
#include"../naive_dpll/solver/solver.h"
#include"../opti_deci_dpll/solver/solver.h"
#include"../opti_memo_dpll/solver/solver.h"

//ģʽ״̬
//MAIN: ������(ѡ��ģʽ����), 
//CNF:CNF���ģʽ, 
//PUZZLE: �������ģʽ
//PUZZLE_PLAY: ��������ģʽ
//DEBUG: DEBUGģʽ(�����֤��Ϣ)
//ESC: �˳�����
enum g_mode
{
	MAIN, CNF, PUZZLE, PUZZLE_PLAY, DEBUG, ESC
} mode;


/**
@brief: ��ӡcnf������汾
*/
void printSolverVer()
{
	if (cnf_solver_version == 0) printf("/*����ʹ������汾�����\n");
	else if (cnf_solver_version == 1) printf("/*����ʹ�þ����Ż�������\n");
	else printf("/*����ʹ���ڴ�ʹ���Ż�������\n");
}


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
	printf("/*������cnf�ļ�·��:\n");
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


//@brief: ����ģʽ, ��ӡ�����Ϣ
void modeSet(g_mode _mode)
{
	mode = _mode;
	if(mode == CNF) printf("/*�������CNF��ʽģʽ\n");
	if (mode == PUZZLE) printf("/*�����������ģʽ(��ǰ��������Ϊ%d)\n", puzzle_size);
	if(mode == PUZZLE_PLAY) printf("/*������������ģʽ(��ǰ��������Ϊ%d)\n", puzzle_size);
	if(mode == DEBUG) printf("/*����debugģʽ\n");
	if(mode == ESC) printf("���˳�����\n");
}


/**
@brief: ģʽѡ����
*/
void modeChange()
{
	mode = MAIN;
	printf("/*�����ģʽѡ��(����ѡ��: c - CNF���ģʽ, p - �������ģʽ, g - ��������ģʽ, d - debugģʽ, ESC�˳�): \n");
	char c = getch();
	if (c == 'c' or c == 'C') modeSet(CNF);
	else if (c == 'p' or c == 'P') modeSet(PUZZLE);
	else if (c == 'g' or c == 'G') modeSet(PUZZLE_PLAY);
	else if (c == 'd' or c == 'D') modeSet(DEBUG);
	else if (c == 27) modeSet(ESC);
	else {	printf("/*���鰴���Ƿ����!\n");	modeChange();	}
}


/**
@brief: ����̬����Ŀռ�
@param .. : ָ����Ҫ���ͷſռ�ĸ���ָ��
*/
void handleTrash(char* filename, struct Result result, struct Formula* formula = NULL, int* counter = NULL)
{
	if(filename)	free(filename), filename = 0;
	if(result.res) free(result.res), result.res = 0;
	if (formula) destoryFormula(formula);
	if (counter) free(counter), counter = 0;
}


/**
@brief: ����cnf������ӿ�
*/
void callCnfSolver()
{
	printSolverVer();
	char* cnf_filename = getCnfFileName();		//��ȡcnf�����ļ�·��
	struct Formula* formula = loadFile(cnf_filename);		//��ȡ�ļ��õ�CNF��ʽ
	struct Result result = DPLL(formula);		//����DPLL�������CNF��ʽ
	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	handleTrash(cnf_filename, result, formula);
}


/**
@brief: ����cnf���ģ��(�����Ż��汾)
*/
void callCnfSolverOpti()
{
	printSolverVer();
	char* cnf_filename = getCnfFileName();
	int* counter = initCounter();
	struct Formula* formula = loadFile_opti1(cnf_filename, &counter);
	struct Result result = DPLLOpti(formula, counter);		//����DPLL�������CNF��ʽ
	cnf_filename = cnfOutputName(cnf_filename);
	cnfResultPrint(cnf_filename, result);
	handleTrash(cnf_filename, result, formula, counter);
}


/**
@brief: ����cnf���ģ��(�ڴ�ʹ���Ż��汾)
*/
void callCNfSolverOptiX()
{
	printSolverVer();
	char* filename = getCnfFileName();
	struct BinVector formula = binVecInit();
	struct Mask mask = maskInit();
	int* counter;
	if (!loadFile(filename, &formula, &mask, &counter)) return;
	else {
		struct Result result = WALKSAT_DPLL(&formula, &mask, counter);
		filename = cnfOutputName(filename);
		cnfResultPrint(filename, result);
		handleTrash(filename, result, NULL);
		freeBinVec(&formula);
	}
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
	struct Formula* formula = loadFile(filename);
	struct Formula* old_formula =  copyFormula(formula);
	printFormula(formula);
	struct Result result = DPLL(formula);
	printDebugInfo(result, old_formula);
	destoryFormula(old_formula);
	handleTrash(filename, result, formula);
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
			if (cnf_solver_version == 2) callCNfSolverOptiX();
		}
		if (mode == PUZZLE) callPuzzleSolver();		//�����������ģ��
		if (mode == PUZZLE_PLAY) callPuzzlePlayer();	//������������ģ��
		if (mode == DEBUG) callDebug();
		printf("\n/*����ESC���˻ص�ģʽѡ��, ���������������ǰģʽ.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}