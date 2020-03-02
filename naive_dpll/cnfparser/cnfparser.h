/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/20                                                     
//* Description: cnf文件解析模块头文件                          
/**********************************************************/

#pragma once
#include"../data_structure/data_struct.h"


/**
@brief: 跳过cnf文件注释行, 获取总体信息
@param fp: cnf文件指针
*/
void skip(FILE* fp)
{
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;		//遇到"p"，说明读取到总体信息行
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);		//读取总体信息
}


/**
@brief: 从cnf文件中读取数据构建公式
@param filename: cnf文件名
@return: 构建完成的公式
*/
struct Formula* loadFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("无法读取文件:%s", filename);
		exit(1);
	}
	else {
		int num=0, i = 1;
		struct Formula* formula = initFormula();		//初始化公式
		skip(fp);	//跳过文件注释行

		struct Clause* clause = createClause(formula);	//特别处理:添加第一个子句
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i < info.num_clause) {		//遇到行尾，新建一个子句
				clause = createClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//给当前子句添加一个文字
				addLiteral(clause, num);
			}
			else break;		//读取输入完毕
		}

		fclose(fp);
		return formula;
	}
}

