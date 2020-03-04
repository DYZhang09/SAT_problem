/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: ����ģ����غ���ͷ�ļ�	                      
/**********************************************************/

#pragma once
#include"../generator/puzzle.h"

/**
@brief: ��ӡ����ָ��
*/
void printPlayGuide()
{
	printf("/*�밴���¸�ʽ������: ��(�ո�) ��(�ո�) 0/1/-1(�������, ֱ�ӷų����)\n");
}


/**
@brief: �������Ϸ���
@param input: ���������������
@return: �Ϸ��򷵻�true
*/
bool checkInput(int* input)
{
	if (input[0] > 0 and input[0] <= puzzle_size
		and input[1] > 0 and input[1] <= puzzle_size
		and (input[2] == 0 or input[2] == 1 or input[2] == -1)) {
		return true;
	}
	return false;
}


/**
@brief: ����Ƿ����̽���Ƿ���ȷ
@param p: ����
@return: ��ȷ�򷵻�true
*/
bool checkValid(struct Puzzle p, struct Formula* formula)
{
	short result[puzzle_size * puzzle_size + 1];
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			int data = i * puzzle_size + j + 1;
			if (p.puzzle[i][j] == -1) 
				result[data] = 0;
			else 
				result[data] = (p.puzzle[i][j] > 0) ? 1 : -1;
		}
	}
	bool res = evaluateFormula(formula, result);
	return res;
}


/**
@brief: ��������Ƿ��п�λ
@param p: ����
@return: ���п�λ�򷵻�true
*/
bool hasVacant(struct Puzzle p)
{
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			if (1 - p.puzzle[i][j] > 1) return true;
	return false;
}


/**
@brief: ��ȡ�û����벢�жϺϷ���
@return: �Ϸ����û�����
*/
int* getPlayInput()
{
	static int input[3] = { -1, -1, -1 };
	printPlayGuide();
	scanf("%d %d %d", input, input + 1, input + 2);
	if (!checkInput(input)) {
		printf("���������Ƿ����\n");
		return getPlayInput();
	}
	return input;
}


/**
@brief: ���û�����Ӧ�õ�����
@param p: ָ�����̵�ָ��
@param input: �û�������
@return: �û�����д��ֻ��λ��ʱ����false
*/
bool applyInput(struct Puzzle* p, int* input)
{
	int row = input[0], col = input[1];
	if (p->mask[row - 1][col - 1]) {
		p->puzzle[row - 1][col - 1] = input[2];
		return true;
	}
	else {
		printf("/*�벻Ҫ�����޸���ĿŶ\n");
		return false;
	}
}


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
	printf("/*����:\n");
	drawLineTerminal();
	for (int i = 0; i < puzzle_size; i++) {
		for (int j = 0; j < puzzle_size; j++) {
			if (p.puzzle[i][j] > -1) printf("| %d ", p.puzzle[i][j]);
			else printf("|   ");
		}
		printf("|\n");
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
	std::string year, month, day, hour, minute, time_str;
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
	return ".\\puzzleCnfFile\\" + getTimeStr() + ".cnf";
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