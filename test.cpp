#include"cnfparser.h"

int main()
{
	struct Formula* formula;

#ifndef DEBUG
char filename[255];
	printf("������cnf�ļ�·��(б������˫б�ܱ�ʾ): ");
	scanf("%s", filename);
	formula = loadFile(filename);
	int* result = (int*)malloc(sizeof(int) * info.num_literal);
#endif // !DEBUG

#ifdef DEBUG
	formula = loadFile("E:\\�γ����\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\��׼����\\���ܲ���\\sat-20.cnf");
	std::cout << std::endl;
	//removeClauseHasLiteral(formula, 4);
	removeLiteralFromFormula(formula, 4);
	printFormula(formula);
	//loadFile("E:\\�γ����\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\����������\\eu-rand_net60-25-10.shuffled-3000.cnf");
#endif // DEBUG
}