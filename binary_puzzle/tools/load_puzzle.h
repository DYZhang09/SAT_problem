/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/29                                                     
//* Description: ������ȡ��غ���ͷ�ļ�                        
/**********************************************************/

#pragma once
#include"../generator/generate.h"

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
@param p: ָ�����̵�ָ��
@return: �ɹ���ȡ�򷵻�true
*/
bool loadPuzzleFromFile(const char* filename, struct Puzzle* p)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("�޷����ļ�: %s\n", filename);
		return false;
	}
	else {
		*p = initPuzzle();
		int c = -1;
		for (int i = 0; i < puzzle_size; i++) {
			for (int j = 0; j < puzzle_size; j++) {
				fscanf(fp, "%d", &c);
				if (c != -1) p->puzzle[i][j] = c;
			}
		}
		return true;
	}
}
