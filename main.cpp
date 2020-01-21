/***********************************************************/
/** Author: Zhang DY                                                     **/
/** Date: 2020/01/21                                                     **/
/** Description: 运行主程序                                            **/
/**********************************************************/

#include"display.h"
#include"config.h"

int main()
{
	struct Formula* formula;
	char* filename = getFileName();

#ifdef DEBUG
	debug(filename);
#endif // DEBUG

#ifndef DEBUG
	calculate(filename);
#endif // !DEBUG
	//free(filename);
	system("Pause");
	return 0;
}