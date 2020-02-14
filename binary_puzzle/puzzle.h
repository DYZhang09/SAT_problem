#pragma once

#include"../config/config.h"

//表示棋盘的结构体
struct Puzzle
{
	int puzzle[puzzle_size][puzzle_size];
	int level;
};