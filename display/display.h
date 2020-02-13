/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ����ģ�����ͷ�ļ�		                          **/
/**********************************************************/

#pragma once
#include"../binary_puzzle/play.h"

//ģʽ״̬
//MAIN: ������(ѡ��ģʽ����), 
//CNF:CNF���ģʽ, 
//PUZZLE: �������ģʽ
//PUZZLE_PLAY: ��������ģʽ
//ESC: �˳�����
enum
{
	MAIN, CNF, PUZZLE, PUZZLE_PLAY, ESC
} mode;


/**
@brief: ��ӡ˵������
*/
void printGuide()
{
	printf("/**************************************************************/\n");
	printf("/*                          ��ӭ                              */\n");
	printf("/*           �˳�����������CNF��ʽ���������                */\n");
	printf("/*     ����c���������CNF��ʽģʽ, ����p�������������ģʽ    */\n");
	printf("/*   ��ͨ������ESC������ģʽѡ�����, �ٴΰ���ESC���˳�����   */\n");
	printf("/**************************************************************/\n");
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
@brief: ģʽѡ����
*/
void modeChange()
{
	mode = MAIN;
	printf("/*�����ģʽѡ��(����c���������CNF��ʽģʽ, ����p�������������ģʽ, ����g��������������ģʽ, esc�˳�����): \n");
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

	char* path = strtok(cnf_filename, ".cnf");		//��ȡ���д���ļ���·��
	char* w_filename = strcat(path, ".res");
	cnfResultPrint(w_filename, result);		//�����д���ļ�
	free(result.res);			//�ͷſռ�
	free(cnf_filename);
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
	char* puzzle_filename = getPuzzleName();
	struct Puzzle p = loadPuzzleFromFile(puzzle_filename);
	puzzlePlay(p);
	free(puzzle_filename);
}


/**
@brief: ����ģ��������
*/
void display()
{
	printGuide();		//��ӡ˵������
	modeChange();	//ģʽѡ��
	while (mode != ESC){		//��ģʽ����ESCʱѭ��
		if (mode == CNF) callCnfSolver();		//����CNF���ģ��
		if (mode == PUZZLE) callPuzzleSolver();		//�����������ģ��
		if (mode == PUZZLE_PLAY) callPuzzlePlayer();	//������������ģ��
		printf("/*����ESC���˻ص�ģʽѡ��, ���������������ǰģʽ.\n");
		char c = getch();
		if (c == 27) modeChange();
	}
}