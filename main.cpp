/***********************************************************/
//* Author: Zhang DY                                                     
//* Date: 2020/01/21                                                     
//* Description: ����������                                            
/**********************************************************/

#include"display/display.h"

int main()
{
	srand((unsigned int)time(NULL));
	if (puzzle_size >= 4 and !(puzzle_size % 2))
		display();
	else printf("�����������̴�С�Ƿ�Ϸ�, �������λ��config/config.h��\n");
	
	system("Pause");
	return 0;
}