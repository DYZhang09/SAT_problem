/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/25                                                     
//* Description: 数独模块相关函数头文件	                      
/**********************************************************/

#pragma once
#include"../generator/puzzle.h"

/**
@brief: 打印输入指南
*/
void printPlayGuide()
{
	printf("/*请按以下格式输入解答: 行(空格) 列(空格) 0/1/-1(放弃解答, 直接放出结果)\n");
}


/**
@brief: 检查输入合法性
@param input: 保存了输入的数组
@return: 合法则返回true
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
@brief: 检查是否棋盘结果是否正确
@param p: 棋盘
@return: 正确则返回true
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
@brief: 检查棋盘是否还有空位
@param p: 棋盘
@return: 仍有空位则返回true
*/
bool hasVacant(struct Puzzle p)
{
	for (int i = 0; i < puzzle_size; i++)
		for (int j = 0; j < puzzle_size; j++)
			if (1 - p.puzzle[i][j] > 1) return true;
	return false;
}


/**
@brief: 获取用户输入并判断合法性
@return: 合法的用户输入
*/
int* getPlayInput()
{
	static int input[3] = { -1, -1, -1 };
	printPlayGuide();
	scanf("%d %d %d", input, input + 1, input + 2);
	if (!checkInput(input)) {
		printf("请检查输入是否符合\n");
		return getPlayInput();
	}
	return input;
}


/**
@brief: 将用户输入应用到棋盘
@param p: 指向棋盘的指针
@param input: 用户的输入
@return: 用户尝试写入只读位置时返回false
*/
bool applyInput(struct Puzzle* p, int* input)
{
	int row = input[0], col = input[1];
	if (p->mask[row - 1][col - 1]) {
		p->puzzle[row - 1][col - 1] = input[2];
		return true;
	}
	else {
		printf("/*请不要尝试修改题目哦\n");
		return false;
	}
}


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
	printf("/*棋盘:\n");
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
	std::string year, month, day, hour, minute, time_str;
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
	return ".\\puzzleCnfFile\\" + getTimeStr() + ".cnf";
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