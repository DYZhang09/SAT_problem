/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: �����������ͷ�ļ�		                          
/**********************************************************/
#pragma once
#include"../config/config.h"
#include"comb_tool.h"
#include"puzzle.h"
#include"tool_function.h"
#include"transform.h"

/**
@brief: ��ʼ��һ������
@return: ��ʼ�����һ������
*/
struct Puzzle initPuzzle()
{
	struct Puzzle p;
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++) {
			p.puzzle[i][j] = -1;
			p.mask[i][j] = 0;
		}
	
	p.level == puzzle_size * puzzle_size - 1;
	return p;
}


/**
@brief: �������̹������ƵĹ�ʽ
@return: ָ�����ƹ�ʽ��ָ��
*/
struct Formula* restrain()
{
	static struct Formula* formula = initFormula();
	transformRule1(formula);
	transformRule2(formula);
	transformRule3(formula);
	return formula;
}


//���̵Ĺ������ƹ�ʽ
static struct Formula* g_rule = restrain();


/**
@brief: dfs�������
@param p: ָ��������ָ��
@return: ���н��򷵻�true
*/
bool dfs(struct Puzzle *p)
{
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			if (p->puzzle[i][j] < 0) {
				for (int ans = 0; ans < 2; ans++) {
					p->puzzle[i][j] = ans;
					if (checkValid(*p, g_rule)) {
						p->puzzle[i][j] = ans;
						if (dfs(p)) return true;
					}
					p->puzzle[i][j] = -1;
				}
				return false;
			}
		}
	}
	return true;
}


/**
@brief: ��˹ά��˹�㷨��ʼ��n����������
@param p: ָ��������ָ��
@param n: ��ʼ�����ֵĸ���
@return: ���ɳɹ�����true
*/
bool lasVegas(struct Puzzle* p, int n = puzzle_size * puzzle_size / 6 + 1)
{
	int i, j, value;
	srand((unsigned)time(NULL));
	
	while (n > 0) {
		i = rand() % puzzle_size;
		j = rand() % puzzle_size;
		if (p->puzzle[i][j] < 0) {
			value = rand() % 2;
			p->puzzle[i][j] = value;
			if (checkValid(*p, g_rule)) n--;
			else p->puzzle[i][j] = -1;
		}
	}

	if(dfs(p)) 
		return true;
	else
		return false;
}


/**
@brief: �ж��ڵ�ĳ�����������Ƿ�Ψһ��
@param p: Ŀ������
@param r: ��r��(��0��ʼ)
@param c: ��c��(��0��ʼ)
@return: ��Ψһ���򷵻�true
*/
bool isUnique(struct Puzzle p, int r, int c)
{
	if (r == 0 and c == 0) return true;

	struct Puzzle copy_p = p;

	copy_p.puzzle[r][c] = -1;
	for (int i = 0; i < 2; i++) {
		if (i != p.puzzle[r][c]) {
			copy_p.puzzle[r][c] = i;
			if (checkValid(copy_p, g_rule)) {
				copy_p.puzzle[r][c] = i;
				if (dfs(&copy_p)) return false;
			}
		}
	}
	return true;
}


/**
@brief: �ڶ���������������Ψһ��
@param p: ָ��������ָ��
*/
void digHoleUnique(struct Puzzle* p)
{
	int level = p->level;
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			if (isUnique(*p, i, j)) {
				p->puzzle[i][j] = -1;
				level++;
				if (level == puzzle_size * puzzle_size)
					break;
			}
		}
	}
}


/**
@brief: ������ڳ�����������
@param p: ָ��������ָ��
*/
void digHoleRandom(struct Puzzle* p)
{
	int level = p->level;
	while (level <= puzzle_size * puzzle_size) {
		int r = rand() % puzzle_size;
		int c = rand() % puzzle_size;
		if (p->puzzle[r][c] >= 0) {
			p->puzzle[r][c] = -1;
			p->mask[r][c] = 1;
			level++;
		}
	}
}


/**
@brief: ��ȡ���������Ѷȵȼ�, ͬʱ���кϷ����ж�
@param p: ָ��������ָ��
*/
void getLevel(struct Puzzle* p)
{
	int difficulty = 0;
	printf("/*�������Ѷȵȼ�(1-3�Ѷ���������): ");
	scanf("%d", &difficulty);
	switch (difficulty)
	{
	case 3:
		p->level = puzzle_size * puzzle_size / 3;
		break;
	case 2:
		p->level = puzzle_size * puzzle_size/ 3 + puzzle_size;
		break;
	case 1:
		p->level = puzzle_size * puzzle_size/ 3 + 2 * puzzle_size;
		break;
	default:
		printf("/*����������Ѷȵȼ�\n");
		getLevel(p);
		break;
	}
	return;
}


/**
@brief: ����һ������
@return: ���ɺ������
*/
struct Puzzle generatePuzzle()
{
	printf("/*����������\n");
	struct Puzzle p = initPuzzle();
	while (!lasVegas(&p));

	getLevel(&p);
	digHoleRandom(&p);
	return p;
}