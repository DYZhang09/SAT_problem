/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: cnf�ļ���ȡ������������                         
/**********************************************************/

#pragma once
#include"data_struct.h"
#include"tools.h"

#include"../naive_dpll/cnfparser/cnfparser.h"


/**
@brief: ��ȡcnf�ļ���ù�ʽ, ��ʼ��mask�������
@param filename: �ļ�·����
@param formula: ��ʽ�����λ��
@param mask: ָ��mask��ָ��
@param p_counter: ָ���������ָ��
@return: ��ȡ�ļ��ɹ�����true
*/
bool loadFile(const char* filename, struct BinVector* formula, struct Mask* mask, int** p_counter)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*�޷����ļ�: %s\n", filename);
		return false;
	}
	else {
		int num = 0, i = 1;
		skip(fp);
		*p_counter = (int*)malloc(sizeof(int) * (2 * info.num_literal + 1));
		memset(*p_counter, 0, sizeof(int) * (2 * info.num_literal + 1));

		struct Vector vec = vecInit();
		struct Vector lit_mask = vecInit();
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%hd", &num);
			if (num == 0 and i <= info.num_clause) {		//������β���½�һ���Ӿ�
				binVec_push_back(formula, vec);
				binVec_push_back(&(mask->lit_masks), lit_mask);
				vec = vecInit();
				lit_mask = vecInit();
				i++;
				continue;
			}
			else if (num != 0) {		//����ǰ�Ӿ����һ������
				vec_push_back(&vec, num);
				vec_push_back(&lit_mask, 0);
				(*p_counter)[dataToIndex(num)]++;
			}
			else break;		//��ȡ�������
		}
		mask->clause_masks = vecInit(info.num_clause + 1, info.num_clause, 0);
		mask->del_lit_log = (int*)malloc(sizeof(int) * (info.num_literal + 1));
		memset(mask->del_lit_log, 0, sizeof(int) * (info.num_literal + 1));
		mask->del_clause_log = (int*)malloc(sizeof(int) * (info.num_clause));
		memset(mask->del_clause_log, 0, sizeof(int) * (info.num_clause));
		return true;
	}
}


/**
@brief: ��ȡcnf�ļ���ù�ʽ, ��ʼ�����������ֵ������
@param filename: �ļ�·����
@param formula: ��ʽ�����λ��
@param vars: ��ű�����ֵ��һάvector
@return: ��ȡ�ļ��ɹ�����true
*/
bool load_formula(const char* filename, struct BinVector* formula, struct Vector* vars)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*�޷����ļ�:%s\n", filename);
		return false;
	}
	else {
		int num = 0, i = 1;
		skip(fp);

		struct Vector buffer = vecInit();
		while (!feof(fp) && i <= info.num_clause) {
			fscanf(fp, "%hd", &num);
			if (num == 0 && i <= info.num_clause) {
				binVec_push_back(formula, buffer);
				buffer = vecInit();
				i++;
				continue;
			}
			else if (num != 0) {
				vec_push_back(&buffer, num);
			}
			else break;
		}
		*vars = vecInit(info.num_literal + 2, info.num_literal + 1, 0);
		return true;
	}
}