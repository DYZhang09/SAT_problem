

#pragma once
#include"puzzle.h"

/**
@brief: ��ӡ����ָ��
*/
void printPlayGuide()
{
	printf("/*�밴���¸�ʽ������: ��(�ո�) ��(�ո�) 0/1/-1(�������, ֱ�ӷų����)\n");
}


/**
@brief: �������Ϸ���
@param input: ���������������
@return: �Ϸ��򷵻�true
*/
bool checkInput(int* input)
{
	if (input[0] > 0 and input[0] <= puzzle_size
		and input[1] > 0 and input[1] <= puzzle_size
		and (input[2] == 0 or input[2] == 1 or input[2] == -1)) {
		return true;
	}
	return false;
}


/**
@brief: ����Ƿ����̽���Ƿ���ȷ
@param p: ����
@return: ��ȷ�򷵻�true
*/
bool checkValid(struct Puzzle p, struct Formula* formula)
{
	int result[puzzle_size * puzzle_size + 1];
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + j + 1;
			if (p.puzzle[i][j] == -1) 
				result[data] = 0;
			else 
				result[data] = (p.puzzle[i][j] > 0) ? 1 : -1;
		}
	}
	bool res = evaluateFormula(formula, result);
	return res;
}


/**
@brief: ��������Ƿ��п�λ
@param p: ����
@return: ���п�λ�򷵻�true
*/
bool hasVacant(struct Puzzle p)
{
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			if (1 - p.puzzle[i][j] > 1) return true;
	return false;
}


/**
@brief: ��ȡ�û����벢�жϺϷ���
@return: �Ϸ����û�����
*/
int* getPlayInput()
{
	int input[3] = { -1 };
	printPlayGuide();
	scanf("%d %d %d", input, input + 1, input + 2);
	if (!checkInput(input)) {
		printf("���������Ƿ����\n");
		return getPlayInput();
	}
	return input;
}


/**
@brief: ���û�����Ӧ�õ�����
@param p: ָ�����̵�ָ��
@param input: �û�������
*/
void applyInput(struct Puzzle* p, int* input)
{
	int row = input[0], col = input[1];
	p->puzzle[row - 1][col - 1] = input[2];
}

