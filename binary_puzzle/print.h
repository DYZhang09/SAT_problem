/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: ���̴�ӡ���ͷ�ļ�		                          **/
/**********************************************************/

#pragma once
#include"generate.h"

/**
@brief: �ն����һ�Ŷ���
*/
void drawLineTerminal()
{
	for (int i = 0; i < 4 * puzzle_size + 1; i++)
		printf("-");
	printf("\n");
}


/**
@brief: ���ļ������һ�Ŷ���
@param fp: ָ������ļ����ļ�ָ��
*/
void drawLineFile(FILE* fp)
{
	for (int i = 0; i < 21 * puzzle_size + 1; i++)
		fprintf(fp, "%s", "-");
	fprintf(fp, "%s", "\n");
}


/**
@brief: ��Ŀ������������ն�
@param p: �����������
*/
void drawTerminal(struct Puzzle p)
{
	printf("Answer: \n");
	drawLineTerminal();
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			if (p.puzzle[i][j] > -1) printf("| %d ", p.puzzle[i][j]);
			else printf("|    ");
		}
		printf(" |\n");
		drawLineTerminal();
	}
}


/**
@brief: ��Ŀ������������ļ�
@param p: �����������
@param filename: ����ļ�·����
*/
void drawFile(struct Puzzle p, const char* filename)
{
	FILE* fp = fopen(filename, "w+");
	if (!fp) {
		printf("�޷����ļ�: %s\n", filename);
		return;
	}
	else {
		fprintf(fp, "\n%s\n", "Answer:");
		drawLineFile(fp);
		for (int i = 0; i < puzzle_size; i++) {
			for (int j = 0; j < puzzle_size; j++) {
				if (p.puzzle[i][j] > -1) fprintf(fp, "|\t%d\t", p.puzzle[i][j]);
			}
			fprintf(fp, "%s", "\t|\n");
			drawLineFile(fp);
		}
		fclose(fp);
	}
}


/**
@brief: ��������ܽӿ�
@param p: �����������
@param filename: ����ļ���·��������ΪNULL���ʾ������ն�
*/
void draw(struct Puzzle p, const char* filename = NULL)
{
	if (!filename) drawTerminal(p);
	else drawFile(p, filename);
}


/**
@brief: ��ȡ��ǰʱ�䲢ת��Ϊ�ַ���
@return: ��ǰʱ���ַ���
*/
string getTimeStr()
{
	std:: string year, month, day, hour, minute, time_str;
	time_t now_time;
	time(&now_time);
	struct tm* tm_now = localtime(&now_time);		//��ȡϵͳ��ǰʱ��
	year = to_string(tm_now->tm_year + 1900);		//ʱ��ת��Ϊ�ַ���
	month = to_string(tm_now->tm_mon + 1);
	day = to_string(tm_now->tm_mday);
	hour = to_string(tm_now->tm_hour);
	minute = to_string(tm_now->tm_min);
	time_str = year + "-" + month + "-" + day + "-" + hour + "-" + minute;
	return time_str;
}


/**
@brief: ��ȡ���CNF�ļ���·��
@return: CNF�ļ�·��
*/
string getNameOfCnfFile()
{
	return ".\\puzzleCnfFile\\" + getTimeStr() + ".puzzle";
}


/**
@brief: ��CNF��ʽд���ļ���
@param formula: ָ����Ҫд���ļ��Ĺ�ʽ��ָ��
*/
void printCnfIntoFile(struct Formula* formula)
{
	string file = getNameOfCnfFile();
	const char* filename = file.c_str();		//��ȡĿ���ļ�·��
	FILE* fp = fopen(filename, "w");			
	if (!fp) {
		printf("/*�޷�д���ļ�: %s\n", filename);
		exit(1);
	}
	else {
		fprintf(fp, "%s %s %d %d\n", "p", "cnf", puzzle_size * puzzle_size, formula->num_clause);		//��ӡ������Ϣ
		struct Clause* curr_clause = formula->head->nextClause;		//������ʽ�������
		while (!curr_clause->isLastClause) {
			struct Literal* curr_literal = curr_clause->head->nextLiteral;
			while (!curr_literal->isTail) {
				fprintf(fp, "%d ", curr_literal->data);
				curr_literal = curr_literal->nextLiteral;
			}
			fprintf(fp, "%d\n", 0);
			curr_clause = curr_clause->nextClause;
		}
		printf("/*���̶�ӦCNF��ʽ��д���ļ�: %s\n", filename);
		fclose(fp);
	}
}