#pragma once
#include"data_struct.h"


void skipHead(FILE* fp)
{
	char c[30];
	while (true) {
		fscanf(fp, "%s", c);
		if (strcmp(c, "p") == 0) break;		//遇到"p"，说明读取到总体信息行
	}
	fscanf(fp, "%s", c);
	fscanf(fp, "%d %d", &info.num_literal, &info.num_clause);		//读取总体信息
}


bool loadFile(const char* filename, struct BinVector* bvec, struct Mask* mask)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*无法打开文件: %s\n", filename);
		return false;
	}
	else {
		int num = 0, i = 1;
		skipHead(fp);
		
		struct Vector vec = vecInit();
		struct Vector lit_mask = vecInit();
		while (!feof(fp) and (i <= info.num_clause)) {
			fscanf(fp, "%d", &num);
			if (num == 0 and i <= info.num_clause) {		//遇到行尾，新建一个子句
				binVec_push_back(bvec, vec);
				binVec_push_back(&(mask->lit_masks), lit_mask);
				vec = vecInit();
				lit_mask = vecInit();
				i++;
				continue;
			}
			else if (num != 0) {		//给当前子句添加一个文字
				vec_push_back(&vec, num);
				vec_push_back(&lit_mask, 0);
			}
			else break;		//读取输入完毕
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
		printf("/*无法打开文件:%s\n", filename);
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