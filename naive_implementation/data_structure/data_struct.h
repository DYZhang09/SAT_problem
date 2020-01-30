/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/20                                                     **/
/** Description: CNF��ʽ�洢���ݽṹ����					  **/
/**********************************************************/

#pragma once
#include"../../config/config.h"

//------------------------naive data structure---------------------//
struct Literal			//���ֽṹ�壬data��Ӧ�������
{
	int data = 0;
	struct Literal* nextLiteral = NULL;
	struct Literal* beforeLiteral = NULL;
	bool isHead = false;
	bool isTail = false;
};

struct Clause		//�Ӿ�ṹ��
{
	struct Literal* head;
	struct Literal* tail;
	struct Clause* nextClause = NULL;
	struct Clause* beforeClause = NULL;

	bool isFirstClause = false;
	bool isLastClause = false;
	int len = 0;
};

struct Formula		//��ʽ�ṹ��
{
	struct Clause* head;
	struct Clause* tail;

	int num_clause = 0;
};
