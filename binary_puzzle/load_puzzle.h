/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/29                                                     **/
/** Description: ������ȡ��غ���ͷ�ļ�                        **/
/**********************************************************/

#pragma once
#include"generate.h"

/**
@brief: ���������ж�ȡ����
@return: ��ȡ���ݺ������
*/
struct Puzzle loadPuzzleFromTerm()
{
	struct Puzzle p = initPuzzle();
	int c = -1;
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			scanf("%d", &c);
			if (c != -1) p.puzzle[i][j] = c;
		}
	}
	return p;
}


/**
@brief: ���ļ��ж�ȡ����
@param filename: Ŀ���ļ�·����
@return: ��ȡ���ݺ������
*/
struct Puzzle loadPuzzleFromFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("�޷����ļ�: %s\n", filename);
		exit(-1);
	}
	else {
		struct Puzzle p = initPuzzle();
		int c = -1;
		for (int i = 0; i < puzzle_size; i++) {
			for (int j = 0; j < puzzle_size; j++) {
				fscanf(fp, "%d", &c);
				if (c != -1) p.puzzle[i][j] = c;
			}
		}
		return p;
	}
}