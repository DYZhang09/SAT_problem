/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独生成相关头文件		                          **/
/**********************************************************/
#pragma once
#include"../naive_implementation/config/config.h"

struct Puzzle
{
	char puzzle[puzzle_size][puzzle_size] = { -1 };
};


void drawLineTerminal()
{
	for (int i = 0; i < 16 * puzzle_size + 1; i++)
		printf("-");
	printf("\n");
}


void drawLineFile(FILE* fp)
{
	for (int i = 0; i < 21 * puzzle_size + 1; i++)
		fprintf(fp, "%s", "-");
	fprintf(fp, "%s", "\n");
}


void drawTerminal(struct Puzzle p)
{
	drawLineTerminal();
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			if (p.puzzle[i][j] > -1) printf("|\t%d\t", p.puzzle[i][j]);
		}
		printf("|\n");
		drawLineTerminal();
	}
}


void drawFile(struct Puzzle p, const char* filename)
{
	FILE* fp = fopen(filename, "w+");
	if (!fp) {
		printf("无法打开文件: %s\n", filename);
		return;
	}
	else {
		drawLineFile(fp);
		for (int i = 0; i < puzzle_size; i++) {
			for (int j = 0; j < puzzle_size; j++) {
				if (p.puzzle[i][j] > -1) fprintf(fp, "|\t%d\t", p.puzzle[i][j]);
			}
			fprintf(fp, "%s", "|\n");
			drawLineFile(fp);
		}
		fclose(fp);
	}
}


void draw(struct Puzzle p, const char* filename = NULL)
{
	if (!filename) drawTerminal(p);
	else drawFile(p, filename);
}


struct Puzzle generatePuzzle()
{
	struct Puzzle p;
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			p.puzzle[i][j] = rand() % 2;
	return p;
}