/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: CNF公式存储数据结构定义					  **/
/**********************************************************/

#pragma once
#include"naive_implementation/config/config.h"

//------------------------naive data structure---------------------//
struct Literal
{
	int data = 0;
	struct Literal* nextLiteral = NULL;
	struct Literal* beforeLiteral = NULL;
	bool isHead = false;
	bool isTail = false;
};

struct Clause
{
	struct Literal* head;
	struct Literal* tail;
	struct Clause* nextClause = NULL;
	struct Clause* beforeClause = NULL;

	bool isFirstClause = false;
	bool isLastClause = false;
	int len = 0;
};

struct Formula
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};
