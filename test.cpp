#include"cnfparser.h"
#include"solver.h"

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
	printf("ԭ��ʽ:\n");
	formula = loadFile("E:\\�γ����\\��������ۺϿγ��������ָ��ѧ����\\SAT���Ա�ѡ����\\��׼����\\���ܲ���\\sat-20.cnf");
	std::cout << std::endl;
	//removeClauseHasLiteral(formula, 4);

	/*struct Formula* formula_copy = copyFormula(formula);
	printf("\n���ƺ�Ĺ�ʽ:\n");
	printFormula(formula_copy);

	printf("\nԭ��ʽɾ������\"4\"���ԭ��ʽ:\n");
	removeLiteralFromFormula(formula, 4);
	printFormula(formula);
	

	printf("\nԭ��ʽɾ������\"4\"��ĸ��ƹ�ʽ:\n");
	printFormula(formula_copy);*/


#endif // DEBUG
}