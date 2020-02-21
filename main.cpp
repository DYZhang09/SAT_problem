/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: 运行主程序                                            **/
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
	else printf("请检查数独棋盘大小是否合法, 相关设置位于config/config.h中\n");
	//testLoadFileOpti();
	/*while(true)
		testCnfSolverOpti();*/
	system("Pause");
	return 0;
}