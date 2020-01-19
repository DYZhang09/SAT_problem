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
	@brief: ����cnf�ļ�ע����, ��ȡ������Ϣ
	@param fp: cnf�ļ�ָ��
	*/
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;		//����"p"��˵����ȡ��������Ϣ��
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);		//��ȡ������Ϣ
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
		return NULL;
	}
	else {
		int num=0, i = 1;
		struct Formula* formula = initFormula();		//��ʼ����ʽ
		skip(fp);	//�����ļ�ע����

		struct Clause* clause = addClause(formula);	//�ر���:��ӵ�һ���Ӿ�
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i < info.num_clause) {		//������β���½�һ���Ӿ�
				clause = addClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//����ǰ�Ӿ����һ������
				addLiteral(clause, num);
			}
			else break;		//��ȡ�������
		}
		printFormula(formula);
		return formula;
	}
}

