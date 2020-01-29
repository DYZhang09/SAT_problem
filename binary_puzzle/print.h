/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 棋盘打印相关头文件		                          **/
/**********************************************************/

#pragma once
#include"generate.h"

/**
@brief: 终端输出一排短线
*/
void drawLineTerminal()
{
	for (int i = 0; i < 4 * puzzle_size + 1; i++)
		printf("-");
	printf("\n");
}


/**
@brief: 在文件中输出一排短线
@param fp: 指向输出文件的文件指针
*/
void drawLineFile(FILE* fp)
{
	for (int i = 0; i < 21 * puzzle_size + 1; i++)
		fprintf(fp, "%s", "-");
	fprintf(fp, "%s", "\n");
}


/**
@brief: 将目标棋盘输出至终端
@param p: 待输出的棋盘
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
@brief: 将目标棋盘输出至文件
@param p: 待输出的棋盘
@param filename: 输出文件路径名
*/
void drawFile(struct Puzzle p, const char* filename)
{
	FILE* fp = fopen(filename, "w+");
	if (!fp) {
		printf("无法打开文件: %s\n", filename);
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
@brief: 棋盘输出总接口
@param p: 待输出的棋盘
@param filename: 输出文件的路径名，若为NULL则表示输出至终端
*/
void draw(struct Puzzle p, const char* filename = NULL)
{
	if (!filename) drawTerminal(p);
	else drawFile(p, filename);
}