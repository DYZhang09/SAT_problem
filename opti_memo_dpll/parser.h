/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: cnf文件读取解析函数定义                         
/**********************************************************/

#pragma once
#include"data_struct.h"
#include"tools.h"

#include"../naive_dpll/cnfparser/cnfparser.h"


/**
@brief: 读取cnf文件获得公式, 初始化mask与计数器
@param filename: 文件路径名
@param formula: 公式保存的位置
@param mask: 指向mask的指针
@param p_counter: 指向计数器的指针
@return: 读取文件成功返回true
*/
bool loadFile(const char* filename, struct BinVector* formula, struct Mask* mask, int** p_counter)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*无法打开文件: %s\n", filename);
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
			if (num == 0 and i <= info.num_clause) {		//遇到行尾，新建一个子句
				binVec_push_back(formula, vec);
				binVec_push_back(&(mask->lit_masks), lit_mask);
				vec = vecInit();
				lit_mask = vecInit();
				i++;
				continue;
			}
			else if (num != 0) {		//给当前子句添加一个文字
				vec_push_back(&vec, num);
				vec_push_back(&lit_mask, 0);
				(*p_counter)[dataToIndex(num)]++;
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


/**
@brief: 读取cnf文件获得公式, 初始化保存变量赋值的数组
@param filename: 文件路径名
@param formula: 公式保存的位置
@param vars: 存放变量赋值的一维vector
@return: 读取文件成功返回true
*/
bool load_formula(const char* filename, struct BinVector* formula, struct Vector* vars)
{
	FILE* fp = fopen(filename, "r");
	if (!fp) {
		printf("/*无法打开文件:%s\n", filename);
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