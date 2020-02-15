/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 数独结构相关头文件		                          **/
/**********************************************************/

#pragma once

#include"../config/config.h"

//表示棋盘的结构体
struct Puzzle
{
	int puzzle[puzzle_size][puzzle_size];		//棋盘数组
	int mask[puzzle_size][puzzle_size];		//掩膜数组, 用来表示某个位置是否只读
	int level;		//难度系数, 表示生成的数独初始数字的个数
};