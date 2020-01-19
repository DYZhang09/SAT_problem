#pragma once
#include<stdio.h>
#include<string.h>
#include<iostream>
#include"data_struct.h"

struct ProblemInfo
{
	int num_clause;
	int num_literal;
}info;

void skip(FILE* fp)
{
	/**
	@brief: 跳过cnf文件注释行, 获取总体信息
	@param fp: cnf文件指针
	*/
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;		//遇到"p"，说明读取到总体信息行
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);		//读取总体信息
}

struct Formula* loadFile(const char* filename)
{
	/**
	@brief: 从cnf文件中读取数据构建公式
	@param filename: cnf文件名
	@return: 构建完成的公式
	*/
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("Can't open the file:%s", filename);
		return NULL;
	}
	else {
		int num=0, i = 1;
		struct Formula* formula = initFormula();		//初始化公式
		skip(fp);	//跳过文件注释行

		struct Clause* clause = addClause(formula);	//特别处理:添加第一个子句
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i < info.num_clause) {		//遇到行尾，新建一个子句
				clause = addClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//给当前子句添加一个文字
				addLiteral(clause, num);
			}
			else break;		//读取输入完毕
		}
		printFormula(formula);
		return formula;
	}
}

