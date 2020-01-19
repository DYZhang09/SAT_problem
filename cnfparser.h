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
	@brief: ����cnf�ļ�ע����, ��ȡ������Ϣ
	@param filename: cnf�ļ���
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
	@brief: ��cnf�ļ��ж�ȡ���ݹ�����ʽ
	@param filename: cnf�ļ���
	@return: ������ɵĹ�ʽ
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

