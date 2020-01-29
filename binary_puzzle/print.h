/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ���̴�ӡ���ͷ�ļ�		                          **/
/**********************************************************/

#pragma once
#include"generate.h"

/**
@brief: �ն����һ�Ŷ���
*/
void drawLineTerminal()
{
	for (int i = 0; i < 4 * puzzle_size + 1; i++)
		printf("-");
	printf("\n");
}


/**
@brief: ���ļ������һ�Ŷ���
@param fp: ָ������ļ����ļ�ָ��
*/
void drawLineFile(FILE* fp)
{
	for (int i = 0; i < 21 * puzzle_size + 1; i++)
		fprintf(fp, "%s", "-");
	fprintf(fp, "%s", "\n");
}


/**
@brief: ��Ŀ������������ն�
@param p: �����������
*/
void drawTerminal(struct Puzzle p)
{
	printf("Answer: \n");
	drawLineTerminal();
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			if (p.puzzle[i][j] > -1) printf("| %d ", p.puzzle[i][j]);
			else printf("|    ");
		}
		printf(" |\n");
		drawLineTerminal();
	}
}


/**
@brief: ��Ŀ������������ļ�
@param p: �����������
@param filename: ����ļ�·����
*/
void drawFile(struct Puzzle p, const char* filename)
{
	FILE* fp = fopen(filename, "w+");
	if (!fp) {
		printf("�޷����ļ�: %s\n", filename);
		return;
	}
	else {
		fprintf(fp, "\n%s\n", "Answer:");
		drawLineFile(fp);
		for (int i = 0; i < puzzle_size; i++) {
			for (int j = 0; j < puzzle_size; j++) {
				if (p.puzzle[i][j] > -1) fprintf(fp, "|\t%d\t", p.puzzle[i][j]);
			}
			fprintf(fp, "%s", "\t|\n");
			drawLineFile(fp);
		}
		fclose(fp);
	}
}


/**
@brief: ��������ܽӿ�
@param p: �����������
@param filename: ����ļ���·��������ΪNULL���ʾ������ն�
*/
void draw(struct Puzzle p, const char* filename = NULL)
{
	if (!filename) drawTerminal(p);
	else drawFile(p, filename);
}