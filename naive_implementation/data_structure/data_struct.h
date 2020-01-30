/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: CNF公式存储数据结构定义					  **/
/**********************************************************/

#pragma once
#include"../../config/config.h"

//------------------------naive data structure---------------------//
struct Literal			//文字结构体，data对应文字序号
{
	int data = 0;
	struct Literal* nextLiteral = NULL;
	struct Literal* beforeLiteral = NULL;
	bool isHead = false;
	bool isTail = false;
};

struct Clause		//子句结构体
{
	struct Literal* head;
	struct Literal* tail;
	struct Clause* nextClause = NULL;
	struct Clause* beforeClause = NULL;

	bool isFirstClause = false;
	bool isLastClause = false;
	int len = 0;
};

struct Formula		//公式结构体
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};
