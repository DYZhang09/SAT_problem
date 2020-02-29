#pragma once
#include"../config/config.h"
#include"../naive_implementation/data_structure/data_struct.h"

#define DEFAULT_CAPACITY 3


struct Vector
{
	int _size, _capacity;
	int* _elem;
};


struct Vector vecInit(int c = DEFAULT_CAPACITY, int s = 0, int v = 0)
{
	struct Vector vec;
	vec._elem = (int*)malloc(sizeof(int) * (vec._capacity = c));
	for (vec._size = 0; vec._size < s; vec._elem[vec._size++] = v);
	return vec;
}


void freeVec(struct Vector* vec)
{
	free(vec->_elem);
	vec->_elem = NULL;
}


int vecSize(struct Vector vec)
{
	return vec._size;
}


bool vecEmpty(struct Vector vec)
{
	return !vec._size;
}


void __expand(struct Vector* vec)
{
	if (vec->_size < vec->_capacity) return;
	if (vec->_capacity < DEFAULT_CAPACITY) vec->_capacity = DEFAULT_CAPACITY;
	int *_more_elem = (int*)realloc(vec->_elem, sizeof(int) * (vec->_capacity <<= 1));
	if (_more_elem) vec->_elem = _more_elem;
}


int vecNth(struct Vector vec, int n)
{
	return vec._elem[n];
}


void vecNth_re(struct Vector* vec, int n, int data)
{
	vec->_elem[n] = data;
}


int vecInsert(struct Vector* vec, int r, int data)
{
	__expand(vec);
	for (int i = vec->_size; i > r; i--)
		vec->_elem[i] = vec->_elem[i - 1];
	vec->_elem[r] = data;
	vec->_size++;
	return r;
}


void vec_push_back(struct Vector* vec, int data)
{
	vecInsert(vec, vec->_size, data);
}


int vecRemove(struct Vector* vec,int low, int high)
{
	if (low == high) return 0;
	while (high < vec->_size) vec->_elem[low++] = vec->_elem[high++];
	vec->_size = low;
	return high - low;
}


int vecRemoveSingle(struct Vector* vec, int r)
{
	int data = vec->_elem[r];
	vecRemove(vec, r, r + 1);
	return data;
}


int vecSearch(struct Vector vec, int data)
{
	for (int i = 0; i < vec._size; i++)
		if (vec._elem[i] == data) return i;
	return -1;
}


void vecTraverse(struct Vector vec, void (*visit)(int data))
{
	for (int i = 0; i < vec._size; i++)
		visit(vec._elem[i]);
}


void vecPrint(struct Vector vec)
{
	for (int i = 0; i < vec._size; i++) {
		printf("%d ", vec._elem[i]);
		if ((i+1) % 10 == 0)printf("\n");
	}
	printf("\n");
}



void test_vec()
{
	struct Vector vec = vecInit();
	for (int i = 0; i < 10; i++)
		vec_push_back(&vec, i + 1);
	vecInsert(&vec, 2, 10);
	vecRemoveSingle(&vec, 4);
	printf("%d %d\n", vec._size, vec._capacity);
	printf("%d\n", vecSearch(vec, 8));
	printf("%d\n", vecNth(vec, 7));
	vecPrint(vec);
}


//--------------------------------------------------------------------------------
struct BinVector
{
	int _size, _capacity;
	struct Vector* _elem;
};


struct BinVector binVecInit(int c = DEFAULT_CAPACITY, int s = 0)
{
	struct BinVector bvec;
	bvec._elem = (struct Vector*)malloc(sizeof(struct Vector) * (bvec._capacity = c));
	for (bvec._size = 0; bvec._size < s; bvec._elem[bvec._size++] = vecInit());
	return bvec;
}


void freeBinVec(struct BinVector* bvec)
{
	for (int i = 0; i < bvec->_size; i++)
		freeVec(&bvec->_elem[i]);
	free(bvec->_elem);
	bvec->_elem = NULL;
}


int binVecSize(struct BinVector bvec)
{
	return bvec._size;
}


bool binVecEmpty(struct BinVector bvec)
{
	return !bvec._size;
}


void __expand_b(struct BinVector* bvec)
{
	if (bvec->_size < bvec->_capacity) return;
	if (bvec->_capacity < DEFAULT_CAPACITY) bvec->_capacity = DEFAULT_CAPACITY;
	struct Vector* _more_elem = (struct Vector*)realloc(bvec->_elem, sizeof(struct Vector) * (bvec->_capacity <<= 1));
	if (_more_elem) bvec->_elem = _more_elem;
}


struct Vector binVecNth(struct BinVector bvec, int n)
{
	return bvec._elem[n];
}


void binVecNth_re(struct BinVector* bvec, int n, struct Vector vec)
{
	bvec->_elem[n] = vec;
}


int binVecGrid(struct BinVector bvec, int n, int m)
{
	return bvec._elem[n]._elem[m];
}


void binVecGrid_re(struct BinVector* bvec, int n, int m, int data)
{
	bvec->_elem[n]._elem[m] = data;
}


int binVecInsert(struct BinVector* bvec, int r, struct Vector vec)
{
	__expand_b(bvec);
	for (int i = bvec->_size; i > r; i--)
		bvec->_elem[i] = bvec->_elem[i - 1];
	bvec->_elem[r] = vec;
	bvec->_size++;
	return r;
}


void binVec_push_back(struct BinVector* bvec, struct Vector vec)
{
	binVecInsert(bvec, bvec->_size, vec);
}


int binVecRemove(struct BinVector* bvec, int low, int high)
{
	if (low == high) return 0;
	while (high < bvec->_size) bvec->_elem[low++] = bvec->_elem[high++];
	bvec->_size = low;
	return high - low;
}


struct Vector binVecRemoveSingle(struct BinVector* bvec, int r)
{
	struct Vector vec = bvec->_elem[r];
	binVecRemove(bvec, r, r + 1);
	return vec;
}


void binVecPrint(struct BinVector bvec)
{
	for (int i = 0; i < bvec._size; i++)
		vecPrint(bvec._elem[i]);
}


void test_binVec()
{
	struct BinVector bvec = binVecInit();
	struct Vector vec = vecInit(3, 3, 2);
	binVec_push_back(&bvec, vec);
	binVec_push_back(&bvec, vec);
	binVecRemoveSingle(&bvec, 0);
	binVecPrint(bvec);
}


//--------------------------------------------------------------------
struct Mask
{
	struct BinVector lit_masks;
	struct Vector clause_masks;

	int* del_lit_log;
	int* del_clause_log;
};


struct Mask maskInit()
{
	struct Mask mask;
	mask.clause_masks = vecInit();
	mask.lit_masks = binVecInit();

	return mask;
}


void freeMask(struct Mask* mask)
{
	freeVec(&(mask->clause_masks));
	freeBinVec(&(mask->lit_masks));
	free(mask->del_lit_log);
	free(mask->del_clause_log);
}