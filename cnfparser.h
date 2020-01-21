/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: cnf�ļ�����ģ��ͷ�ļ�                          **/
/**********************************************************/

#pragma once
#include"data_struct.h"

struct ProblemInfo
{
	int num_clause;
	int num_literal;
}info;


//-------------------------functions for debug-----------------------------------//
/**
@brief: ��ӡ��ʽ
@param formula: ָ��ʽ��ָ��
*/
void printFormula(struct Formula* formula)
{
	int i = 1;
	struct Clause* currClause = formula->head->nextClause;
	while (!currClause->isLastClause) {
		struct Literal* currLit = currClause->head->nextLiteral;
		printf("Formula %d:\n", i++);
		while (!currLit->isTail) {
			printf("%d ", currLit->data);
			currLit = currLit->nextLiteral;
		}
		std::cout << currClause->len << '\n';
		currClause = currClause->nextClause;
	}
}


/**
@brief: ��ӡ���������
@param res: �������
*/
void printArray(int* res)
{
	for (int i = 1; i <= info.num_literal; i++) {
		printf("%d ", res[i]);
		if (i % 10 == 0) printf("\n");
	}
}


//-----------------------------functions for cnfparser-----------------------------//
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

		#ifdef DEBUG
			printFormula(formula);
		#endif // DEBUG

		return formula;
	}
}

