#pragma once
#include"data_struct.h"


void skipHead(FILE* fp)
{
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;		//����"p"��˵����ȡ��������Ϣ��
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);		//��ȡ������Ϣ
}


bool loadFile(const char* filename, struct BinVector* bvec, struct Mask* mask)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*�޷����ļ�: %s\n", filename);
		return false;
	}
	else {
		int num = 0, i = 1;
		skipHead(fp);
		
		struct Vector vec = vecInit();
		struct Vector lit_mask = vecInit();
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i <= info.num_clause) {		//������β���½�һ���Ӿ�
				binVec_push_back(bvec, vec);
				binVec_push_back(&(mask->lit_masks), lit_mask);
				vec = vecInit();
				lit_mask = vecInit();
				i++;
				continue;
			}
			else if (num != 0) {		//����ǰ�Ӿ����һ������
				vec_push_back(&vec, num);
				vec_push_back(&lit_mask, 0);
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


bool load_formula(const char* filename, struct BinVector* formula, struct Vector* vars)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*�޷����ļ�:%s\n", filename);
		return false;
	}
	else {
		int num = 0, i = 1;
		skipHead(fp);

		struct Vector buffer = vecInit();
		while (!feof(fp) && i <= info.num_clause) {
			fscanf(fp, "%d", &num);
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
		*vars = vecInit(info.num_literal + 2, info.num_literal + 1, -1);
		return true;
	}
}