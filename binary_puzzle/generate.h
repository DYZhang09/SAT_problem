/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: 数独生成相关头文件		                          
/**********************************************************/
#pragma once
#include"../config/config.h"
#include"comb_tool.h"
#include"puzzle.h"
#include"tool_function.h"
#include"transform.h"

/**
@brief: 初始化一个棋盘
@return: 初始化后的一个棋盘
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
@brief: 生成棋盘规则限制的公式
@return: 指向限制公式的指针
*/
struct Formula* restrain()
{
	static struct Formula* formula = initFormula();
	transformRule1(formula);
	transformRule2(formula);
	transformRule3(formula);
	return formula;
}


//棋盘的规则限制公式
static struct Formula* g_rule = restrain();


/**
@brief: dfs求解数独
@param p: 指向数独的指针
@return: 若有解则返回true
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
@brief: 拉斯维加斯算法初始化n个数独数字
@param p: 指向数独的指针
@param n: 初始化数字的个数
@return: 生成成功返回true
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
@brief: 判断挖掉某个数字数独是否唯一解
@param p: 目标数独
@param r: 第r行(从0开始)
@param c: 第c列(从0开始)
@return: 有唯一解则返回true
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
@brief: 挖洞法生成数独并有唯一解
@param p: 指向数独的指针
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
@brief: 随机地挖除数独的数字
@param p: 指向数独的指针
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
@brief: 获取玩家输入的难度等级, 同时进行合法性判断
@param p: 指向数独的指针
*/
void getLevel(struct Puzzle* p)
{
	int difficulty = 0;
	printf("/*请输入难度等级(1-3难度依次增加): ");
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
		printf("/*请检查输入的难度等级\n");
		getLevel(p);
		break;
	}
	return;
}


/**
@brief: 生成一个棋盘
@return: 生成后的棋盘
*/
struct Puzzle generatePuzzle()
{
	printf("/*棋盘生成中\n");
	struct Puzzle p = initPuzzle();
	while (!lasVegas(&p));

	getLevel(&p);
	digHoleRandom(&p);
	return p;
}