/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: ����������                                            **/
/**********************************************************/

#include"naive_implementation/display/display.h"
#include"config/config.h"
#include"binary_puzzle/solve_puzzle.h"
#include"binary_puzzle/test.h"
#include"display/display.h"
#include"display/test.h"

int main()
{
	srand((unsigned int)time(NULL));
	if (puzzle_size >= 4 and !(puzzle_size % 2))
		display();
	else printf("�����������̴�С�Ƿ�Ϸ�, �������λ��config/config.h��\n");
	//testLoadFileOpti();
	/*while(true)
		testCnfSolverOpti();*/
	system("Pause");
	return 0;
}