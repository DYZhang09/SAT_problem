/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: ����������                                            **/
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
		printf("��ESC�˳�, ����������\n");
		c = getchar();
	} while ((c = getch()) != 27);
	printf("���˳�\n");
	system("Pause");
	return 0;
}