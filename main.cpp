/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/21                                                     
//* Description: ����������                                            
/**********************************************************/

#include"config/config.h"
#include"display/display.h"
#include"display/test.h"
#include"walksat-dpll/data_struct.h"
#include"walksat-dpll/parser.h"
#include"walksat-dpll/test.h"

int main()
{
	srand((unsigned int)time(NULL));
	if (puzzle_size >= 4 and !(puzzle_size % 2))
		display();
	else printf("�����������̴�С�Ƿ�Ϸ�, �������λ��config/config.h��\n");
	//testLoadFileOpti();
	/*while(true)
		testCnfSolverOpti();*/
	//while (true) {
	//	//test_WALKSAT_DPLL();
	//	testCnfSolverOpti();
	//	/*callCnfSolver();*/
	//	//test_WALKSAT();
	//}
	
	system("Pause");
	return 0;
}