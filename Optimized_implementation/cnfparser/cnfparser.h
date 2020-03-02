/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/01                                                     
//* Description: cnf�����Ż�ģ�����ͷ�ļ�		                          
/**********************************************************/

#pragma once
#include"../solver/tool_functions.h"
#include"../../naive_dpll/cnfparser/cnfparser.h"


/**
@brief: ��cnf�ļ��ж�ȡ���ݹ�����ʽ(�Ľ���1)
@param filename: cnf�ļ���
@param counter: ָ������������ָ��
@return: ������ɵĹ�ʽ
*/
struct Formula* loadFile_opti1(const char* filename, int** counter)
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

		struct Clause* clause = createClause(formula);	//�ر���:��ӵ�һ���Ӿ�
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i < info.num_clause) {		//������β���½�һ���Ӿ�
				clause = createClause(formula);
				i++;
				continue;
			}
			else if (num != 0) {		//����ǰ�Ӿ����һ������
				addLiteral(clause, num);
				(*counter)[counterIndex(num)] ++;
			}
			else break;		//��ȡ�������
		}

		fclose(fp);
		return formula;
	}
}