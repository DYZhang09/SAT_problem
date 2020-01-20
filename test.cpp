#include"cnfparser.h"

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
	formula = loadFile("E:\\课程设计\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\基准算例\\功能测试\\sat-20.cnf");
	std::cout << std::endl;
	//removeClauseHasLiteral(formula, 4);
	removeLiteralFromFormula(formula, 4);
	printFormula(formula);
	//loadFile("E:\\课程设计\\程序设计综合课程设计任务及指导学生包\\SAT测试备选算例\\不满足算例\\eu-rand_net60-25-10.shuffled-3000.cnf");
#endif // DEBUG
}