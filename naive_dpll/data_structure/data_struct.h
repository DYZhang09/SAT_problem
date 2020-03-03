/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/20                                                     
//* Description: CNF公式存储数据结构定义					  
/**********************************************************/

#pragma once
#include"../../config/config.h"

//------------------------naive data structure---------------------//
struct Literal			//文字结构体，data对应文字序号
{
	short data = 0;
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
	short len = 0;
};

struct Formula		//公式结构体
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};


//CNF总体信息结构体
struct ProblemInfo
{
	short num_literal;
	int num_clause;
}info;


struct Result
{
	bool isSatisfied;
	short* res;
	int* counter;
	float time;
};


//----------------------------------------functions---------------------------------------------------//
/**
@brief: 初始化一个文字
@param ... : Literal各成员初始值
@return: 初始化后的指向文字的指针
*/
struct Literal* initLiteral(short data = 0, struct Literal* nextLiteral = NULL, struct Literal* beforeLiteral = NULL,
										bool isHead = false, bool isTail = false)
{
	struct Literal* new_lit = (struct Literal*)malloc(sizeof(struct Literal));
	new_lit->data = data;
	new_lit->nextLiteral = nextLiteral;
	new_lit->beforeLiteral = beforeLiteral;
	new_lit->isHead = isHead;
	new_lit->isTail = isTail;
	return new_lit;
}


/**
@brief: 初始化一个子句
@param ... : Clause的成员初始值
@return: 指向初始化子句的指针
*/
struct Clause* initClause(struct Clause* nextClause = NULL,struct Clause* beforeClause = NULL, 
										bool isFirstCla = false, bool isLastCla = false)
{
	struct Clause* clause = (struct Clause*) malloc(sizeof(struct Clause));			//动态分配子句空间
	clause->head = initLiteral(0, NULL, NULL, true, false);
	clause->tail = initLiteral(0, NULL, clause->head, false, true);
	clause->head->nextLiteral = clause->tail;
	clause->nextClause = nextClause;
	clause->beforeClause = beforeClause;
	clause->isFirstClause = isFirstCla;
	clause->isLastClause = isLastCla;
	clause->len = 0;
	return clause;
}


/**
@brief: 初始化一个公式
@calls: initClause()
@return: 指向初始化公式的指针
*/
struct Formula* initFormula()
{
	struct Formula* formula = (struct Formula*)malloc(sizeof(struct Formula));		//动态分配公式空间
	formula->head = initClause(NULL, NULL, true, false);
	formula->tail = initClause(NULL, formula->head, false, true);
	formula->head->nextClause = formula->tail;
	formula->num_clause = 0;
	return formula;
}


/*
@brief: 初始化一个答案结构体
@return: 初始化后的答案结构体
*/
struct Result initResult()
{
	struct Result result;
	result.isSatisfied = false;		//初始化结果
	result.res = (short*)malloc(sizeof(short) * (info.num_literal + 1));
	memset(result.res, 0, sizeof(short) * (info.num_literal + 1));
	return result;
}


/**
@brief: 给一个子句尾部加上一个文字
@param clause: 需要添加文字的指向子句的指针
@param data: 添加文字的序号
*/
void addLiteral(struct Clause* clause, short data)
{
	struct Literal* tl = clause->tail;
	struct Literal* temp = initLiteral(data, tl, tl->beforeLiteral);
	tl->beforeLiteral->nextLiteral = temp;		//添加文字
	tl->beforeLiteral = temp;
	clause->len++;

}


/**
@brief: 删除指定子句中的指定文字
@param clause: 指向指定子句的指针
@param data: 需要删除的文字的序号
*/
void deleteLiteral(struct Clause* clause, short data)
{
	struct Literal* curr = clause->head->nextLiteral;
	struct Literal* target = curr;
	while (!curr->isTail) {
		if (curr->data == data) {		//定位需要删除的文字
			target = curr;
			target->beforeLiteral->nextLiteral = target->nextLiteral;		//删除对应文字
			target->nextLiteral->beforeLiteral = target->beforeLiteral;
			curr = curr->nextLiteral;
			free(target);
			clause->len--;
		}
		else curr = curr->nextLiteral;
	}
}


/**
@brief: 给指定公式尾部创建一个子句
@param formula: 指定的公式
@return: 指向新添加子句的指针
*/
struct Clause* createClause(struct Formula* formula)
{
	struct Clause* tl = formula->tail;
	struct Clause* temp = initClause(tl, tl->beforeClause);
	tl->beforeClause->nextClause = temp;
	tl->beforeClause = temp;
	formula->num_clause++;
	return temp;
}


/**
@brief: 给公式末尾添加一个子句
@param formula: 指向公式的指针
@param clause: 指向被添加子句的指针
*/
void addClause(struct Formula* formula, struct Clause* clause)
{
	struct Clause* tl = formula->tail;
	clause->isFirstClause = clause->isLastClause = false;		//添加子句
	tl->beforeClause->nextClause = clause;
	clause->beforeClause = tl->beforeClause;
	clause->nextClause = tl;
	tl->beforeClause = clause;
	formula->num_clause++;
}


/**
@brief: 复制公式
@param formula: 指向被复制公式的指针
@calls: initFormula(), addClause(), addLiteral()
@return: 指向复制后公式的指针
*/
struct Formula* copyFormula(struct Formula* formula)
{
	struct Formula* formula_copy = initFormula();		//初始化公式空间
	struct Clause* curr_clause = formula->head->nextClause;
	struct Clause* curr_clause_copy = formula_copy->head;
	while (!curr_clause->isLastClause) {			//遍历复制公式
		curr_clause_copy = createClause(formula_copy);
		struct Literal* curr_literal = curr_clause->head->nextLiteral;
		while (!curr_literal->isTail) {
			addLiteral(curr_clause_copy, curr_literal->data);
			curr_literal = curr_literal->nextLiteral;
		}
		curr_clause = curr_clause->nextClause;
	}
	return formula_copy;
}


/**
@brief: 删除一个子句
@param clause: 指向被删除子句的指针
*/
void destroyClause(struct Clause* clause)
{
	struct Literal* curr = clause->tail;
	struct Literal* prev = curr->beforeLiteral;
	while (prev != NULL) {		//遍历删除文字
		free(curr);
		curr = prev;
		prev = prev->beforeLiteral;
	}
	clause->len = 0;
	free(clause->head);
	free(clause);
}


/**
@brief: 删除公式, 归还动态分配内存
@param formula: 指向被删除公式的指针
@calls: destroyClause()
*/
void destoryFormula(struct Formula* formula)
{
	struct Clause* curr = formula->tail;
	struct Clause* prev = curr->beforeClause;
	while (prev != NULL) {		//遍历删除子句集
		destroyClause(curr);
		curr = prev;
		prev = prev->beforeClause;
	}
	formula->num_clause = 0;
	destroyClause(formula->head);
	free(formula);
}