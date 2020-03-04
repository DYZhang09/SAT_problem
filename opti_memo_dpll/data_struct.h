/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/02/10                                                     
//* Description: ���ݽṹ����غ�������ͷ�ļ�		                          
/**********************************************************/


#pragma once
#include"../config/config.h"
#include"../naive_dpll/data_structure/data_struct.h"

#define DEFAULT_CAPACITY 3


//һάvector�Ľṹ��
struct Vector
{
	int _size, _capacity;
	short* _elem;
};


/**
@brief: ��ʼ��һ��vector
@param c: vector�ĳ�ʼ����, Ĭ��ΪDEFAULT_CAPACITY
@param s: vector�ĳ�ʼ��С, Ĭ��Ϊ0
@param v: ÿ��Ԫ�صĳ�ʼֵ, Ĭ��Ϊ0
@return: ��ʼ�����һάvector
*/
struct Vector vecInit(int c = DEFAULT_CAPACITY, int s = 0, short v = 0)
{
	struct Vector vec;
	vec._elem = (short*)malloc(sizeof(short) * (vec._capacity = c));
	for (vec._size = 0; vec._size < s; vec._elem[vec._size++] = v);
	return vec;
}


/**
@brief: ����һ��vector
@param vec: ָ������vector��ָ��
*/
void freeVec(struct Vector* vec)
{
	free(vec->_elem);
	vec->_elem = NULL;
}


//brief:����vector�Ĵ�С
int vecSize(struct Vector vec)
{
	return vec._size;
}


/**
@brief: ����vector(����push_back������ʹ��)
@param vec: �����ݵ�vector
*/
void __expand(struct Vector* vec)
{
	if (vec->_size < vec->_capacity) return;		//�������ݼ��˳�����
	if (vec->_capacity < DEFAULT_CAPACITY) vec->_capacity = DEFAULT_CAPACITY;
	short*_more_elem = (short*)realloc(vec->_elem, sizeof(short) * (vec->_capacity <<= 1));
	if (_more_elem) vec->_elem = _more_elem;
}


/**
@brief: ����vector�е��±�Ϊn��Ԫ��
@param vec: vector
@param n: Ԫ�����ڵ��±�
@return: �±�Ϊn��Ԫ��
*/
short vecNth(struct Vector vec, int n)
{
	return vec._elem[n];
}


/**
@brief: ����vector���±�Ϊn��Ԫ�ص�ֵ
@param vec: ָ���޸ĵ�vector��ָ��
@param n: ���޸�Ԫ�ص��±�
@param data: �滻���ֵ
*/
void vecNth_re(struct Vector* vec, int n, short data)
{
	vec->_elem[n] = data;
}


/**
@brief: ��vector�в���Ԫ��
@param vec: ָ���޸�vector��ָ��
@param r: ����λ�õ��±�(����Ԫ�ط���r֮ǰ)
@param data: ��Ҫ�����ֵ
@return: ����λ�õ��±�
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
@brief: ��vector��ĩβѹ��Ԫ��
@param vec: ָ���޸�vector��ָ��
@param data: �������Ԫ��
*/
void vec_push_back(struct Vector* vec, short data)
{
	vecInsert(vec, vec->_size, data);
}


/**
@brief: ��vector������ָ����Ԫ��, ������λ��
@param vec: ��������vector
@param data: ��Ҫ������Ԫ��
@return: �����������ĵ�һ��λ���±�, ��û���������򷵻�-1
*/
int vecSearch(struct Vector vec, short data)
{
	for (int i = 0; i < vec._size; i++)
		if (vec._elem[i] == data) return i;
	return -1;
}


/**
@brief: ��ӡvector
@param vec: ����ӡ��vector
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
//��άvector�Ľṹ��
struct BinVector
{
	int _size, _capacity;
	struct Vector* _elem;
};


/**
@brief: ��ʼ��һ����άvector
@param c: ��άvector��һά����
@param s: ��άvector��һά��С
@return: ���س�ʼ����Ķ�άvector
*/
struct BinVector binVecInit(int c = DEFAULT_CAPACITY, int s = 0)
{
	struct BinVector bvec;
	bvec._elem = (struct Vector*)malloc(sizeof(struct Vector) * (bvec._capacity = c));
	for (bvec._size = 0; bvec._size < s; bvec._elem[bvec._size++] = vecInit());
	return bvec;
}


/**
@brief: ���ٶ�άvector
@param bvec: ָ�����ٶ�άvector��ָ��
*/
void freeBinVec(struct BinVector* bvec)
{
	for (int i = 0; i < bvec->_size; i++)
		freeVec(&bvec->_elem[i]);
	free(bvec->_elem);
	bvec->_elem = NULL;
}


/**
@brief: ���ض�άvector�ĵ�һά��С
*/
int binVecSize(struct BinVector bvec)
{
	return bvec._size;
}


/**
@brief: ���ݶ�άvector(������һάvector������)
*/
void __expand_b(struct BinVector* bvec)
{
	if (bvec->_size < bvec->_capacity) return;
	if (bvec->_capacity < DEFAULT_CAPACITY) bvec->_capacity = DEFAULT_CAPACITY;
	struct Vector* _more_elem = (struct Vector*)realloc(bvec->_elem, sizeof(struct Vector) * (bvec->_capacity <<= 1));
	if (_more_elem) bvec->_elem = _more_elem;
}


/**
@brief: ���ض�άvector��һά�±�Ϊn��Ԫ��
@param bvec: ��άvector
@param n: ��һά�±�n
@return: ��һά�±�n��Ӧ��һάvector
*/
struct Vector binVecNth(struct BinVector bvec, int n)
{
	return bvec._elem[n];
}


/**
@brief: �滻��άvector��һά�±�Ϊn��Ԫ��
@param bvec: ָ���޸�vector��ָ��
@param n: ��һά�±�n
@param vec: �����滻��һάvector
*/
void binVecNth_re(struct BinVector* bvec, int n, struct Vector vec)
{
	bvec->_elem[n] = vec;
}


/**
@brief: �����±�Ϊ(n,m) ��Ԫ��
@param bvec: ��άvector
@param n: ��һά�±�
@param m: �ڶ�ά�±�
@return: �±�ά(n,m)��Ԫ��
*/
short binVecGrid(struct BinVector bvec, int n, int m)
{
	return bvec._elem[n]._elem[m];
}


/**
@brief: �����±�Ϊ(n,m)��Ԫ��ֵ
@param bvec: ָ���޸ĵĶ�άvector��ָ��
@param n: ��һά�±�
@param m: �ڶ�ά�±�
@param data: �����滻��ֵ
*/
void binVecGrid_re(struct BinVector* bvec, int n, int m, short data)
{
	bvec->_elem[n]._elem[m] = data;
}


/**
@brief: ���άvector�в���һάvector
@param bvec: ָ���޸ĵĶ�άvector��ָ��
@param r: ����λ���±�(����ԭ��ӦԪ��֮ǰ)
@param vec: �������һάvector
@return: ����λ���±�
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
@brief: ���άvectorĩβѹ��һάvector
*/
void binVec_push_back(struct BinVector* bvec, struct Vector vec)
{
	binVecInsert(bvec, bvec->_size, vec);
}


/**
@brief: ��ӡ��άvector��Ԫ��
*/
void binVecPrint(struct BinVector bvec)
{
	for (int i = 0; i < bvec._size; i++)
		vecPrint(bvec._elem[i]);
}


//--------------------------------------------------------------------
//ɾ�����Mask�ṹ��
struct Mask
{
	struct BinVector lit_masks;
	struct Vector clause_masks;

	int* del_lit_log;
	int* del_clause_log;
};


//@brief: ��ʼ��һ��mask
struct Mask maskInit()
{
	struct Mask mask;
	mask.clause_masks = vecInit();
	mask.lit_masks = binVecInit();

	return mask;
}


//@brief: ����һ��mask
void freeMask(struct Mask* mask)
{
	freeVec(&(mask->clause_masks));
	freeBinVec(&(mask->lit_masks));
	free(mask->del_lit_log);
	free(mask->del_clause_log);
}