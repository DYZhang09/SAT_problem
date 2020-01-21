/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: 运行主程序                                            **/
/**********************************************************/

#include"display.h"
#include"config.h"

int main()
{
	char c;
	struct Formula* formula;

	printHelp();
	do {
		char* filename = getFileName();
#ifdef DEBUG
		debug(filename);
#endif // DEBUG

#ifndef DEBUG
		calculate(filename);
#endif // !DEBUG
		//free(filename);
		printf("按ESC退出, 其他键继续\n");
	} while ((c = getchar()) != 27);
	system("Pause");
	return 0;
}