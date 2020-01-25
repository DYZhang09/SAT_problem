/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: cnf�ļ�����ģ��ͷ�ļ�                          **/
/**********************************************************/

#pragma once
#include"naive_implementation/data_structure/tool_functions.h"


/**
@brief: ����cnf�ļ�ע����, ��ȡ������Ϣ
@param fp: cnf�ļ�ָ��
*/
void skip(FILE* fp)
{
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;		//����"p"��˵����ȡ��������Ϣ��
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);		//��ȡ������Ϣ
}


/**
@brief: ��cnf�ļ��ж�ȡ���ݹ�����ʽ
@param filename: cnf�ļ���
@return: ������ɵĹ�ʽ
*/
struct Formula* loadFile(const char* filename)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("�޷���ȡ�ļ�:%s", filename);
		exit(1);
	}
	else {
		int num=0, i = 1;
		struct Formula* formula = initFormula();		//��ʼ����ʽ
		skip(fp);	//�����ļ�ע����

		struct Clause* clause = createClause(formula);	//�ر���:���ӵ�һ���Ӿ�
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i < info.num_clause) {		//������β���½�һ���Ӿ�
				clause = createClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//����ǰ�Ӿ�����һ������
				addLiteral(clause, num);
			}
			else break;		//��ȡ�������
		}

		fclose(fp);
		return formula;
	}
}


/**
@brief: ��cnf�ļ��ж�ȡ���ݹ�����ʽ(�Ľ���1)
@param filename: cnf�ļ���
@param counter: ָ������������ָ��
@return: ������ɵĹ�ʽ
*/
struct Formula* loadFile_opti1(const char* filename, struct Counter** counter)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("�޷���ȡ�ļ�:%s", filename);
		exit(1);
	}
	else {
		int num = 0, i = 1;
		struct Formula* formula = initFormula();		//��ʼ����ʽ
		skip(fp);	//�����ļ�ע����
		
		*counter = initCounter();
		struct Clause* clause = createClause(formula);	//�ر���:���ӵ�һ���Ӿ�
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i < info.num_clause) {		//������β���½�һ���Ӿ�
				clause = createClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//����ǰ�Ӿ�����һ������
				addLiteral(clause, num);
				(num > 0) ? ((*counter)[abs(num)].positive++) : ((*counter)[abs(num)].negative++);
			}
			else break;		//��ȡ�������
		}

		fclose(fp);
		return formula;
	}
}