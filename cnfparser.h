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

void skip(const char* filename)
{
	/**
	@brief: 跳过cnf文件注释行, 获取总体信息
	@param filename: cnf文件名
	*/
	FILE* fp = fopen(filename, "r");
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);
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
		exit(1);
	}
	else {
		char num[30];
		struct Formula* formula = initFormula();
		skip(filename);
		
	}
}

