/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/25                                                     **/
/** Description: 棋盘打印相关头文件		                          **/
/**********************************************************/

#pragma once
#include"generate.h"

/**
@brief: 终端输出一排短线
*/
void drawLineTerminal()
{
	for (int i = 0; i < 4 * puzzle_size + 1; i++)
		printf("-");
	printf("\n");
}


/**
@brief: 在文件中输出一排短线
@param fp: 指向输出文件的文件指针
*/
void drawLineFile(FILE* fp)
{
	for (int i = 0; i < 21 * puzzle_size + 1; i++)
		fprintf(fp, "%s", "-");
	fprintf(fp, "%s", "\n");
}


/**
@brief: 将目标棋盘输出至终端
@param p: 待输出的棋盘
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
@brief: 将目标棋盘输出至文件
@param p: 待输出的棋盘
@param filename: 输出文件路径名
*/
void drawFile(struct Puzzle p, const char* filename)
{
	FILE* fp = fopen(filename, "w+");
	if (!fp) {
		printf("无法打开文件: %s\n", filename);
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
@brief: 棋盘输出总接口
@param p: 待输出的棋盘
@param filename: 输出文件的路径名，若为NULL则表示输出至终端
*/
void draw(struct Puzzle p, const char* filename = NULL)
{
	if (!filename) drawTerminal(p);
	else drawFile(p, filename);
}


/**
@brief: 获取当前时间并转换为字符串
@return: 当前时间字符串
*/
string getTimeStr()
{
	std:: string year, month, day, hour, minute, time_str;
	time_t now_time;
	time(&now_time);
	struct tm* tm_now = localtime(&now_time);		//获取系统当前时间
	year = to_string(tm_now->tm_year + 1900);		//时间转换为字符串
	month = to_string(tm_now->tm_mon + 1);
	day = to_string(tm_now->tm_mday);
	hour = to_string(tm_now->tm_hour);
	minute = to_string(tm_now->tm_min);
	time_str = year + "-" + month + "-" + day + "-" + hour + "-" + minute;
	return time_str;
}


/**
@brief: 获取存放CNF文件的路径
@return: CNF文件路径
*/
string getNameOfCnfFile()
{
	return ".\\puzzleCnfFile\\" + getTimeStr() + ".puzzle";
}


/**
@brief: 将CNF公式写入文件中
@param formula: 指向需要写入文件的公式的指针
*/
void printCnfIntoFile(struct Formula* formula)
{
	string file = getNameOfCnfFile();
	const char* filename = file.c_str();		//获取目的文件路径
	FILE* fp = fopen(filename, "w");			
	if (!fp) {
		printf("/*无法写入文件: %s\n", filename);
		exit(1);
	}
	else {
		fprintf(fp, "%s %s %d %d\n", "p", "cnf", puzzle_size * puzzle_size, formula->num_clause);		//打印总体信息
		struct Clause* curr_clause = formula->head->nextClause;		//遍历公式进行输出
		while (!curr_clause->isLastClause) {
			struct Literal* curr_literal = curr_clause->head->nextLiteral;
			while (!curr_literal->isTail) {
				fprintf(fp, "%d ", curr_literal->data);
				curr_literal = curr_literal->nextLiteral;
			}
			fprintf(fp, "%d\n", 0);
			curr_clause = curr_clause->nextClause;
		}
		printf("/*棋盘对应CNF范式已写入文件: %s\n", filename);
		fclose(fp);
	}
}