/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/21                                                     
//* Description: 定义了cnf求解所需的工具函数                       
/**********************************************************/

#pragma once
#include"../data_structure/data_struct.h"


/**
@brief: 判断子句中有无指定的文字
@param clause: 指向需要判断的子句的指针
@param data: 指定文字的序号
@return: 找到返回true, 否则false
*/
bool hasData(struct Clause* clause, short data)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {		//遍历判断
		if (curr->data == data) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}


/**
@brief: 判断公式中是否有空子句
@param formula: 指向公式的指针
@return: 有空子句返回true, 否则返回false
*/
bool hasVoidClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历判断
		if (curr->len == 0) return true;
		else curr = curr->nextClause;
	}
	return false;
}


/**
@brief: 删除公式里所有子句所包含的特定文字
@param formula: 指向需要删除子句文字的公式的指针
@param data: 需要删除的文字的序号
@calls: deleteLiteral()
@return: 被删除的文字的序号
*/
short removeLiteralFromFormula(struct Formula* formula, short data)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		deleteLiteral(curr, data);
		curr = curr->nextClause;
	}
	return data;
}


/**
@brief: 删除公式里所有包含指定文字的子句
@param formula: 指向需要删除子句的公式的指针
@param data: 需要删除的文字的序号
@calls: hasData()
@return: 被删除的文字的序号
*/
short removeClauseHasLiteral(struct Formula* formula, short data)
{
	struct Clause* target;
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历删除
		if (hasData(curr, data)) {
			curr->beforeClause->nextClause = curr->nextClause;
			curr->nextClause->beforeClause = curr->beforeClause;
			target = curr;
			curr = curr->nextClause;
			//free(target);
			destroyClause(target);
			formula->num_clause--;
		}
		else curr = curr->nextClause;
	}
	return data;
}


/**
@brief: 判断一个子句是否为单子句
@param clause: 指向需要判断的子句的指针
@return: 为单子句则为true, 否则为false
*/
bool isUnitClause(struct Clause* clause)
{
	return (clause->len == 1) ? true : false;
}


/**
@brief: 选取公式的第一个子句的第一个文字
@param formula: 指向公式的指针
@return: 选取的文字的序号
*/
short selectFirstData(struct Formula* formula)
{
	return	formula->head->nextClause->head->nextLiteral->data;
}


/**
@brief: 从公式中选取一个单子句并返回其文字序号, 若不存在单子句则返回0
@param formula: 指向公式的指针
@calls: isUnitClause()
@return: 单子句的文字序号, 若不存在单子句返回0
*/
short selectDataFromUnitClause(struct Formula* formula)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历，直到找到一个单子句
		if (isUnitClause(curr)) return curr->head->nextLiteral->data;
		else curr = curr->nextClause;
	}
	return 0;
}


/**
@brief: 评估子句真假
@param clause: 指向需要评估子句的指针
@param var: 变元值数组
@return: 子句为真则为true, 否则为false
*/
bool evaluateClause(struct Clause* clause, short* var)
{
	struct Literal* curr = clause->head->nextLiteral;
	while (!curr->isTail) {		//当存在文字序号与所赋的值同号时表明为真
		if (curr->data * var[abs(curr->data)] >= 0) return true;
		else curr = curr->nextLiteral;
	}
	return false;
}


/**
@brief: 评估公式的真假
@param formula: 指向需要评估公式的指针
@param var: 变元值数组
@calls: evaluateClause()
@return: 公式为真则为true, 否则为false
*/
bool evaluateFormula(struct Formula* formula, short* var, bool verbose = false)
{
	struct Clause* curr = formula->head->nextClause;
	while (!curr->isLastClause) {		//遍历，只要有子句不满足则整个公式即不满足
		bool flag = evaluateClause(curr, var);
		if (verbose) printf("%d\n", flag);
		if (!flag) return false;
		curr = curr->nextClause;
	}
	return true;
}


/**
@brief: 根据所选的文字赋值并进行传播
@param formula: 指向公式的指针
@param res: 存放所有文字赋值的数组
@param data: 所选的文字序号
*/
void applySelData(struct Formula* formula, short* res, short data)
{
	res[abs(data)] = data;
	removeClauseHasLiteral(formula, data);
	removeLiteralFromFormula(formula, -data);
}