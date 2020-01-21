#include"cnfparser.h"
#include"solver.h"

int main()
{
	struct Formula* formula;

#ifndef DEBUG
char filename[255];
	printf("请输入cnf文件路径(斜杠请用双斜杠表示): ");
	scanf("%s", filename);
	formula = loadFile(filename);
	int* result = (int*)malloc(sizeof(int) * info.num_literal);
#endif // !DEBUG

#ifdef DEBUG
	printf("原公式:\n");
	formula = loadFile("E:\\课程设计\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\基准算例\\功能测试\\sat-20.cnf");
	std::cout << std::endl;
	//removeClauseHasLiteral(formula, 4);

	/*struct Formula* formula_copy = copyFormula(formula);
	printf("\n复制后的公式:\n");
	printFormula(formula_copy);

	printf("\n原公式删除文字\"4\"后的原公式:\n");
	removeLiteralFromFormula(formula, 4);
	printFormula(formula);
	

	printf("\n原公式删除文字\"4\"后的复制公式:\n");
	printFormula(formula_copy);*/


#endif // DEBUG
}