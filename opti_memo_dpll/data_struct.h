/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: 数据结构及相关函数定义头文件		                          
/**********************************************************/


#pragma once
#include"../config/config.h"
#include"../naive_dpll/data_structure/data_struct.h"

#define DEFAULT_CAPACITY 3


//一维vector的结构体
struct Vector
{
	int _size, _capacity;
	short* _elem;
};


/**
@brief: 初始化一个vector
@param c: vector的初始容量, 默认为DEFAULT_CAPACITY
@param s: vector的初始大小, 默认为0
@param v: 每个元素的初始值, 默认为0
@return: 初始化后的一维vector
*/
struct Vector vecInit(int c = DEFAULT_CAPACITY, int s = 0, short v = 0)
{
	struct Vector vec;
	vec._elem = (short*)malloc(sizeof(short) * (vec._capacity = c));
	for (vec._size = 0; vec._size < s; vec._elem[vec._size++] = v);
	return vec;
}


/**
@brief: 销毁一个vector
@param vec: 指向被销毁vector的指针
*/
void freeVec(struct Vector* vec)
{
	free(vec->_elem);
	vec->_elem = NULL;
}


//brief:返回vector的大小
int vecSize(struct Vector vec)
{
	return vec._size;
}


/**
@brief: 扩容vector(仅在push_back函数中使用)
@param vec: 待扩容的vector
*/
void __expand(struct Vector* vec)
{
	if (vec->_size < vec->_capacity) return;		//无需扩容即退出函数
	if (vec->_capacity < DEFAULT_CAPACITY) vec->_capacity = DEFAULT_CAPACITY;
	short*_more_elem = (short*)realloc(vec->_elem, sizeof(short) * (vec->_capacity <<= 1));
	if (_more_elem) vec->_elem = _more_elem;
}


/**
@brief: 返回vector中的下标为n的元素
@param vec: vector
@param n: 元素所在的下标
@return: 下标为n的元素
*/
short vecNth(struct Vector vec, int n)
{
	return vec._elem[n];
}


/**
@brief: 更改vector中下标为n的元素的值
@param vec: 指向被修改的vector的指针
@param n: 被修改元素的下标
@param data: 替换后的值
*/
void vecNth_re(struct Vector* vec, int n, short data)
{
	vec->_elem[n] = data;
}


/**
@brief: 向vector中插入元素
@param vec: 指向被修改vector的指针
@param r: 插入位置的下标(插入元素放在r之前)
@param data: 需要插入的值
@return: 插入位置的下标
*/
int vecInsert(struct Vector* vec, int r, short data)
{
	__expand(vec);
	for (int i = vec->_size; i > r; i--)
		vec->_elem[i] = vec->_elem[i - 1];
	vec->_elem[r] = data;
	vec->_size++;
	return r;
}


/**
@brief: 向vector的末尾压入元素
@param vec: 指向被修改vector的指针
@param data: 被插入的元素
*/
void vec_push_back(struct Vector* vec, short data)
{
	vecInsert(vec, vec->_size, data);
}


/**
@brief: 在vector中搜索指定的元素, 返回其位置
@param vec: 被搜索的vector
@param data: 需要搜索的元素
@return: 返回搜索到的第一个位置下标, 若没有搜索到则返回-1
*/
int vecSearch(struct Vector vec, short data)
{
	for (int i = 0; i < vec._size; i++)
		if (vec._elem[i] == data) return i;
	return -1;
}


/**
@brief: 打印vector
@param vec: 被打印的vector
*/
void vecPrint(struct Vector vec)
{
	for (int i = 0; i < vec._size; i++) {
		printf("%d ", vec._elem[i]);
		if ((i+1) % 10 == 0)printf("\n");
	}
	printf("\n");
}


//--------------------------------------------------------------------------------
//二维vector的结构体
struct BinVector
{
	int _size, _capacity;
	struct Vector* _elem;
};


/**
@brief: 初始化一个二维vector
@param c: 二维vector的一维容量
@param s: 二维vector的一维大小
@return: 返回初始化后的二维vector
*/
struct BinVector binVecInit(int c = DEFAULT_CAPACITY, int s = 0)
{
	struct BinVector bvec;
	bvec._elem = (struct Vector*)malloc(sizeof(struct Vector) * (bvec._capacity = c));
	for (bvec._size = 0; bvec._size < s; bvec._elem[bvec._size++] = vecInit());
	return bvec;
}


/**
@brief: 销毁二维vector
@param bvec: 指向被销毁二维vector的指针
*/
void freeBinVec(struct BinVector* bvec)
{
	for (int i = 0; i < bvec->_size; i++)
		freeVec(&bvec->_elem[i]);
	free(bvec->_elem);
	bvec->_elem = NULL;
}


/**
@brief: 返回二维vector的第一维大小
*/
int binVecSize(struct BinVector bvec)
{
	return bvec._size;
}


/**
@brief: 扩容二维vector(类似于一维vector的扩容)
*/
void __expand_b(struct BinVector* bvec)
{
	if (bvec->_size < bvec->_capacity) return;
	if (bvec->_capacity < DEFAULT_CAPACITY) bvec->_capacity = DEFAULT_CAPACITY;
	struct Vector* _more_elem = (struct Vector*)realloc(bvec->_elem, sizeof(struct Vector) * (bvec->_capacity <<= 1));
	if (_more_elem) bvec->_elem = _more_elem;
}


/**
@brief: 返回二维vector第一维下标为n的元素
@param bvec: 二维vector
@param n: 第一维下标n
@return: 第一维下标n对应的一维vector
*/
struct Vector binVecNth(struct BinVector bvec, int n)
{
	return bvec._elem[n];
}


/**
@brief: 替换二维vector第一维下标为n的元素
@param bvec: 指向被修改vector的指针
@param n: 第一维下标n
@param vec: 用来替换的一维vector
*/
void binVecNth_re(struct BinVector* bvec, int n, struct Vector vec)
{
	bvec->_elem[n] = vec;
}


/**
@brief: 返回下标为(n,m) 的元素
@param bvec: 二维vector
@param n: 第一维下标
@param m: 第二维下标
@return: 下标维(n,m)的元素
*/
short binVecGrid(struct BinVector bvec, int n, int m)
{
	return bvec._elem[n]._elem[m];
}


/**
@brief: 更改下标为(n,m)的元素值
@param bvec: 指向被修改的二维vector的指针
@param n: 第一维下标
@param m: 第二维下标
@param data: 用来替换的值
*/
void binVecGrid_re(struct BinVector* bvec, int n, int m, short data)
{
	bvec->_elem[n]._elem[m] = data;
}


/**
@brief: 向二维vector中插入一维vector
@param bvec: 指向被修改的二维vector的指针
@param r: 插入位置下标(插入原对应元素之前)
@param vec: 被插入的一维vector
@return: 插入位置下标
*/
int binVecInsert(struct BinVector* bvec, int r, struct Vector vec)
{
	__expand_b(bvec);
	for (int i = bvec->_size; i > r; i--)
		bvec->_elem[i] = bvec->_elem[i - 1];
	bvec->_elem[r] = vec;
	bvec->_size++;
	return r;
}


/**
@brief: 向二维vector末尾压入一维vector
*/
void binVec_push_back(struct BinVector* bvec, struct Vector vec)
{
	binVecInsert(bvec, bvec->_size, vec);
}


/**
@brief: 打印二维vector的元素
*/
void binVecPrint(struct BinVector bvec)
{
	for (int i = 0; i < bvec._size; i++)
		vecPrint(bvec._elem[i]);
}


//--------------------------------------------------------------------
//删除标记Mask结构体
struct Mask
{
	struct BinVector lit_masks;
	struct Vector clause_masks;

	int* del_lit_log;
	int* del_clause_log;
};


//@brief: 初始化一个mask
struct Mask maskInit()
{
	struct Mask mask;
	mask.clause_masks = vecInit();
	mask.lit_masks = binVecInit();

	return mask;
}


//@brief: 销毁一个mask
void freeMask(struct Mask* mask)
{
	freeVec(&(mask->clause_masks));
	freeBinVec(&(mask->lit_masks));
	free(mask->del_lit_log);
	free(mask->del_clause_log);
}